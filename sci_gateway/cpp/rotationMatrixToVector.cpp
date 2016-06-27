/***************************************************
Author : Shashank Shekhar
**************************************************/

#include <bits/stdc++.h>
using namespace std;
extern "C"
{
    #include "api_scilab.h"
    #include "Scierror.h"
    #include "BOOL.h"
    #include <localization.h>
    #include "sciprint.h"

	int rotationMTV(char *fname, unsigned long fname_len)
	{
		SciErr sciErr;
		int *piAddr = NULL;
		int iRows = 0;
        int iCols = 0;
		int *piLen = 0;
		char **pstData = NULL;
		double RotMat[9];
        double *temp = NULL;
    	int noOfarguments = *getNbInputArgument(pvApiCtx);
        double outMat[3];
        double theta;
    	int i;



	    CheckInputArgument(pvApiCtx,1,1);
	    CheckOutputArgument(pvApiCtx,1,1);	

    	sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
        if (sciErr.iErr)
        {
            printError(&scisErr, 0);
            return 0;
        }

        if(!isDoubleType(pvApiCtx, piAddr) || isVarComplex(pvApiCtx, piAddr))
        {
            Scierror(999,"A 3x3 Matrix expected.\n");
            return 0;
        }

        sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &temp);  
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if(iRows!=3 || iCols!=3)
        {
            Scierror(999,"Invalid Argument\n");
            return 0;
        }
          
        for(i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                RotMat[j*3+i]=temp[i*3+j];    
            }
        }

//  -----------------------------------------------------------  Evaluation  --------------------------------------------------------

        double myMat[3][3];
        for(int i=0;i<3;i++)
            for(int j=0;j<3;j++)
                myMat[i][j] = RotMat[i*3+j];
       
        double trace = RotMat[0]+RotMat[4]+RotMat[8];
        theta = acos((trace-1)/2);       
        outMat[0] = myMat[2][1]-myMat[1][2];
        outMat[1] = myMat[0][2]-myMat[2][0];
        outMat[2] = myMat[1][0]-myMat[0][1];

        double element;
        double threshold = 1e-4;
        if(sin(theta) >= threshold)
        {
            element = theta/(2*sin(theta));           
            for(int i=0;i<3;i++)
                outMat[i]=outMat[i]*element;          
        }
        else if (theta-1 > 0)
        {
            element = (0.5-(theta-3)/12);
            for(int i=0;i<3;i++)
                outMat[i]=outMat[i]*element;
        }
        else
        {
            double myMax=myMat[0][0];
            int iter1=0;
            int iter2,iter3;
            for(int i=0;i<3;i++)
            {
                if(myMat[i][i]>myMax)
                {
                    myMax=myMat[i][i];
                    iter1=i;
                }
            }

            iter2 = (iter1%2) +1;
            iter3 = ((iter1+1)%2) +1;

            double ev = sqrt(myMat[iter1][iter1] - myMat[iter2][iter2] - myMat[iter3][iter3] + 1);
            outMat[0] = ev/2;
            outMat[1] = (myMat[iter2][iter1] + myMat[iter1][iter2])/(2*ev);
            outMat[2] = (myMat[iter3][iter1] + myMat[iter1][iter3])/(2*ev);

            for(int i=0;i<3;i++)
                ev+=pow(outMat[i],2);
            ev = sqrt(ev);

            for(int i=0;i<3;i++)
                outMat[i]=(outMat[i]/ev)*theta;
        }

//		--------------------------------------------------------Creating 1x3 Mat to Return -------------------------------------------------------------

	    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 3,1,outMat);
	    if(sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	        return 0;
	    }

	    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	    ReturnArguments(pvApiCtx);
	    return 0;
    }
}