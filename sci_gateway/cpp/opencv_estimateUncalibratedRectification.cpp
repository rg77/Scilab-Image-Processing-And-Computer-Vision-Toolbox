#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <iostream>
using namespace cv;
using namespace std;
extern "C"
{
  #include "api_scilab.h"
  #include "Scierror.h"
  #include "BOOL.h"
  #include <localization.h>
  #include <sciprint.h>
  #include "../common.h"
  
  int opencv_estimateUncalibratedRectification(char *fname, unsigned long fname_len)
  {
   SciErr sciErr;
   int intErr=0;
   int iComplex = 0;
   int iType = 0;
   int *piAddr  = NULL;
   int i,j;
   int iA1Rows=0,iA1Cols=0,iA2Rows=0,iA2Cols=0;
   int fmRows=0,fmCols=0,imageRows=0,imageCols=0;
   double *fm=NULL,*iArray1=NULL,*iArray2=NULL,*imageSize=NULL;
   double *oArray1=NULL,*oArray2=NULL;
   Mat fundamentalMatrix(3,3,CV_64F);
   int imagesize[2];
   vector<Point2f> points1,points2;
   
   CheckInputArgument(pvApiCtx, 4, 4);
   CheckOutputArgument(pvApiCtx, 2, 2) ;
   
   
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	sciErr = getVarType(pvApiCtx, piAddr, &iType);
	if(sciErr.iErr || iType != sci_matrix)
	{
		printError(&sciErr, 0);
		return 0;
	}
	iComplex = isVarComplex(pvApiCtx, piAddr);
	if(iComplex)
	{
		Scierror(999, "%s: Wrong type for input argument: A complex number is not expected.\n");
		return 0;
	}	
 	sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &fmRows, &fmCols, &fm);
 	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
   
   
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	sciErr = getVarType(pvApiCtx, piAddr, &iType);
	if(sciErr.iErr || iType != sci_matrix)
	{
		printError(&sciErr, 0);
		return 0;
	}
	iComplex = isVarComplex(pvApiCtx, piAddr);
	if(iComplex)
	{
		Scierror(999, "%s: Wrong type for input argument: A complex number is not expected.\n");
		return 0;
	}	
 	sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iA1Rows, &iA1Cols, &iArray1);
 	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	
	
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	sciErr = getVarType(pvApiCtx, piAddr, &iType);
	if(sciErr.iErr || iType != sci_matrix)
	{
		printError(&sciErr, 0);
		return 0;
	}
	iComplex = isVarComplex(pvApiCtx, piAddr);
	if(iComplex)
	{
		Scierror(999, "%s: Wrong type for input argument: A complex number is not expected.\n");
		return 0;
	}	
 	sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iA2Rows, &iA2Cols, &iArray2);
 	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	
	
	sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	sciErr = getVarType(pvApiCtx, piAddr, &iType);
	if(sciErr.iErr || iType != sci_matrix)
	{
		printError(&sciErr, 0);
		return 0;
	}
	iComplex = isVarComplex(pvApiCtx, piAddr);
	if(iComplex)
	{
		Scierror(999, "%s: Wrong type for input argument: A complex number is not expected.\n");
		return 0;
	}	
 	sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &imageRows, &imageCols, &imageSize);
 	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	
	if(fmRows!=3 || fmCols!=3)
	{
		Scierror(999, "Fudamental matrix must be of size 3*3\n"); 
        return 0;
	}
	if(iA1Rows != iA2Rows)
    {
    	Scierror(999, "There must be the same number of points in both input arrays\n"); 
        return 0; 
    }
    if(imageRows!=1 || (imageCols!=2 && imageCols!=3))
	{
		Scierror(999, "Image size must be of vector of  with 2 or 3 elements\n"); 
        return 0;
	}
	for(i = 0 ; i < 3 ; i++)
	{
		for(j = 0 ; j < 3 ; j++)
		{
			fundamentalMatrix.at<double>(i,j) = fm[i+3*j];
		}
	}
	for(i=0;i<iA1Rows;i++)
	{
		points1.push_back(Point2f(iArray1[i],iArray1[iA1Rows+i]));
		points2.push_back(Point2f(iArray2[i],iArray2[iA2Rows+i]));
	}
	Mat H1,H2;
	if (imageCols==2)
		stereoRectifyUncalibrated(points1, points2, fundamentalMatrix,Size(imageSize[0],imageSize[1]), H1, H2);
    else if (imageCols==3)
    	stereoRectifyUncalibrated(points1, points2, fundamentalMatrix,Size(imageSize[0],imageSize[1],imageSize[2]), H1, H2);
	oArray1 = (double*) malloc(sizeof(double) * 3 *3);
	oArray2 = (double*) malloc(sizeof(double) * 3 *3);
	for(i = 0 ; i < 3 ; i++)
	{
		for(j = 0 ; j < 3 ; j++)
		{
			oArray1[i + 3 * j] = H1.at<double>(i,j);
			oArray2[i + 3 * j] = H2.at<double>(i,j);
		}
	}
	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 3 , 3, oArray1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2, 3 , 3, oArray2);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
    ReturnArguments(pvApiCtx);
    return 0;
  }
}
