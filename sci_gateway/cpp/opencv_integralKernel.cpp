/***************************************************
Author : Tanmay Chaudhari
 ***************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;
extern "C"
{
#include "api_scilab.h"
#include "Scierror.h"
#include "BOOL.h"
#include <localization.h>
#include "sciprint.h"
	//    #include "../common.h"
	//    #include "../common.cpp"

	int opencv_integralKernel(char *fname, unsigned long fname_len)
	{
		//Error management variable 
		SciErr sciErr;

		//Variable declaration
		int i, j, k;
		int iType = 0;
		int iComplex = 0;
		int rowsOfBbox = 0;
		int rowsOfWeights = 0;
		int colsOfBbox = 0;
		int colsOfWeights = 0;
		int *piAddr = NULL;
		double *filterSize = NULL;
		double *hlSize = NULL;
		double *ulCorner = NULL;
		double *lrCorner = NULL;
		double *centre = NULL;
		double sR = 0;
		double sC = 0;
		double eR = 0;
		double eC = 0;
		double *coefficients = NULL;
		double *bbox = NULL;
		double *weights = NULL;

		//Check input output argument
		checkInputArgument(pvApiCtx, 2, 2);
		checkOutputArgument(pvApiCtx, 5, 5);

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

		sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &rowsOfBbox, &colsOfBbox, &bbox);
		if(sciErr.iErr || colsOfBbox != 4 || rowsOfBbox == 0)
		{
			printError(&sciErr, 0);
			return 0;
		}

		//Get variable address of the second input arguent
		sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
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

		sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &rowsOfWeights, &colsOfWeights, &weights);
		if(sciErr.iErr || colsOfWeights == 0 || rowsOfWeights != 1 || colsOfWeights != rowsOfBbox)
		{
			printError(&sciErr, 0);
			return 0;
		}

		//Creating list to storing bouding boxes
		sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, &piAddr);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 1, rowsOfBbox, colsOfBbox, bbox);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		//Creating list to store weights
		sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 2, 1, &piAddr);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 2, piAddr, 1, rowsOfWeights, colsOfWeights, weights);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		//Computing filter size
		filterSize = (double*)malloc(sizeof(double) * 2);
		hlSize = (double*)malloc(sizeof(double) * 2);
		ulCorner = (double*)malloc(sizeof(double) * 2);
		lrCorner = (double*)malloc(sizeof(double) * 2);

		filterSize[0] = filterSize[1] = 0;
		for( i = 0 ; i < rowsOfBbox; i++)
		{
			hlSize[0] = bbox[3 * rowsOfBbox + i];
			hlSize[1] = bbox[2 * rowsOfBbox + i];

			ulCorner[0] = bbox[1 * rowsOfBbox + i];
			ulCorner[1] = bbox[0 * rowsOfBbox + i];

			lrCorner[0] = ulCorner[0] + hlSize[0] - 1;	
			lrCorner[1] = ulCorner[1] + hlSize[1] - 1;

			if(filterSize[0] < lrCorner[0])
				filterSize[0] = lrCorner[0];
			if(filterSize[1] < lrCorner[1])
				filterSize[1] = lrCorner[1];
		}
	
		//Computing coefficient matrix
		coefficients = (double*)malloc(sizeof(double) * int(filterSize[0]) * int(filterSize[1]));
		for( i = 0; i < int(filterSize[0]) * int(filterSize[1]); i++)
			coefficients[i] = 0;

		for( i = 0; i < rowsOfBbox; i++)
		{
			sR = bbox[1 * rowsOfBbox + i];
			sC = bbox[0 * rowsOfBbox + i];
			eR = sR + bbox[3 * rowsOfBbox + i] - 1;
			eC = sC + bbox[2 * rowsOfBbox + i] - 1;

			for( j = sR - 1; j < eR; j++)
				for( k = sC - 1; k < eC; k++)
					coefficients[k * int(filterSize[0]) + j] += weights[i];
		}

		//Computing centre of filter
		centre = (double*)malloc(sizeof(double) * 2);
		centre[0] = ceil(filterSize[0]/2.0);
		centre[1] = ceil(filterSize[1]/2.0);
	
		//Storing coefficient matrix in list
		sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 3, 1, &piAddr);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 3, piAddr, 1, int(filterSize[0]), int(filterSize[1]), coefficients);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		
		//Storing centre of filter in list
		sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 4, 1, &piAddr);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 4, piAddr, 1, 1, 2, centre);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
		
		//Storing size of filter in list
		sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 5, 1, &piAddr);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 5, piAddr, 1, 1, 2, filterSize);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		//Return output arguments
		AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
		AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
		AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx) + 3;
		AssignOutputVariable(pvApiCtx, 4) = nbInputArgument(pvApiCtx) + 4;
		AssignOutputVariable(pvApiCtx, 5) = nbInputArgument(pvApiCtx) + 5;
		ReturnArguments(pvApiCtx);

		return 0;
	}
	/* ==================================================================== */
}
