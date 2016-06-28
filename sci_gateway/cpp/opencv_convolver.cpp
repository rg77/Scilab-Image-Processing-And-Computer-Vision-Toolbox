/******************************************************************************************
*Author : Kevin George
*
*-> To execute, Image = convolver(I1, size, values,scalar)
*   where 'I1' is image to be convoluted,
*   where 'size' is size of kernel i.e size x size gives total no. of values in kernel,
*   where 'values' contains the values of the kernel 
*   where 'scalar' is a float value
*
*******************************************************************************************/
#include <numeric>
#include <string.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include <iostream>
#include <math.h>
#include <vector>

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
    // # include "../common.cpp"

    int opencv_convolver(char *fname, unsigned long fname_len)
    {
    	//-> Error Management variables
    	SciErr sciErr;
    	int intErr=0;

        //-> Mat containers for images
        Mat image_1;
        Mat image_2;

        //-> Address of Various Arguments
        int *piAddr = NULL;

        //-> Local Variables
        double *values1 = NULL;
        float *values2 =NULL;
        double size;
        double scalar;
        int iRows = 0; 
        int iCols = 0;
        int *outList = NULL;
        unsigned char *red = NULL;
        unsigned char *green = NULL;
        unsigned char *blue = NULL;

        //-> Checks the number of arguments
        //-> pvApiCtx is a Scilab environment pointer
        CheckInputArgument(pvApiCtx, 1, 5);                     //Check on Number of Input Arguments
        CheckOutputArgument(pvApiCtx, 1, 5);                    //Check on Number of Output Arguments

    	//-> Read Image
        retrieveImage( image_1, 1);
        //retrieveImage(image_2, 2);

        //-> Count number of input arguments
        //num_InputArgs = *getNbInputArgument(pvApiCtx);

        //-> Based on number of input arguments

//*****************************************************  Actual Processing  *************************************************************

     	//-> Taking in size of kernel
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr); 
        if (sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }
                    
        //intErr = getScalarInteger32(pvApiCtx, piAddr, &size);
        intErr = getScalarDouble(pvApiCtx, piAddr, &size); 
        if(intErr)
        {
             return intErr; 
        }

        if( size > 5)
        {
              Scierror(999," Invalid Value size. Please enter a non negative Double value\\n");
              return 0;
        }   

        //-> Taking the kernel values
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr); 
        if (sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }

        sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &values1);
        //sciErr =  getMatrixOfInteger32(pvApiCtx, piAddr, &iRows, &iCols, &values); 
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        if(iRows*iCols!=9 && iRows*iCols!=16 && iRows*iCols!=25 )
        {   
    	    Scierror(999,"Invalid Argument\n");
            return 0;
        }           

       	values2 = (float*)malloc(sizeof(float)*size*size);
        for(int i=0; i<size*size; i++)
        {
            values2[i] = (float)values1[i];
        }

        
        //-> Taking scalar values
        sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr); 
        if (sciErr.iErr)
        {
            printError(&sciErr, 0); 
            return 0; 
        }

        intErr = getScalarDouble(pvApiCtx, piAddr, &scalar); 
        if(intErr)
        {
             return intErr; 
        }

        if( scalar == 0)
        {
              Scierror(999," Invalid scalar value. Please enter a non negative Double value\\n");
              return 0;
        }

        //float kdata[] = {1, 4, 6, -1, 3, 5, -1, -2, 2};
  		Mat kernel(size,size,CV_32F, values2);

  	    Mat kernel2 = kernel/ (float)(scalar);

        Point anchor;
        double delta;
        int ddepth;

        anchor = Point( -1, -1 );
        delta = 0;
        ddepth = -1;

        //filter2D(image_1, image_2, CV_32F, kernel);
        filter2D(image_1, image_2, ddepth,kernel2,anchor,delta, BORDER_DEFAULT);
  	    
  	    //imshow("Convoluted Image",image_2);
  	    //waitKey(0);

        if( image_2.channels() == 1)
        {
            sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, &outList);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            red = (unsigned char *)malloc(sizeof(unsigned char)*image_2.rows*image_2.cols);

            for(int k=0;k<image_2.rows;k++)
                for(int p=0;p<image_2.cols;p++)
                    red[k+image_2.rows*p]=image_2.at<uchar>(k, p);

            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 1, image_2.rows, image_2.cols, red);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }                       
            free(red);
        }

        else
        {
            sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 3, &outList);
            if(sciErr.iErr)
            {
                    printError(&sciErr, 0);
                    return 0;
            }

            red = (unsigned char *)malloc(sizeof(unsigned char)*image_2.rows*image_2.cols);
            green = (unsigned char *)malloc(sizeof(unsigned char)*image_2.rows*image_2.cols);
            blue = (unsigned char *)malloc(sizeof(unsigned char)*image_2.rows*image_2.cols);

            for(int k=0;k<image_2.rows;k++)
            {
                for(int p=0;p<image_2.cols;p++)
                {
                    Vec3b intensity = image_2.at<Vec3b>(k, p);
                    red[k+image_2.rows*p]=intensity.val[2];
                    green[k+image_2.rows*p]=intensity.val[1];
                    blue[k+image_2.rows*p]=intensity.val[0];
                }
            }

            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 1, image_2.rows, image_2.cols, red);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
                sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 2, image_2.rows, image_2.cols, green);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }                   
                sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, outList, 3, image_2.rows, image_2.cols, blue);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    return 0;
                }
                free(red);
                free(green);
                free(blue); 

        }

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        ReturnArguments(pvApiCtx);
        return 0;
    }

}