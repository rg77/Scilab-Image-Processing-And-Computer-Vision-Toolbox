/********************************************************
Author: Deepshikha
return_image = Deinterlacer(source_image)
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
  
  int opencv_Deinterlacer(char *fname, unsigned long fname_len){

    /// Error management variable
    SciErr sciErr;
    
    /// Variables
    	int i, j, k , ch;
    	int iRows=0;
    	int iCols=0;
    	int *piAddr2=NULL;
    	int *piLen = 0;
    	
    	// to specify deinterlacing method
    	char **method=NULL;

    	/// checking input argument
    	CheckInputArgument(pvApiCtx, 1, 2);
   		CheckOutputArgument(pvApiCtx, 1, 1);

    	vector <uint> v;

    	Mat image;
    	retrieveImage(image, 1);

   		/// new_image contains deinterlaced image of original image
    	Mat new_image(image.rows, image.cols, image.type());

    	int nbInputArguments = *getNbInputArgument(pvApiCtx);
    	if(nbInputArguments == 2)
    	{
      	sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
      	if(sciErr.iErr)
    		{
      		printError(&sciErr, 0);
      		return 0;
        }
      	/// Retrieve string from input parameter. (requires 3 calls)
      	/// first to retrieve dimensions
      	sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, NULL, NULL);
      	if(sciErr.iErr)
      	{
        	printError(&sciErr, 0);
        	return 0;
      	}
    
        //second call to retrieve length of each string    
      	piLen = (int*)malloc(sizeof(int) * iRows * iCols);
      	sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen, NULL);
      	if(sciErr.iErr)
      	{
       		printError(&sciErr, 0);
        	return 0;
      	}

      	method = (char**)malloc(sizeof(char*) * iRows * iCols);
      	for(i = 0 ; i < iRows * iCols ; i++)
        method[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination

      	//third call to retrieve data
      	sciErr = getMatrixOfString(pvApiCtx, piAddr2, &iRows, &iCols, piLen, method);
      	if(sciErr.iErr)
      	{
        	printError(&sciErr, 0);
        	return 0;
      	}
      		
      	// if second argument, method is Linear interpolation
      	if(strcmp(method[0],"Linear interpolation")==0)
      	{
        	for(i = 0; i < image.rows; i++)
        	{
        		for(j = 0; j < image.cols; j++)
        		{
           		if(i == image.rows-1)
            	{
              	if(i%2!=0)
                	new_image.at <uint> (i,j) = image.at <uint> (i-1,j);
              	else
                	new_image.at <uint> (i,j) = image.at <uint> (i,j);
            	}
            	else if(i%2 != 0 and i+1 < image.rows and i-1 >= 0)
              	new_image.at <uint> (i,j) = ( image.at <uint> (i-1,j) + image.at <uint> (i+1,j) ) / 2;
            	else
              	new_image.at <uint> (i,j) = image.at <uint> (i,j);
          	}
        	}
      	}

        // if second argument, method is Vertical temporal median filtering
      	else if(strcmp(method[0],"Vertical temporal median filtering")==0)
      	{
        	for(i = 0; i < image.rows; i++)
        	{
        		for(j = 0;j < image.cols; j++)
          	{
            	v.clear();
            	if(i == image.rows-1)
            	{
              	if(i%2!=0)
                  new_image.at <uint> (i,j) = image.at <uint> (i-1,j);
              	else
                  new_image.at <uint> (i,j) = image.at <uint> (i,j);
            	}
            	else if(i%2 != 0 and i-1 >= 0 and i+1 < image.rows)
            	{
              	v.push_back( image.at <uint> (i-1,j) );
              	v.push_back( image.at <uint> (i,j) );
              	v.push_back( image.at <uint> (i+1,j) );

              	sort( v.begin(),v.end() ); // to find median 

              	new_image.at <uint> (i,j) = v[1];
            	}
            	else
              	new_image.at <uint> (i,j) = image.at <uint> (i,j);
          	}
        	}
      	}
        
        // if second argument, method is Line repetition
        else if(strcmp(method[0],"Line repetition")==0)
        {
          for( i = 0; i < image.rows; i++ )
          {
            for(j = 0; j < image.cols; j++ )
            {
              if( i == image.rows-1 and i%2!=0 )
                new_image.at <uint> (i,j,k) = image.at <uint> (i,j,k);
              if( i%2 != 0 and i-1 >=0 )
                new_image.at <uint> (i,j,k) = image.at <uint> (i-1,j,k);
              else
                new_image.at <uint> (i,j, k) = image.at<uint> (i,j,k);
            }
          }
        }
      	// if any other invalid method name is passed as argument
        else
        {
          Scierror(99,"Wrong method specified, use Linear interpolation or Vertical temporal median filtering or Line repetition as method name \n");
          return 0;
        }
      }

		  // if no of input argument is 1 that is no method is specified then we use Line repetition method by default
      else
    	{
        for( i = 0; i < image.rows; i++ )
      	{
        	for(j = 0; j < image.cols; j++ )
        	{
          	if( i == image.rows-1 and i%2!=0 )
            	new_image.at <uint> (i,j,k) = image.at <uint> (i,j,k);
          	if( i%2 != 0 and i-1 >=0 )
            	new_image.at <uint> (i,j,k) = image.at <uint> (i-1,j,k);
          	else
            	new_image.at <uint> (i,j, k) = image.at<uint> (i,j,k);
        	}
      	}
     	}
    
      // writing to scilab memory
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

