/********************************************************
Author: Suraj Prakash
[xyxrefinedPoints] = bundleAdjustment(3dpoints, imagepoints, visibility, cameramatrix, rotation, translation, distcoeffs);
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
  
  int opencv_bundleAdjustment(char *fname, unsigned long fname_len){

    /// Error management variable
    SciErr sciErr;
    
    /// Variables
    int i, j;

    int iRows = 0;
    int iCols = 0;
    int iRows1 = 0;
    int iCols1 = 0;
    int *piLen = NULL;
    int *piAddr = NULL;
    int *piAddr2 = NULL;
    int *piChild = NULL;
    int *piAddrNew = NULL;
    int iItem = 0;
    double *pdblReal = NULL;
 
    char **pstData = NULL;
    char *currentArg = NULL;
    bool *providedArgs = NULL; 

    double *cameraMatrix = NULL; 
    double *rotationMatrix = NULL;
    double *translationMatrix = NULL;
    double *newCameraMatrix = NULL;
    double *distCoeffs = NULL;

    /// checking input argument

    CheckInputArgument(pvApiCtx, 7, 7);
    CheckOutputArgument(pvApiCtx, 1, 1);

    // cv::LevMarqSparse::bundleAdjust (   vector< Point3d > &     points,
    //                                     const vector< vector< Point2d > > &     imagePoints,
    //                                     const vector< vector< input > > &     visibility,
    //                                     vector< Mat > &     cameraMatrix,
    //                                     vector< Mat > &     R,
    //                                     vector< Mat > &     T,
    //                                     vector< Mat > &     distCoeffs,
    //                                     const TermCriteria &    criteria = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, DBL_EPSILON),
    //                                     BundleAdjustCallback    cb = 0,
    //                                     void *  user_data = 0 
    //                                 )   

    ///////////          getting the 3d points
    vector< Point3d > points;
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    for(i=0; i<iRows; i++)
    {
        points.push_back(Point3d(pdblReal[i + 0 * iRows], pdblReal[i + 1 * iRows], pdblReal[i + 2 * iRows]));
    }
    
    /// getting image points M * N * 2

    double *pstDatax = NULL;
    double *pstDatay = NULL;

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }

    //retrive the matrix of the x coordinates   
    sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr, 1, &iRows, &iCols, &pstDatax);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    //retrive address of the list
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }   
    //retrive the matrix of the y coordinates 
    sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr, 2, &iRows, &iCols, &pstDatay);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    vector< vector< Point2d > > imagePoints;

    imagePoints.resize(iRows);

    for(i=0; i<iRows; ++i){
        for(j=0; j<iCols; ++j){
            imagePoints[i].push_back(Point2d(pstDatax[i + iRows * j], pstDatay[i + iRows * j]));
        }
    }


    /// getting the visibility matrix
    sciErr = getVarAddressFromPosition(pvApiCtx, 3  , &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    vector< vector < int> > visibility(iRows, vector< int >(iCols, 0));

    for(i=0; i<iRows; ++i){
        for(j=0; j<iCols; ++j)
            visibility[i][j] = pdblReal[i + j * iRows];
    }




    ///  M * 3 * 3 camera matrix
    vector< Mat > camera;
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }


    sciErr = getListItemNumber(pvApiCtx, piAddr, &iItem);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }

    for(int item = 0; item<iItem; ++item){
            sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            
            //retrive the matrix of the camera values   
            sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr, item + 1, &iRows, &iCols, &cameraMatrix);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            Mat temp(3, 3, CV_64F);

            for(i=0; i<3; ++i)
                for(j=0; j<3; ++j)
                    temp.at<double>(i, j) = cameraMatrix[i + j * 3];

            camera.push_back(temp);

    }

    ///  M * 3 * 3 rotation matrix
    vector< Mat > rotation;
    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }


    sciErr = getListItemNumber(pvApiCtx, piAddr, &iItem);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }

    for(int item = 0; item<iItem; ++item){
            sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            
            //retrive the matrix of the camera values   
            sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr, item + 1, &iRows, &iCols, &rotationMatrix);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            Mat temp(3, 3, CV_64F);

            for(i=0; i<iRows; ++i)
                for(j=0; j<iCols; ++j)
                    temp.at<double>(i, j) = rotationMatrix[i + j * iRows];

            rotation.push_back(temp);

    }


    ///  M * 3 * 1 translation matrix
    vector< Mat > translation;
    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }


    sciErr = getListItemNumber(pvApiCtx, piAddr, &iItem);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }

    for(int item = 0; item<iItem; ++item){
            sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            
            //retrive the matrix of the camera values   
            sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr, item + 1, &iRows, &iCols, &translationMatrix);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            Mat temp(3, 1, CV_64F);

            for(i=0; i<3; ++i)
                for(j=0; j<1; ++j)
                    temp.at<double>(i, j) = translationMatrix[i + j * 3];

            translation.push_back(temp);

    }


    ///  M * (4, 5, 8) * 1 distCoefficients matrix
    vector< Mat > distCoefficients;
    sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddr);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }


    sciErr = getListItemNumber(pvApiCtx, piAddr, &iItem);
    if(sciErr.iErr)   
    {
        printError(&sciErr, 0);
        return 0;
    }

    for(int item = 0; item<iItem; ++item){
            sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }

            
            //retrive the matrix of the camera values   
            sciErr = getMatrixOfDoubleInList(pvApiCtx, piAddr, item + 1, &iRows, &iCols, &distCoeffs);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return 0;
            }
            Mat temp(iRows, iCols, CV_64F);

            for(i=0; i<iRows; ++i)
                for(j=0; j<iCols; ++j)
                    temp.at<double>(i, j) = distCoeffs[i + j * iRows];

            distCoefficients.push_back(temp);

    }

    // cv::LevMarqSparse::bundleAdjust (   vector< Point3d > &     points,
    //                                     const vector< vector< Point2d > > &     imagePoints,
    //                                     const vector< vector< input > > &     visibility,
    //                                     vector< Mat > &     cameraMatrix,
    //                                     vector< Mat > &     R,
    //                                     vector< Mat > &     T,
    //                                     vector< Mat > &     distCoeffs,
    //                                     const TermCriteria &    criteria = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, DBL_EPSILON),
    //                                     BundleAdjustCallback    cb = 0,
    //                                     void *  user_data = 0 
    //                                 )   

    /// TermCriteria
    
    TermCriteria criteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 30, DBL_EPSILON);

    LevMarqSparse   lms;
    lms.bundleAdjust(points, imagePoints, visibility, camera, rotation, translation, distCoefficients, criteria);


    double *ans = NULL;

    ans = (double*)malloc(sizeof(double) * points.size() * 3);

    for(i = 0; i < points.size(); ++i){
        ans[i + 0 * points.size()] = points[i].x;
        ans[i + 1 * points.size()] = points[i].y;
        ans[i + 2 * points.size()] = points[i].z;
    }


    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, points.size(), 3 , ans);
    if(sciErr.iErr){
      printError(&sciErr, 0);
      return 0;
    }
    


    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}
