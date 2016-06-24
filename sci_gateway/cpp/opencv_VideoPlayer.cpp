/***************************************************
Author : Tanmay Chaudhari
**************************************************/

#include "opencv2/video/tracking.hpp"
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
    #include "../common.h"
    //#include "../common.cpp"

    int opencv_VideoPlayer()
    {   
        SciErr sciErr;
        int* piAddr = NULL;
        int iRows = 0;
        int iCols = 0;
        int iRet = 0;
        char** pstData = NULL;

        CheckInputArgument(pvApiCtx, 1, 1);
        CheckOutputArgument(pvApiCtx, 1, 1);

        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }
        iRet = getAllocatedMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, &pstData);
        if(iRet)
        {
            freeAllocatedMatrixOfString(iRows, iCols, pstData);
            return iRet;
        }
        sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRows, iCols, pstData);
        if(sciErr.iErr)
        {
            freeAllocatedMatrixOfString(iRows, iCols, pstData);
            printError(&sciErr, 0);
            return sciErr.iErr;
        }
        VideoCapture cap(pstData[0]);
        namedWindow("Video Player", 0 );
        Mat frame;
        bool paused = false;
        for(;;)
        {
            if( !paused )
            {
                cap >> frame;
                if(frame.empty())
                    break;
            }
            imshow("Video Player", frame );
            char c = (char)waitKey(20);
            if(c==27)
                break;
            else if(c==32)
                paused=!paused;
            else;
        }
        destroyAllWindows();
        ReturnArguments(pvApiCtx); 
        return 0;
    }
}

