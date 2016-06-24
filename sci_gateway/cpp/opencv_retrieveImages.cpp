/********************************************************************************************************************************
* Author: Umang Agrawal                                                                                                         *
* Code: reteieveImage.cpp                                                                                                       *
* Funtion Call:    [ Index, Score, ImageWord ] = retrieveImage( QureyImage, IndexImage, OptionalArgument)                       *
*         Optional Argument:          Name                      Value                                                           *
*                                   NumResults           Any Value greater than 0                                               *
*                                     ROI       [ x_coordinate y_coordinate width height ]                                      *
********************************************************************************************************************************/
#include <iostream>
#include <numeric>
#include <vector>
#include <string>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/ml/ml.hpp"

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

  bool count_based(const vector<int>& a, const vector<int>& b) {
      return a[1] > b[1];
  }

	int opencv_retrieveImages(char *fname, unsigned long fname_len)
  {
    SciErr sciErr;

    int *piAddr = NULL;
    int *piAddr2 = NULL;
    int *piAddr3 = NULL;
    int *piAddr4 = NULL;
    int *piChild = NULL;
    int *piGrandChild = NULL;
    int *piLen = NULL;
    char **pstData = NULL;
    int iRows, iCols;
    int no_item_list = 0;
    int no_item_list_2 = 0;
    int inp_params = 0;
    char **arg = NULL;

    char ***location_img = NULL;
    double **WordIndex = NULL;
    double **Location = NULL;
    double **Vocab_size = NULL;
    double **Count = NULL;
    double *wordFrequency = NULL;
    char *bagOfFeaturesLocation = NULL;
    int count_res = 0, count_roi = 0;
    double numRes = 0;
    double *roi_arg = NULL;
    double x, y, width, height;

    Mat image;
    Mat feature_des;
    Mat dictionary;
    Mat des_matched;
    Mat hist;
    vector<KeyPoint> keypoints;
    vector<KeyPoint> valid_key;
    vector< vector<int> > clusterID;
    vector<int> WordIndex_size;

    int indx;
    int search_ind;
    double *Index_Out = NULL;
    double *Score = NULL;
    double **ImageWords = NULL;

    CheckInputArgument(pvApiCtx, 2, 6);						//Check on Number of Input Arguments
		CheckOutputArgument(pvApiCtx, 1, 3);						//Check on Number of Output Arguments

    retrieveImage(image, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    if(!isListType(pvApiCtx, piAddr2))
    {
        Scierror(999, "Error: Invalid Second argument. List Expected.\n");
        return 0;
    }
    sciErr = getListItemNumber(pvApiCtx, piAddr2, &no_item_list);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
    sciErr = getMatrixOfStringInList(pvApiCtx, piAddr2, 1, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    sciErr = getMatrixOfStringInList(pvApiCtx, piAddr2, 1, &iRows, &iCols, piLen, NULL);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(int iter = 0 ; iter < iRows * iCols ; iter++)
    {
      pstData[iter] = (char*)malloc(sizeof(char) * (piLen[iter] + 1));//+ 1 for null termination
    }
    sciErr = getMatrixOfStringInList(pvApiCtx, piAddr2, 1, &iRows, &iCols, piLen, pstData);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    if(!(strcmp(pstData[0],"invertedImageIndex")==0))
    {
        Scierror(999, "Error: The input argument 2 is not of type invertedImageIndex.\n");
        return 0;
    }

    sciErr = getListItemAddress(pvApiCtx, piAddr2, 2, &piChild);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    sciErr = getListItemNumber(pvApiCtx, piChild, &no_item_list_2);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}
    WordIndex = (double**)malloc(sizeof(double*)*no_item_list_2);
    Location = (double**)malloc(sizeof(double*)*no_item_list_2);
    Vocab_size = (double**)malloc(sizeof(double*)*no_item_list_2);
    Count = (double**)malloc(sizeof(double*)*no_item_list_2);
    for( int i=0; i<no_item_list_2; i++)
    {
      sciErr = getListItemAddress(pvApiCtx, piChild, i+1, &piGrandChild);
      if(sciErr.iErr)
      {
        printError(&sciErr, 0);
        return 0;
      }
      sciErr = getMatrixOfDoubleInList(pvApiCtx, piGrandChild, 1, &iRows, &iCols, &WordIndex[i]);
      if(sciErr.iErr)
      {
        printError(&sciErr, 0);
        return 0;
      }
      WordIndex_size.push_back(iRows);
      sciErr = getMatrixOfDoubleInList(pvApiCtx, piGrandChild, 2, &iRows, &iCols, &Location[i]);
      if(sciErr.iErr)
      {
        printError(&sciErr, 0);
        return 0;
      }
      sciErr = getMatrixOfDoubleInList(pvApiCtx, piGrandChild, 3, &iRows, &iCols, &Vocab_size[i]);
      if(sciErr.iErr)
      {
        printError(&sciErr, 0);
        return 0;
      }
      sciErr = getMatrixOfDoubleInList(pvApiCtx, piGrandChild, 4, &iRows, &iCols, &Count[i]);
      if(sciErr.iErr)
      {
        printError(&sciErr, 0);
        return 0;
      }
    }
    sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr2, 3, &iRows, &iCols, &wordFrequency);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    sciErr = getMatrixOfStringInList(pvApiCtx, piAddr2, 4, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    sciErr = getMatrixOfStringInList(pvApiCtx, piAddr2, 4, &iRows, &iCols, piLen, NULL);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(int iter = 0 ; iter < iRows * iCols ; iter++)
    {
      pstData[iter] = (char*)malloc(sizeof(char) * (piLen[iter] + 1));//+ 1 for null termination
    }
    sciErr = getMatrixOfStringInList(pvApiCtx, piAddr2, 4, &iRows, &iCols, piLen, pstData);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    bagOfFeaturesLocation = pstData[0];
    if(no_item_list == 5)
    {
      location_img = (char***)malloc(sizeof(char**)*1);
      sciErr = getListItemAddress(pvApiCtx, piAddr2, 5, &piChild);
      if(sciErr.iErr)
      {
        printError(&sciErr, 0);
        return 0;
      }
      sciErr = getMatrixOfStringInList(pvApiCtx, piChild, 1, &iRows, &iCols, NULL, NULL);
      if(sciErr.iErr)
      {
        printError(&sciErr, 0);
        return 0;
      }
      piLen = (int*)malloc(sizeof(int) * iRows * iCols);
      sciErr = getMatrixOfStringInList(pvApiCtx, piChild, 1, &iRows, &iCols, piLen, NULL);
      if(sciErr.iErr)
      {
        printError(&sciErr, 0);
        return 0;
      }
      location_img[0] = (char**)malloc(sizeof(char*) * iRows * iCols);
      for(int colIter = 0 ; colIter < iRows * iCols ; colIter++)
      {
        location_img[0][colIter] = (char*)malloc(sizeof(char) * (piLen[colIter] + 1));//+ 1 for null termination
      }
      sciErr = getMatrixOfStringInList(pvApiCtx, piChild, 1, &iRows, &iCols, piLen, location_img[0]);
      if(sciErr.iErr)
      {
        printError(&sciErr, 0);
        return 0;
      }
    }

    inp_params = *getNbInputArgument(pvApiCtx);
    for( int i=3; i<=inp_params; i++)
    {
      if( inp_params%2 != 0)
      {
        Scierror(999,"Either Argument Name or its Value missing\n");
        return 0;
      }
      sciErr = getVarAddressFromPosition(pvApiCtx, i, &piAddr3);
      if(sciErr.iErr)
      {
        printError(&sciErr, 0);
        return 0;
      }
      //Check for Argument type
      if( !isStringType(pvApiCtx, piAddr3))
      {
        Scierror(999, "%s: Wrong type of argument for Name of Optional Argument. A string is expected.\n", fname);
        return 0;
      }
      //Matrix of Stings
      sciErr = getMatrixOfString(pvApiCtx, piAddr3, &iRows, &iCols, NULL, NULL);
      if(sciErr.iErr)
      {
        printError(&sciErr, 0);
        return 0;
      }
      piLen = (int*)malloc(sizeof(int) * iRows * iCols);
      //second call to retrieve the length of the string
      sciErr = getMatrixOfString(pvApiCtx, piAddr3, &iRows, &iCols, piLen, NULL);
      if(sciErr.iErr)
      {
        printError(&sciErr, 0);
        free(piLen);
        return 0;
      }
      arg = (char**)malloc(sizeof(char*) * iRows * iCols);
      for(int j=0;j< iRows * iCols; j++)
      {
        arg[j] = (char*)malloc(sizeof(char) * (piLen[j] + 1));
      }
      //third call to retrieve data
      sciErr = getMatrixOfString(pvApiCtx, piAddr3, &iRows, &iCols, piLen, arg);
      if(sciErr.iErr)
      {
        printError(&sciErr, 0);
        free(piLen);
        free(arg);
        return 0;
      }

      if(strcmp(arg[0],"NumResuts") == 0)
      {
        if(count_res != 0)
        {
          Scierror(999,"NumResults Argument has been called twice.\n");
          return 0;
        }
        free(arg);
        free(piLen);
        sciErr = getVarAddressFromPosition(pvApiCtx, i+1, &piAddr4);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}
				//Checking the type of value
				if(!(isDoubleType(pvApiCtx, piAddr4)||isIntegerType(pvApiCtx, piAddr4)))
				{
					Scierror(999,"Not a valid type of value for NumResults.\n");
          return 0;
				}
        if(getScalarDouble(pvApiCtx, piAddr4, &numRes))
        {
          Scierror(999,"Error Reading value of NumResults Argument\n");
          return 0;
        }
        if(numRes < 0)
        {
          Scierror(999,"Not a valid value for NumResults\n");
          return 0;
        }
        i++;
        count_res += 1;
      }
      else if(strcmp(arg[0],"ROI") == 0)
      {
        if(count_roi != 0)
        {
          Scierror(999,"ROI Argument has been called twice.\n");
          return 0;
        }
        free(arg);
        free(piLen);
        sciErr = getVarAddressFromPosition(pvApiCtx, i+1, &piAddr4);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}
				//Checking the type of argument
				if(!(isDoubleType(pvApiCtx, piAddr4)||isIntegerType(pvApiCtx, piAddr4)))
				{
					Scierror(999,"Not a valid type of value.\n");
          return 0;
				}
				//Reading the Value of the argument
				sciErr = getMatrixOfDouble(pvApiCtx, piAddr4, &iRows, &iCols, &roi_arg);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}
				//Assigning the elements of the list to their proper function
				x = roi_arg[0];
				y = roi_arg[1];
				width = roi_arg[2];
				height = roi_arg[3];
        i++;
        count_roi += 1;
      }
      else
      {
        Scierror(999,"Invalid Argument Name\n");
        return 0;
      }
    }

    if(count_roi != 0)
    {
      Rect masker(x, y, width, height);
      Mat croppedRef(image, masker);
      image = Mat();
      croppedRef.copyTo(image);
    }

    FileStorage fs(bagOfFeaturesLocation, FileStorage::READ);
    fs["dictionary"] >> dictionary;
    fs.release();

    SurfFeatureDetector detector(100, 4, 2, 1, 1);
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("FlannBased");
    Ptr<DescriptorExtractor> extractor = new SurfDescriptorExtractor(100, 4, 2, 1, 1);
    BOWImgDescriptorExtractor bowDE(extractor, matcher);
    bowDE.setVocabulary(dictionary);
    detector.detect(image, keypoints);
    bowDE.compute(image, keypoints, hist, &clusterID, &des_matched);

    ImageWords = (double**)malloc(sizeof(double*)*4*1);
    ImageWords[0] = (double*)malloc(sizeof(double)*keypoints.size()*1);
    ImageWords[1] = (double*)malloc(sizeof(double)*keypoints.size()*2);
    ImageWords[2] = (double*)malloc(sizeof(double)*1);
    ImageWords[3] = (double*)malloc(sizeof(double)*1);

    ImageWords[2][0] = Vocab_size[0][0];
    ImageWords[3][0] = keypoints.size();

    vector<vector<int> > count_index(no_item_list_2 , vector<int>(3));

    for( int i=0; i<keypoints.size(); i++)
    {
      ImageWords[1][i] = keypoints[i].pt.x;
      ImageWords[1][keypoints.size() + i] = keypoints[i].pt.y;
    }
    for( int i=0; i<clusterID.size(); i++)
    {
      for( int j=0; j<clusterID[i].size(); j++)
      {
          indx = clusterID[i][j];
          ImageWords[0][indx] = i;
      }
    }

    for( int i=0; i<no_item_list_2; i++)
    {
      int count = 0;
      for( int j=0; j<keypoints.size(); j++)
      {
        search_ind = ImageWords[0][j];
        for( int k=0; k<WordIndex_size[i]; k++)
        {
          if(int(WordIndex[i][k]) == int(search_ind))
          {
            count += 1;
            break;
          }
        }
      }
      count_index[i][0] = i+1;
      count_index[i][1] = count;
      count_index[i][2] = WordIndex_size[i];
    }

    sort(count_index.begin(), count_index.end(), count_based);
    if(count_res == 0 || no_item_list_2<numRes)
      numRes = no_item_list_2;
    Index_Out = (double*)malloc(sizeof(double)*numRes);
    Score = (double*)malloc(sizeof(double)*numRes);
    for( int i=0; i<numRes; i++)
    {
      Index_Out[i] = count_index[i][0];
      Score[i] = double(count_index[i][1])/double(keypoints.size());
    }

    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, numRes, 1, Index_Out);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2, numRes, 1, Score);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 3, 4, &piAddr);
    if(sciErr.iErr)
    {
      printError(&sciErr, 0);
      return 0;
    }
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 3, piAddr, 1, keypoints.size(), 1, ImageWords[0]);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 3, piAddr, 2, keypoints.size(), 2, ImageWords[1]);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 3, piAddr, 3, 1, 1, ImageWords[2]);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 3, piAddr, 4, 1, 1, ImageWords[3]);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx)+1;
    AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx)+2;
    AssignOutputVariable(pvApiCtx, 3) = nbInputArgument(pvApiCtx)+3;
    ReturnArguments(pvApiCtx);
    return 0;

  }

}
