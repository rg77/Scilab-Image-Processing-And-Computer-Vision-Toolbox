/***************************************************
Author : Tanmay Chaudhari, Shashank Shekhar
 ***************************************************/

#include <bits/stdc++.h>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/imgproc/imgproc.hpp"

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
	//#include "../common.cpp"
	pair<int, int > displacementVector[1005][1005];

	int opencv_BlockMatcher(char *fname, unsigned long fname_len)
	{	
		//Error management variable
		SciErr sciErr;

		//Variable declaration
		int maxm, itr;
		int iRows = 0;
		int iCols = 0;
		int nbInputArguments = 0;	
		int *piLen = NULL;
		int *piAddr = NULL;
		bool *providedArgs = NULL;
		char *currentArg = NULL; 
		char **pstData  = NULL;	
		double *blocksize = NULL;
		double *maximumDisplacement = NULL;
		double *overlap = NULL;
		double *horizontal = NULL;
		double *vertical = NULL;
		double *magnitude = NULL;
		vector< KeyPoint > keypoints;
		vector< KeyPoint > keypointsSecondImage;
		vector< DMatch > matches;
		vector< pair< vector< DMatch >, pair<int, int > > > matchesPerBlock;
		string s,t;
		string matchCriteria;
		string outputValue;
		Mat img1, img2, cropped, feature_des1, feature_des2;
		Mat croppedBlock;

		//Check input output arguments
		CheckInputArgument(pvApiCtx, 1, 12);
		CheckOutputArgument(pvApiCtx, 1, 1);

		nbInputArguments = *getNbInputArgument(pvApiCtx);

		if((nbInputArguments-2)%2!=0)
		{
			Scierror(999, "Incorrect number of arguments provided. Please check the documentation for more information.\n"); 
			return 0; 
		}

		providedArgs = (bool*)malloc(sizeof(bool) * 5);
		for(int i=0;i<5;i++)
			providedArgs[i] = 0;

		//Read image
		retrieveImage(img1, 1); 
		retrieveImage(img2, 2); 

		for(int iter=3;iter<=nbInputArguments;iter++)
		{
			// Getting address of next argument
			sciErr = getVarAddressFromPosition(pvApiCtx, iter, &piAddr); 
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

			currentArg = pstData[0];
			// providedArgs[] makes sure that no optional argument is provided more than once

			if(strcmp(currentArg,"BlockSize")==0)
			{
				if(!providedArgs[0])
				{
					sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
					if (sciErr.iErr)
					{
						printError(&sciErr, 0); 
						return 0; 
					}
					if(!isDoubleType(pvApiCtx, piAddr) || isVarComplex(pvApiCtx, piAddr))
					{
						Scierror(999, "%s: Wrong type for input argument #%d: A real matrix expected.\n", fname, iter); 
						return 0; 
					}
					sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &blocksize); 
					if(sciErr.iErr)
					{
						printError(&sciErr, 0); 
						return 0; 
					}
					if(iRows!=1 || iCols!=2)
					{
						Scierror(999, "Incorrect dimension of matrix for argument BlockSize.\n"); 
						return 0; 
					}
					if(blocksize[0]<=0 || blocksize[1]<=0)
					{
						Scierror(999, "Positive inputs for BlockSize needed.\n"); 
						return 0; 
					}
					providedArgs[0]=1;
				}
				else if(providedArgs[0])
				{
					Scierror(999, "Please provide optional arguments only once.\n"); 
					return 0; 
				}	
			}
			else if(strcmp(currentArg, "Overlap")==0)
			{
				if(!providedArgs[1])
				{
					sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
					if (sciErr.iErr)
					{
						printError(&sciErr, 0); 
						return 0; 
					}
					if(!isDoubleType(pvApiCtx, piAddr) || isVarComplex(pvApiCtx, piAddr))
					{
						Scierror(999, "%s: Wrong type for input argument #%d: A real matrix expected.\n", fname, iter); 
						return 0; 
					}
					sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &overlap); 
					if(sciErr.iErr)
					{
						printError(&sciErr, 0); 
						return 0; 
					}
					if(iRows!=1 || iCols!=2)
					{
						Scierror(999, "Incorrect dimension of matrix for argument Overlap.\n"); 
						return 0; 
					}
					if(overlap[0]<0 || overlap[1]<0)
					{
						Scierror(999, "Non-negative inputs for Overlap needed\n");
						return 0;
					}
					providedArgs[1]=1;

				}
				else if(providedArgs[1])
				{
					Scierror(999, "Please provide optional arguments only once.\n"); 
					return 0; 
				}	
			}
			else if(strcmp(currentArg, "MaximumDisplacement")==0)
			{
				if(!providedArgs[2])
				{
					sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
					if (sciErr.iErr)
					{
						printError(&sciErr, 0); 
						return 0; 
					}
					if(!isDoubleType(pvApiCtx, piAddr) || isVarComplex(pvApiCtx, piAddr))
					{
						Scierror(999, "%s: Wrong type for input argument #%d: A real matrix expected.\n", fname, iter); 
						return 0; 
					}
					sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &maximumDisplacement); 
					if(sciErr.iErr)
					{
						printError(&sciErr, 0); 
						return 0; 
					}
					if(iRows!=1 || iCols!=2)
					{
						Scierror(999, "Incorrect dimension of matrix for argument MaximumDisplacement.\n"); 
						return 0; 
					}
					if(maximumDisplacement[0]<0 || maximumDisplacement[1]<0)
					{
						Scierror(999, "Non-negative inputs for Overlap needed\n");
						return 0;
					}
					providedArgs[2]=1;

				}
				else if(providedArgs[2])
				{
					Scierror(999, "Please provide optional arguments only once.\n"); 
					return 0; 
				}	
			}
			else if(strcmp(currentArg, "MatchCriteria")==0)
			{
				if(!providedArgs[3])
				{
					sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
					if (sciErr.iErr)
					{
						printError(&sciErr, 0); 
						return 0; 
					}

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

					if(strcmp(pstData[0],"Mean square error")==0)
						matchCriteria = "Mean Square Error";
					else if(strcmp(pstData[0], "Mean absolute difference")==0)
						matchCriteria = "	";
					else
					{
						Scierror(999, "Error: Please provide proper value for \"%s\". Permissible values are Mean square error or Mean absolute difference.\n", currentArg); 
						return 0;
					}
					providedArgs[3]=1;
				}
				else if(providedArgs[3])
				{
					Scierror(999, "Please provide optional arguments only once.\n"); 
					return 0; 
				}
			}
			else if(strcmp(currentArg, "OutputValue")==0)
			{
				if(!providedArgs[4])
				{
					sciErr = getVarAddressFromPosition(pvApiCtx, ++iter, &piAddr); 
					if (sciErr.iErr)
					{
						printError(&sciErr, 0); 
						return 0; 
					}

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

					if(strcmp(pstData[0],"Magnitude-squared")==0)
						outputValue = "Magnitude-squared";
					else if(strcmp(pstData[0], "Horizontal and vertical components")==0)
						outputValue = "Horizontal and vertical components";
					else
					{
						Scierror(999, "Error: Please provide proper value for \"%s\". Permissible values are Magnitude-squared or Horizontal and vertical components.\n", currentArg); 
						return 0;
					}	
					providedArgs[4]=1;
				}
				else if(providedArgs[4])
				{
					Scierror(999, "Please provide optional arguments only once.\n"); 
					return 0; 
				}
			}
		}

		//Initialisaton
		if(!providedArgs[0])
		{
			blocksize = (double*)malloc(sizeof(double) * 2);
			blocksize[0] = 17;
			blocksize[1] = 17;
		}

		if(!providedArgs[1])
		{
			overlap = (double*)malloc(sizeof(double) * 2);
			overlap[0] = 0;
			overlap[1] = 0;
		}

		if(!providedArgs[2])
		{
			maximumDisplacement = (double*)malloc(sizeof(double) * 2);
			maximumDisplacement[0] = 7;
			maximumDisplacement[1] = 7;
		}

		if(!providedArgs[3])
		{
			matchCriteria = "Mean Square Error";
		}

		if(!providedArgs[4])
		{
			outputValue = "Magnitude-squared";
		}

		if(outputValue == "Magnitude-squared")
		{	
			magnitude = (double*)malloc(sizeof(double) * int(img1.rows) * int(img1.cols));
		}
		else
		{
			horizontal = (double*)malloc(sizeof(double) * int(img1.rows) * int(img1.cols));
			vertical = (double*)malloc(sizeof(double) * int(img1.rows) * int(img1.cols));
		}

		SurfFeatureDetector detector(1, 4, 2, 1, 1);
		SurfDescriptorExtractor extractor(1, 4, 2, 1, 1);

		//diving images into blocks and then finding best match for each strongest keypoint of that point
		itr = iRows = iCols = 0;
		for(int i=0;i<int(img1.rows);i+=int(blocksize[0])-int(overlap[0]))
		{
			if(i+int(blocksize[0])>=int(img1.rows))
				break;
			for(int j=0;j<int(img1.cols);j+=int(blocksize[1])-int(overlap[1]))
			{
				if(j+int(blocksize[1])>=int(img1.cols))
					break;

				//crop image as per the block size 
				Rect myROI(j, i, int(blocksize[0]), int(blocksize[1]));
				Mat croppedRef(img1, myROI);
				croppedRef.copyTo(cropped);

				//getting surf features of the cropped block
				keypoints.clear();
				detector.detect(cropped, keypoints);//detects feature points
				extractor.compute(cropped, keypoints, feature_des1);//descriptors

				matchesPerBlock.clear();
				//same for the 2nd image
				for(int k=-int(maximumDisplacement[0]);k<=int(maximumDisplacement[0]);k++)
				{
					if((i+k>=int(img2.rows)) || (i+k+int(blocksize[0])>=int(img2.rows)))
						break;
					if(i+k<0)
						continue;
					for(int l=-int(maximumDisplacement[1]);l<=int(maximumDisplacement[1]);l++)
					{
						if((j+l>=int(img2.cols)) || (j+l+int(blocksize[1])>=int(img2.cols)))
							break;
						if(j+l<0)
							continue;
						//crop the current block from the second image
						Rect myroi(j+l,i+k,int(blocksize[0]),int(blocksize[1]));
						Mat croppedref(img2, myroi);
						croppedref.copyTo(croppedBlock);

						//getting surf features of the cropped block from the second image
						keypointsSecondImage.clear();
						detector.detect(croppedBlock, keypointsSecondImage);
						extractor.compute(croppedBlock, keypointsSecondImage, feature_des2);
						matches.clear();
						//finding matches using the match criteria
						if(matchCriteria=="MSE")
						{
							BFMatcher matcher(NORM_L2, 1);
							matcher.match(feature_des1, feature_des2, matches);
						}
						else
						{
							BFMatcher matcher(NORM_L1, 1);
							matcher.match(feature_des1, feature_des2, matches);
						}
						matchesPerBlock.push_back(make_pair(matches, make_pair(i+k,j+l)));
					}
				}
				maxm = -1;
				for(int k=0;k<matchesPerBlock.size();k++)
				{
					maxm=max(maxm,int(matchesPerBlock[k].first.size()));
				}
				vector<pair<int, int > > v;
				for(int k=0;k<matchesPerBlock.size();k++)
				{
					if(matchesPerBlock[k].first.size()==maxm)
						v.push_back(matchesPerBlock[k].second);
				}	
				sort(v.begin(),v.end());
				if(!v.empty())
					displacementVector[i][j] = v[0];
				else
					displacementVector[i][j] = make_pair(i,j);
				v.clear();
				if(outputValue == "Magnitude-squared")
				{
					double dist = ((i-displacementVector[i][j].first)*(i-displacementVector[i][j].first) + (j-displacementVector[i][j].second) * (j-displacementVector[i][j].second));
					magnitude[itr++] = dist; 
				}
				else
				{
					horizontal[itr] = displacementVector[i][j].first-i;
					vertical[itr] = displacementVector[i][j].second-j;
					itr++;
				}
				iCols++;
			}
			iRows++;
		}
		
		if(outputValue == "Magnitude-squared")
		{
			//Creating list to store magnitude matrix in it
			sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, &piAddr);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

			sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 1, iRows, (iCols/iRows), magnitude);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
		}
		else
		{
			//Creating list to store horizontal and vertical matrix in it
			sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 2, &piAddr);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

			sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 1, iRows, (iCols/iRows), horizontal);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

			sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 2, iRows, (iCols/iRows), vertical);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
		}

		//return output Arguments
		AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
		ReturnArguments(pvApiCtx);
		return 0;
	}
	/* ==================================================================== */
}
