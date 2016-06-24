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
 //   #include "../common.h"
 //   #include "../common.cpp"

int opencv_integralImage(char *fname, unsigned long fname_len)
{
	//Error management variable
	SciErr sciErr;
	
	//Variable declaration
	int i, j;
	int iComplex = 0;
	int iType = 0;
	int iRows = 0;
	int iCols = 0;
	int *piAddr = NULL;
	double *pdblReal = NULL;
	double *integralImage = NULL;
	double *tempArray = NULL;	
	
	//Check input output arguments
	checkInputArgument(pvApiCtx, 1, 1);
	checkOutputArgument(pvApiCtx, 1, 1);

	//Get variable address of the first input arguent
	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
	if(sciErr.iErr)
	{		
		printError(&sciErr, 0);
		return 0;
	}

	//Check type
	sciErr = getVarType(pvApiCtx, piAddr, &iType);
	if(sciErr.iErr || iType != sci_matrix)
	{
		printError(&sciErr, 0);
		return 0;
	}

	//Get complexity
	iComplex = isVarComplex(pvApiCtx, piAddr);
	
	//Check complexity
	if(iComplex)
	{
		Scierror(999, "%s: Wrong type for input argument: A complex number is not expected.\n");
		return 0;
	}
	
	sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal);
	if(sciErr.iErr || iCols == 0 || iRows == 0)
	{
		printError(&sciErr, 0);
		return 0;
	}
	
	tempArray = (double*)malloc(sizeof(double) * iRows * iCols);
	integralImage = (double*)malloc(sizeof(double) * (iRows) * (iCols));
	memset(integralImage, 0, sizeof(integralImage));
	memset(tempArray, 0, sizeof(tempArray));
	
	for( i = 0; i < iRows; i++)
	{
		for( j = 0; j < iCols; j++)
		{
			tempArray[j * iRows + i] = pdblReal[j * iRows + i];
			if((j - 1) >= 0)
				tempArray[j * iRows + i] += tempArray[(j - 1) * iRows + i];
		}
	}

	for( i = 0; i < iRows; i++)
	{
		for( j = 0; j < iCols; j++)
		{
			integralImage[j * iRows + i] = tempArray[j * iRows + i];
		
			if((i - 1) >= 0)
				integralImage[j * iRows + i] += integralImage[j * iRows + (i - 1)];
		}
	}
	
	free(tempArray);
	tempArray = (double*)malloc(sizeof(double) * (iRows + 1) * (iCols + 1));

	for( i = 0; i <= iRows; i++)
	{
		for( j = 0; j <= iCols; j++)
		{
			if(i == 0 || j == 0)
				tempArray[j * (iRows + 1) + i] = 0;
			else
				tempArray[j * (iRows + 1) + i] = integralImage[(j - 1) * iRows + (i - 1) ];
		}
	}
	
	free(integralImage);

	sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 1, iRows + 1, iCols + 1, tempArray);
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
