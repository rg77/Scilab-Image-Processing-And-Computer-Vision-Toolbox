/***************************************************
Author : Tanmay Chaudhari
***************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <bits/stdc++.h>
using namespace cv;
using namespace std;
extern "C"
{
    #include "api_scilab.h"
    #include "Scierror.h"
    #include "BOOL.h"
    #include <localization.h>
    #include "sciprint.h"
 	#include "../common.h"
 //   #include "../common.cpp"

int opencv_integralImage(char *fname, unsigned long fname_len)
{
	//Error management variable
	SciErr sciErr;
	
	//Variable declaration
	int nbInputArguments = 0;
	int iRows = 0;
	int iCols = 0;
	int *piLen = NULL;
	int *piAddr = NULL;
	int tilted = 0;
	char **pstData  = NULL;
	double *integralImage;
	Mat image, sum, sqsum, tiltedsum;

	//Check input output arguments
	checkInputArgument(pvApiCtx, 1, 2);
	checkOutputArgument(pvApiCtx, 1, 1);

	nbInputArguments = *getNbInputArgument(pvApiCtx);
	
	//getting input arguments
	retrieveImage(image, 1); 

	if(nbInputArguments == 2)
	{
		sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr); 
		if (sciErr.iErr)
		{
			printError(&sciErr, 0); 
			return 0; 
		}

		// Extracting name of next argument takes three calls to getMatrixOfString
		sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL); 
		if (sciErr.iErr)
		{
			printError(&sciErr, 0); 
			return 0; 
		}

		piLen = (int*) malloc(sizeof(int) * iRows * iCols); 

		sciErr = getMatrixOfString(pvApiCtx,  piAddr,  &iRows,  &iCols,  piLen,  NULL); 
		if (sciErr.iErr)
		{
			printError(&sciErr, 0); 
			return 0; 
		}

		pstData = (char**) malloc(sizeof(char*) * iRows * iCols); 
		for(int iterPstData = 0; iterPstData < iRows * iCols; iterPstData++)
		{
			pstData[iterPstData] = (char*) malloc(sizeof(char) * piLen[iterPstData] + 1); 
		}

		sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, pstData); 
		if (sciErr.iErr)
		{
			printError(&sciErr, 0); 
			return 0; 
		}

		if(strcmp(pstData[0],"upright")==0)
			tilted=0;
		else if(strcmp(pstData[0],"rotated")==0)
			tilted=1;
		else
		{
			Scierror(999, "Error: Please provide proper value for \"%s\". Permissible values are upright or rotated.\n", pstData[0]); 
			return 0; 
		}
	}

	integral(image,sum,sqsum,tiltedsum);

	if(tilted==0)
	{
		integralImage = (double*)malloc(sizeof(double) * int(sum.rows) * int(sum.cols));
		for(int i=0;i<sum.rows;i++)
		{
			for(int j=0;j<sum.cols;j++)
			{
				integralImage[j*int(sum.rows)+i] = sum.at<int>(i,j);
			}
		}
	}
	else
	{
		integralImage = (double*)malloc(sizeof(double) * int(sum.rows) * (int(sum.cols)+1));
		for(int i=0;i<sum.rows;i++)
		{
			for(int j=0;j<=sum.cols;j++)
			{
				integralImage[j*sum.rows+i]=0;
			}
		}
		for(int i=0;i<sum.rows;i++)
		{
			for(int j=0;j<sum.cols;j++)
			{
					integralImage[j*int(sum.rows)+i] = tiltedsum.at<int>(i,j);
			}
		}
	}

	sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(tilted==0)
		sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 1, int(sum.rows), int(sum.cols), integralImage);
	else
		sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 1, int(sum.rows), int(sum.cols)+1, integralImage);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	//Return output arguments
	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	ReturnArguments(pvApiCtx);
	
	return 0;
}
/* ==================================================================== */
}
