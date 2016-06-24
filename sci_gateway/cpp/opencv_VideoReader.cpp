/***************************************************
Author : Tanmay Chaudhari, Shashank Shekhar
**************************************************/

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
    #include "../common.h"
    //#include "../common.cpp"

	Ptr<VideoCapture> capture;
	double videoCurrentTime = 0;
	double videoDuration = 0;
	int videoFPS;
	char *videoFileName = NULL;
	bool videoObjectSt = false;
	int opencv_VideoReader(char *fname, unsigned long fname_len)
	{
	    
	    SciErr sciErr;
	    int *piAddr = NULL;
	    int *piAddrVal = NULL;
	    int *piLen = NULL;
	    int iRows, iCols = 0;
	    char ** pstData1 = NULL;
	    char ** pstData2 = NULL;
	    double frames;
	    double width;
	    double height;
	    double CT;
	    char full_path[PATH_MAX];
	    bool stillLeft = false;
	    CheckInputArgument(pvApiCtx,1,3);
	    CheckOutputArgument(pvApiCtx,1,10);	
	    int noOfarguments = *getNbInputArgument(pvApiCtx);
	    if(noOfarguments==2)
	    {
	        Scierror(999,"Invalid number of Arguments\n");
	        return 0;
	    }

		sciErr = getVarAddressFromPosition(pvApiCtx,1,&piAddr);
	    if (sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	        return 0;
	    }
	    if(!isStringType(pvApiCtx, piAddr))
	    {
	        Scierror(999,"Invalid Argument\n");
	        return 0;
	    }
	    //first call to get rows and columns 
	    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL);
	    if(sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	        return 0;
	    }
	    piLen = (int*)malloc(sizeof(int) * iRows * iCols);  
		//second call to retrieve length of each string
	    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, NULL);
	    if(sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	        return 0;
	    }
	    pstData1 = (char**)malloc(sizeof(char*) * iRows * iCols);
	    for(int j = 0 ; j < iRows * iCols ; j++)
	    {
	        pstData1[j] = (char*)malloc(sizeof(char) * (piLen[j] + 1));//+ 1 for null termination
	    }
		//third call to retrieve data
	    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, pstData1);
	    if(sciErr.iErr)
	    {
	        printError(&sciErr, 0);
	        return 0;
	    }   

	    if(noOfarguments>1)
	    {
			sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr);
		    if (sciErr.iErr)
		    {
		        printError(&sciErr, 0);
		        return 0;
		    }
		    if(!isStringType(pvApiCtx, piAddr))
		    {
		        Scierror(999,"Invalid Argument\n");
		        return 0;
		    }
		    //first call to get rows and columns 
		    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL);
		    if(sciErr.iErr)
		    {
		        printError(&sciErr, 0);
		        return 0;
		    }
		    piLen = (int*)malloc(sizeof(int) * iRows * iCols);  
			//second call to retrieve length of each string
		    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, NULL);
		    if(sciErr.iErr)
		    {
		        printError(&sciErr, 0);
		        return 0;
		    }
		    pstData2 = (char**)malloc(sizeof(char*) * iRows * iCols);
		    for(int j = 0 ; j < iRows * iCols ; j++)
		    {
		        pstData2[j] = (char*)malloc(sizeof(char) * (piLen[j] + 1));//+ 1 for null termination
		    }
			//third call to retrieve data
		    sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, pstData2);
		    if(sciErr.iErr)
		    {
		        printError(&sciErr, 0);
		        return 0;
		    }   

		    if(strcmp(pstData2[0], "CurrentTime")==0)
		    {
		    	sciErr = getVarAddressFromPosition(pvApiCtx,3,&piAddrVal);
	            if (sciErr.iErr)
	            {
	                printError(&sciErr, 0);
	                return 0;
	            }

		        if(!isDoubleType(pvApiCtx, piAddrVal) || isVarComplex(pvApiCtx,piAddrVal) || !isScalar(pvApiCtx, piAddrVal))
	            {
		
	                Scierror(999," Invalid Value for Current Time.\n");
	                return 0;
		        }       
	 	        getScalarDouble(pvApiCtx, piAddrVal, &CT);
	 	        if(CT<0)
	 	        {
			        Scierror(999,"Invalid Input for CurrentTime.\n");
			        return 0;
	 	        }
	 	        CT*=1000;
		    }
		    else
		    {
		        Scierror(999,"Invalid Argument\n");
		        return 0;	    	
		    }
		}

	    if(videoObjectSt==true && strcmp(videoFileName, pstData1[0])==0)
	    { 
	    	if(int(videoCurrentTime) >= int(videoDuration))
	    	{
	 			stillLeft = false;
	    	}
	 		else if(int(videoCurrentTime) < int(videoDuration))
	 			stillLeft = true;
	    }

	    if(!stillLeft)
	   	{	
	   		capture = new VideoCapture(pstData1[0]);
			if(!capture->isOpened())
		   	{
			    Scierror(999,"Could not open file\n");
		        return 0;
		   	}
		}

		videoFileName = pstData1[0];
	   	videoFPS = capture->get(CV_CAP_PROP_FPS);
	    frames = capture->get(CV_CAP_PROP_FRAME_COUNT);
	    
	    if(noOfarguments!=1)
	    {
	    	if(int(CT) >= int(videoDuration))
	    	{
			    Scierror(999,"Current Time should be less than Video Duration\n");
		        return 0;	    
        	}
        	capture->set(CV_CAP_PROP_POS_MSEC, CT);
	    }

	    videoObjectSt = true;
	    videoCurrentTime = capture->get(CV_CAP_PROP_POS_MSEC);
	    videoDuration = (1000/double(videoFPS))*double(frames);
	    width = capture->get(CV_CAP_PROP_FRAME_WIDTH);
	    height = capture->get(CV_CAP_PROP_FRAME_HEIGHT);
    	realpath(pstData1[0], full_path);

	    createSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, pstData1[0]);
	    createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx) + 2, videoDuration);
	    createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx) + 3, videoFPS);
	    createScalarDouble(pvApiCtx, nbInputArgument(pvApiCtx)+4, videoCurrentTime);
	    createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx) + 5, frames);
	    createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx) + 6, width);
	    createScalarDouble(pvApiCtx,nbInputArgument(pvApiCtx) + 7, height);
	    createSingleString(pvApiCtx, nbInputArgument(pvApiCtx)+8, full_path);

	    for(int i=0;i<=8;i++)
	    {
 			AssignOutputVariable(pvApiCtx, i) = nbInputArgument(pvApiCtx) + i;
 		}
    
	    ReturnArguments(pvApiCtx);
	    return 0;
	}

	int has_frame(char *fname, unsigned long fname_len)
	{
	    SciErr sciErr;

		if(videoFileName==NULL)
		{
	        Scierror(999,"No video has been read yet.\n");
	        return 0;
		}

	    int *piAddr = NULL;
	    int *piLen = NULL;
	    int iRows, iCols = 0;
	    char ** pstData = NULL;

	    CheckInputArgument(pvApiCtx,0,0);
	    CheckOutputArgument(pvApiCtx,0,1);

	    if(videoObjectSt == false)
	    {
	    	Scierror(999,"Invalid Video File. You need to read the video using VideoReader.\n");
	        return 0;
	    }
	    
		if(videoCurrentTime>videoDuration)
			createScalarBoolean(pvApiCtx,nbInputArgument(pvApiCtx) + 1, false);
		else if(int(videoCurrentTime) == int(videoDuration))
			createScalarBoolean(pvApiCtx,nbInputArgument(pvApiCtx) + 1, false);
		else
			createScalarBoolean(pvApiCtx,nbInputArgument(pvApiCtx) + 1, true);

		AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
		ReturnArguments(pvApiCtx);
		
		return 0;
	}

	int read_frame(char *fname, unsigned long fname_len)
	{
		SciErr sciErr;

		if(videoFileName==NULL)
		{
	        Scierror(999,"No video has been read yet.\n");
	        return 0;
		}

		if(int(videoCurrentTime)>=int(videoDuration))
		{
	    	Scierror(999,"No more frames left to be read\n");
	        return 0;
		}
		//sciprint("Current Time of the Video is : %f\n", videoCurrentTime);
	    
	    int *piAddr = NULL;
	    int *piLen = NULL;
	    int iRows, iCols = 0;
	    char ** pstData = NULL;
	    int *singleFrameList = NULL;
 	    unsigned char *red = NULL;
	    unsigned char *green = NULL;
	    unsigned char *blue = NULL;	

	    CheckInputArgument(pvApiCtx,0,0);
	    CheckOutputArgument(pvApiCtx,0,1);

	    if(videoObjectSt == false)
	    {
	    	Scierror(999,"Invalid Video File. You need to read the video using VideoReader.\n");
	        return 0;
	    }

//--------------------------------------- Processing Frame at the particular frame -----------------------------------

	    Mat frame;
	    capture->set(CV_CAP_PROP_POS_MSEC, videoCurrentTime);
		if(!capture->read(frame))
		{
			Scierror(999, "Can't read Frame");
			return 0;	
		}
//--------------------------------------------------------------------------------------------------------------------

	    if(frame.channels()==1)
	    {

	 		sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, &singleFrameList);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

			red = (unsigned char *)malloc(sizeof(unsigned char)*frame.rows*frame.cols);

	    	for(int k=0;k<frame.rows;k++)
                for(int p=0;p<frame.cols;p++)
                    red[k+frame.rows*p]=frame.at<uchar>(k, p);

            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, singleFrameList, 1, frame.rows, frame.cols, red);
	    	if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}		                
	
			free(red);
		}
		else
		{

	    	sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 3, &singleFrameList);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

			red = (unsigned char *)malloc(sizeof(unsigned char)*frame.rows*frame.cols);
            green = (unsigned char *)malloc(sizeof(unsigned char)*frame.rows*frame.cols);
            blue = (unsigned char *)malloc(sizeof(unsigned char)*frame.rows*frame.cols);

		    for(int k=0;k<frame.rows;k++)
		    {
                for(int p=0;p<frame.cols;p++)
                {
                    Vec3b intensity = frame.at<Vec3b>(k, p);
                    red[k+frame.rows*p]=intensity.val[2];
                    green[k+frame.rows*p]=intensity.val[1];
                    blue[k+frame.rows*p]=intensity.val[0];
                }
            }

            sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, singleFrameList, 1, frame.rows, frame.cols, red);
	    	if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}
    		sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, singleFrameList, 2, frame.rows, frame.cols, green);
	    	if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}	    			
			sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, singleFrameList, 3, frame.rows, frame.cols, blue);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

					free(red);
					free(green);
					free(blue);
		}

		videoCurrentTime+=(1000/double(videoFPS));
		AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;	

	    ReturnArguments(pvApiCtx);
		
		return 0;
	}
}



