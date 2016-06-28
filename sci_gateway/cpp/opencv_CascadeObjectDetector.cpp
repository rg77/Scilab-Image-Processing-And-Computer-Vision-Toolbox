/********************************************************
Author: Diwakar Bhardwaj
*********************************************************
Usage :return_image=insertObjectAnnotation(input_image,'object')

where object can be mouth,nose,ear or upperbody etc.
********************************************************/

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
  
  int opencv_CascadeObjectDetector(char *fname, unsigned long fname_len)
  {
    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piLen = NULL;
    char **object = NULL;
    double *bboxes = NULL;
    int i,j,k=0,l=0;
    double alpha, beta,x,n;

    CascadeClassifier face_cascade;
    CascadeClassifier eyes_cascade;
    CascadeClassifier mouth_cascade;
    CascadeClassifier nose_cascade;
    CascadeClassifier upperbody_cascade;

    vector<Rect>faces,eyes,mouth,nose,upperbody;
    face_cascade.load("/home/diwakar/Desktop/opencv-2.4.9/data/haarcascades/haarcascade_frontalface_alt.xml");
    eyes_cascade.load("/home/diwakar/Desktop/opencv-2.4.9/data/haarcascades/haarcascade_mcs_eyepair_big.xml");
    mouth_cascade.load("/home/diwakar/Desktop/opencv-2.4.9/data/haarcascades/haarcascade_mcs_mouth.xml");
    nose_cascade.load("/home/diwakar/Desktop/opencv-2.4.9/data/haarcascades/haarcascade_mcs_nose.xml");
    upperbody_cascade.load("/home/diwakar/Desktop/opencv-2.4.9/data/haarcascades/haarcascade_mcs_upperbody.xml");

    float eye_x1,eye_y1,eye_x2,eye_y2,mouth_x1,mouth_y1,mouth_x2,mouth_y2,nose_x1,nose_y1,nose_x2,nose_y2;
    //checking input argument
    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1) ;
    Mat image,new_image,image_gray;
    
    retrieveImage(image, 1);

    sciErr = getVarAddressFromPosition(pvApiCtx,2, &piAddr2);

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, NULL, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    piLen = (int*)malloc(sizeof(int) * iRows * iCols);
    //second call to retrieve length of each string
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen, NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    object= (char**)malloc(sizeof(char*) * iRows * iCols);
    for(i = 0 ; i < iRows * iCols ; i++)
    {
        object[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
    }
    //third call to retrieve data
    sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen,object);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    int m;
    double *bboxes = NULL;
    bboxes = (double*)malloc(sizeof(double) * (int)answer.size() * 4);
    int tbox=0;
    //Converting image from rgb to gray scale image.
    cvtColor( image, image_gray, CV_BGR2GRAY );
    //equalizing the histrogram of gray scale image
    equalizeHist( image_gray, image_gray );
       
    face_cascade.detectMultiScale( image, faces, 1.2,2,CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_CANNY_PRUNING, cvSize(0,0), cvSize(255,255) );
    if(strcmp(object[0],"face")==0)
      {
    for( int i = 0; i < faces.size(); i++ )
       {    
<<<<<<< HEAD
       	   tbox = (int)faces.Size();
       	   bboxes[i + 0 * (int)faces.size()] = faces[i].x;
       	   bboxes[i + 1 * (int)faces.size()] = faces[i].y;
       	   bboxes[i + 2 * (int)faces.size()] = faces[i].width;
       	   bboxes[i + 3 * (int)faces.size()] = faces[i].height;
=======
>>>>>>> c6d5c3d8dbf4db6da6d7ac081918599b6977960a
               
           Point point1( faces[i].x + faces[i].width, faces[i].y + faces[i].height );
           Point point2( faces[i].x, faces[i].y);
           rectangle(image,point1,point2,cvScalar(0,255,0),1,16,0);
<<<<<<< HEAD
           Mat faceROI = image_gray( faces[i] );

           eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

           for( int j = 0; j < eyes.size(); j++ )
              {
                  Point point3( faces[i].x + eyes[j].x + eyes[j].width, faces[i].y + eyes[j].y + eyes[j].height );
                  Point point4(faces[i].x + eyes[j].x, faces[i].y + eyes[j].y );
                  
                  //rectangle(image,point3,point4,cvScalar(0,0,255),1,8,0);
               }
            eye_x1=faces[0].x + eyes[0].x + eyes[0].width;
            eye_y1=faces[0].y + eyes[0].y + eyes[0].height;
            eye_x2=faces[0].x + eyes[0].x;
            eye_y2=faces[0].y + eyes[0].y;

            nose_cascade.detectMultiScale( faceROI, nose, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
            for( int j = 0; j < nose.size(); j++ )
               {
                    if((faces[i].x +nose[j].x + nose[j].width)<eye_x1 && (faces[i].y + nose[j].y + nose[j].height)>eye_y1)
                    if((faces[i].x +nose[j].x)>eye_x2)     
                       {
                          Point point7(faces[i].x +nose[j].x + nose[j].width,faces[i].y + nose[j].y + nose[j].height );
                          Point point8(faces[i].x +nose[j].x,faces[i].y+nose[j].y );
                          
                          //rectangle(image,point7,point8,cvScalar(255,255,0),1,8,0);
                          m=j;
                  
                        } 
                }

            nose_x1=faces[0].x + nose[m].x + nose[m].width;
            nose_y1=faces[0].y + nose[m].y + nose[m].height;
            nose_x2=faces[0].x + nose[m].x;
            nose_y2=faces[0].y + nose[m].y;
            
            mouth_cascade.detectMultiScale( faceROI, mouth, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
            for( int j = 0; j < mouth.size(); j++ )
               {
     
                  if((faces[i].x +mouth[j].x + mouth[j].width)<eye_x1 && (faces[i].x +mouth[j].x)>eye_x2)
                    {
                      if((faces[i].y + mouth[j].y + mouth[j].height)>(faces[i].y+nose[m].y +nose[m].height))
                        {
                          Point point5(faces[i].x +mouth[j].x + mouth[j].width,faces[i].y + mouth[j].y + mouth[j].height );
                          Point point6(faces[i].x +mouth[j].x,faces[i].y + mouth[j].y );
                         
                          //rectangle(image,point5,point6,cvScalar(0,255,0),1,8,0);
                          m=j;
                        }
                    }
                }
            mouth_x1=faces[0].x + mouth[m].x + mouth[m].width;
            mouth_y1=faces[0].y + mouth[m].y + mouth[m].height;
            mouth_x2=faces[0].x + mouth[m].x;
            mouth_y2=faces[0].y + mouth[m].y;
=======
>>>>>>> c6d5c3d8dbf4db6da6d7ac081918599b6977960a
      
       }
      }
      else if(strcmp(object[0],"eyes")==0)
      {
    for( int i = 0; i < faces.size(); i++ )
       {
           Point point1( faces[i].x + faces[i].width, faces[i].y + faces[i].height );
           Point point2( faces[i].x, faces[i].y);
           //rectangle(image,point1,point2,cvScalar(0,255,0),1,16,0);
           Mat faceROI = image_gray( faces[i] );

           eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

           for( int j = 0; j < eyes.size(); j++ )
              {
                  Point point3( faces[i].x + eyes[j].x + eyes[j].width, faces[i].y + eyes[j].y + eyes[j].height );
                  Point point4(faces[i].x + eyes[j].x, faces[i].y + eyes[j].y );
                  
                  rectangle(image,point3,point4,cvScalar(0,0,255),1,8,0);
               }
            eye_x1=faces[0].x + eyes[0].x + eyes[0].width;
            eye_y1=faces[0].y + eyes[0].y + eyes[0].height;
            eye_x2=faces[0].x + eyes[0].x;
            eye_y2=faces[0].y + eyes[0].y;

            nose_cascade.detectMultiScale( faceROI, nose, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
            for( int j = 0; j < nose.size(); j++ )
               {
                    if((faces[i].x +nose[j].x + nose[j].width)<eye_x1 && (faces[i].y + nose[j].y + nose[j].height)>eye_y1)
                    if((faces[i].x +nose[j].x)>eye_x2)     
                       {
                          Point point7(faces[i].x +nose[j].x + nose[j].width,faces[i].y + nose[j].y + nose[j].height );
                          Point point8(faces[i].x +nose[j].x,faces[i].y+nose[j].y );
                          
                          //rectangle(image,point7,point8,cvScalar(255,255,0),1,8,0);
                          m=j;
                  
                        } 
                }

            nose_x1=faces[0].x + nose[m].x + nose[m].width;
            nose_y1=faces[0].y + nose[m].y + nose[m].height;
            nose_x2=faces[0].x + nose[m].x;
            nose_y2=faces[0].y + nose[m].y;
            
            mouth_cascade.detectMultiScale( faceROI, mouth, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
            for( int j = 0; j < mouth.size(); j++ )
               {
     
                  if((faces[i].x +mouth[j].x + mouth[j].width)<eye_x1 && (faces[i].x +mouth[j].x)>eye_x2)
                    {
                      if((faces[i].y + mouth[j].y + mouth[j].height)>(faces[i].y+nose[m].y +nose[m].height))
                        {
                          Point point5(faces[i].x +mouth[j].x + mouth[j].width,faces[i].y + mouth[j].y + mouth[j].height );
                          Point point6(faces[i].x +mouth[j].x,faces[i].y + mouth[j].y );
                         
                          //rectangle(image,point5,point6,cvScalar(0,255,0),1,8,0);
                          m=j;
                        }
                    }
                }
            mouth_x1=faces[0].x + mouth[m].x + mouth[m].width;
            mouth_y1=faces[0].y + mouth[m].y + mouth[m].height;
            mouth_x2=faces[0].x + mouth[m].x;
            mouth_y2=faces[0].y + mouth[m].y;
      
       }
      }
      else if(strcmp(object[0],"nose")==0)
      {
    for( int i = 0; i < faces.size(); i++ )
       {
           Point point1( faces[i].x + faces[i].width, faces[i].y + faces[i].height );
           Point point2( faces[i].x, faces[i].y);
           //rectangle(image,point1,point2,cvScalar(0,255,0),1,16,0);
           Mat faceROI = image_gray( faces[i] );

           eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

           for( int j = 0; j < eyes.size(); j++ )
              {
                  Point point3( faces[i].x + eyes[j].x + eyes[j].width, faces[i].y + eyes[j].y + eyes[j].height );
                  Point point4(faces[i].x + eyes[j].x, faces[i].y + eyes[j].y );
                  
                  //rectangle(image,point3,point4,cvScalar(0,0,255),1,8,0);
               }
            eye_x1=faces[0].x + eyes[0].x + eyes[0].width;
            eye_y1=faces[0].y + eyes[0].y + eyes[0].height;
            eye_x2=faces[0].x + eyes[0].x;
            eye_y2=faces[0].y + eyes[0].y;

            nose_cascade.detectMultiScale( faceROI, nose, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
            for( int j = 0; j < nose.size(); j++ )
               {
                    if((faces[i].x +nose[j].x + nose[j].width)<eye_x1 && (faces[i].y + nose[j].y + nose[j].height)>eye_y1)
                    if((faces[i].x +nose[j].x)>eye_x2)     
                       {
                          Point point7(faces[i].x +nose[j].x + nose[j].width,faces[i].y + nose[j].y + nose[j].height );
                          Point point8(faces[i].x +nose[j].x,faces[i].y+nose[j].y );
                          
                          rectangle(image,point7,point8,cvScalar(255,255,0),1,8,0);
                          m=j;
                  
                        } 
                }

            nose_x1=faces[0].x + nose[m].x + nose[m].width;
            nose_y1=faces[0].y + nose[m].y + nose[m].height;
            nose_x2=faces[0].x + nose[m].x;
            nose_y2=faces[0].y + nose[m].y;
            
            mouth_cascade.detectMultiScale( faceROI, mouth, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
            for( int j = 0; j < mouth.size(); j++ )
               {
     
                  if((faces[i].x +mouth[j].x + mouth[j].width)<eye_x1 && (faces[i].x +mouth[j].x)>eye_x2)
                    {
                      if((faces[i].y + mouth[j].y + mouth[j].height)>(faces[i].y+nose[m].y +nose[m].height))
                        {
                          Point point5(faces[i].x +mouth[j].x + mouth[j].width,faces[i].y + mouth[j].y + mouth[j].height );
                          Point point6(faces[i].x +mouth[j].x,faces[i].y + mouth[j].y );
                          //rectangle(image,point5,point6,cvScalar(0,255,0),1,8,0);
                          m=j;
                        }
                    }
                }
            mouth_x1=faces[0].x + mouth[m].x + mouth[m].width;
            mouth_y1=faces[0].y + mouth[m].y + mouth[m].height;
            mouth_x2=faces[0].x + mouth[m].x;
            mouth_y2=faces[0].y + mouth[m].y;
      
       }
      }
      else if(strcmp(object[0],"mouth")==0)
      {
    for( int i = 0; i < faces.size(); i++ )
       {
           Point point1( faces[i].x + faces[i].width, faces[i].y + faces[i].height );
           Point point2( faces[i].x, faces[i].y);
           //rectangle(image,point1,point2,cvScalar(0,255,0),1,16,0);
           Mat faceROI = image_gray( faces[i] );

           eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );

           for( int j = 0; j < eyes.size(); j++ )
              {
                  Point point3( faces[i].x + eyes[j].x + eyes[j].width, faces[i].y + eyes[j].y + eyes[j].height );
                  Point point4(faces[i].x + eyes[j].x, faces[i].y + eyes[j].y );
                  
                  //rectangle(image,point3,point4,cvScalar(0,0,255),1,8,0);
               }
            eye_x1=faces[0].x + eyes[0].x + eyes[0].width;
            eye_y1=faces[0].y + eyes[0].y + eyes[0].height;
            eye_x2=faces[0].x + eyes[0].x;
            eye_y2=faces[0].y + eyes[0].y;

            nose_cascade.detectMultiScale( faceROI, nose, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
            for( int j = 0; j < nose.size(); j++ )
               {
                    if((faces[i].x +nose[j].x + nose[j].width)<eye_x1 && (faces[i].y + nose[j].y + nose[j].height)>eye_y1)
                    if((faces[i].x +nose[j].x)>eye_x2)     
                       {
                          Point point7(faces[i].x +nose[j].x + nose[j].width,faces[i].y + nose[j].y + nose[j].height );
                          Point point8(faces[i].x +nose[j].x,faces[i].y+nose[j].y );
                          
                          //rectangle(image,point7,point8,cvScalar(255,255,0),1,8,0);
                          m=j;
                  
                        } 
                }

            nose_x1=faces[0].x + nose[m].x + nose[m].width;
            nose_y1=faces[0].y + nose[m].y + nose[m].height;
            nose_x2=faces[0].x + nose[m].x;
            nose_y2=faces[0].y + nose[m].y;
            mouth_cascade.detectMultiScale( faceROI, mouth, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
            for( int j = 0; j < mouth.size(); j++ )
               {
                  if((faces[i].x +mouth[j].x + mouth[j].width)<eye_x1 && (faces[i].x +mouth[j].x)>eye_x2)
                    {
                      if((faces[i].y + mouth[j].y + mouth[j].height)>(faces[i].y+nose[m].y +nose[m].height))
                        {
                          Point point5(faces[i].x +mouth[j].x + mouth[j].width,faces[i].y + mouth[j].y + mouth[j].height );
                          Point point6(faces[i].x +mouth[j].x,faces[i].y + mouth[j].y );
                          //if(strcmp(object[0],"mouth")==0)
                          rectangle(image,point5,point6,cvScalar(0,255,0),1,8,0);
                          m=j;
                        }
                    }
                }
            mouth_x1=faces[0].x + mouth[m].x + mouth[m].width;
            mouth_y1=faces[0].y + mouth[m].y + mouth[m].height;
            mouth_x2=faces[0].x + mouth[m].x;
            mouth_y2=faces[0].y + mouth[m].y;
      
       }
      }
     else if(strcmp(object[0],"upper_body")==0)
     {
        upperbody_cascade.detectMultiScale( image_gray, upperbody, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(255, 255) );
    
        for( int i = 0; i < upperbody.size(); i++ )
           {
        Rect t = upperbody[i];
        t.y += 100;
        rectangle(image, t, Scalar(255,255,255), 4, 8);
           }
     }
     else
    {
     sciprint("Wrong object type");

    }

    //new_image=image;
    
 

<<<<<<< HEAD
    /*int temp = nbInputArgument(pvApiCtx) + 1;
    string tempstring = type2str(new_image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,new_image,1);
    free(checker); */
    sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, tbox, 4, bboxes);
=======
    int temp = nbInputArgument(pvApiCtx) + 1;
    string tempstring = type2str(image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,image,1);
    free(checker); 
    /*sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, tbox, 4, bboxes);
>>>>>>> c6d5c3d8dbf4db6da6d7ac081918599b6977960a
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
