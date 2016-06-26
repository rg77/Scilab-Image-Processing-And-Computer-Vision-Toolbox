/********************************************************
Author: Suraj Prakash
return_image = TemplateMatcher(InputArray image, InputArray templage_image, int method)
********************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

extern "C"{
  #include "api_scilab.h"
  #include "Scierror.h"
  #include "BOOL.h"
  #include <localization.h>
  #include <sciprint.h>
  #include "../common.h"
  
  int opencv_TemplateMatcher(char *fname, unsigned long fname_len){

    /// Error management variable
    SciErr sciErr;
    
    /// Variables
    int i, j;

    int iRows = 0;
    int iCols = 0;
    int *piLen = NULL;
    int *piAddr = NULL;

    char **method = NULL;


    /// checking input argument
    CheckInputArgument(pvApiCtx, 2, 3);
    CheckOutputArgument(pvApiCtx, 1, 1);

    Mat image, template_image;
    retrieveImage(image, 1);
    retrieveImage(template_image, 2);

    /// if there is inconsistency in data type of image and template image
    if(type2str(image.type()) != type2str(template_image.type())){
      Scierror(999, "%s: Image and template image are not of same type\n");
      return 0;
    }

    if(template_image.rows > image.rows or template_image.cols > image.cols){
      Scierror(999, "%s: Dimensions of template image is greater then image\n");
      return 0;
    }

    /// generated image is (W - w + 1) * (H - h + 1) and it must be 32 bit floating number with single channel

    Mat new_image(image.rows, image.cols, image.type()); 


     int nbInputArguments = *getNbInputArgument(pvApiCtx);
     if(nbInputArguments > 2){
     		/// variable address
		    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr);
		    if(sciErr.iErr){   
		        printError(&sciErr, 0);
		        return 0;
		    }
		    
		    /// Retrieve string from input parameter. (requires 3 calls)
		    /// first to retrieve dimensions
		    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL);
		    if(sciErr.iErr){
		        printError(&sciErr, 0);
		        return 0;
		    }
		    
		    piLen = (int*)malloc(sizeof(int) * iRows * iCols);

		    //second call to retrieve length of each string
		    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, NULL);
		    if(sciErr.iErr){
		        printError(&sciErr, 0);
		        return 0;
		    }

		    method = (char**)malloc(sizeof(char*) * iRows * iCols);
		    for(i = 0 ; i < iRows * iCols ; i++)
		        method[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination

		    //third call to retrieve data
		    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, method);
		    if(sciErr.iErr){
		        printError(&sciErr, 0);
		        return 0;
		    }

		     if(strcmp(method[0], "CV_TM_SQDIFF") == 0)
		      matchTemplate(image, template_image, new_image, CV_TM_SQDIFF);
		    else if(strcmp(method[0], "CV_TM_SQDIFF_NORMED") == 0)
		      matchTemplate(image, template_image, new_image, CV_TM_SQDIFF_NORMED);
		    else if(strcmp(method[0], "CV_TM_CCORR") == 0)
		      matchTemplate(image, template_image, new_image, CV_TM_CCORR);
		    else if(strcmp(method[0], "CV_TM_CCORR_NORMED") == 0)
		      matchTemplate(image, template_image, new_image, CV_TM_CCORR_NORMED);
		    else if(strcmp(method[0], "CV_TM_CCOEFF") == 0)
		      matchTemplate(image, template_image, new_image, CV_TM_CCOEFF);
		    else if(strcmp(method[0], "CV_TM_CCOEFF_NORMED") == 0)
		      matchTemplate(image, template_image, new_image, CV_TM_CCOEFF_NORMED);
		    else{
		      matchTemplate(image, template_image, new_image, CV_TM_SQDIFF);
		      sciprint("Wrong method given, using CV_TM_SQDIFF instead");  
    		}          
     }

     else{
     	 matchTemplate(image, template_image, new_image, CV_TM_SQDIFF);
     }
    

    


     
    
    string tempstring = type2str(new_image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker, new_image, 1);
    free(checker);


    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}
