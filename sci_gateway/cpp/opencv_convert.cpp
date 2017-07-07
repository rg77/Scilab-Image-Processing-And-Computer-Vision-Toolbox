/***************************************************
Author : Rohan Gurve
****************************************************
   dst = convert(src,rtype,alpha,beta)
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
  #include <sciprint.h>
  #include "../common.h"
  
  int opencv_convert(char *fname, unsigned long fname_len)
  {

    SciErr sciErr;
    int intErr=0;
    int iRows=0,iCols=0;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piLen = NULL;
    char **rtype = NULL;
    double alpha,beta;
    int i;
    int type;
    
    //checking input argument
    CheckInputArgument(pvApiCtx, 4, 4);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat src;
    retrieveImage(src,1);

    
    //for type
    sciErr = getVarAddressFromPosition(pvApiCtx,2, &piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    //second call to retrieve length of each string
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    rtype = (char**)malloc(sizeof(char*) * iRows * iCols);
    for(i = 0 ; i < iRows * iCols ; i++)
    {
        rtype[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
    }
    //third call to retrieve data
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen, rtype);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    
    
    
    
    //for value of alpha
    sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddr3);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr3, &alpha);
    if(intErr)
        return intErr;

    
    //for value of beta
    sciErr = getVarAddressFromPosition(pvApiCtx,4,&piAddr4);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    
    intErr = getScalarDouble(pvApiCtx, piAddr4 ,&beta);
    if(intErr)
        return intErr;

    
   
    
    if(strcmp(rtype[0], "CV_8U") == 0)
        type = CV_8U;
    else if(strcmp(rtype[0], "CV_8S") == 0)
        type = CV_8S;
    else if(strcmp(rtype[0], "CV_16U") == 0)
        type = CV_16U;
    else if(strcmp(rtype[0], "CV_16S") == 0)
        type = CV_16S;
    else if(strcmp(rtype[0], "CV_32S") == 0)
        type = CV_32S;
    else if(strcmp(rtype[0], "CV_64F") == 0)
        type = CV_64F;
    
    else
      {
      Scierror(999, "%s: Invalid image type\n");
      return -1;
      }   

    
     try
     {
         src.convertTo(src,type,alpha,beta);
     }
    catch(Exception& e)
    {
        const char* err = e.what();
        Scierror(999, "%s",err);
        return -1;
          
    }

    
    string tempstring = type2str(src.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,src,1);
    free(checker);

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}
