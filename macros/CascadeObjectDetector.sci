function [out]=insertObjectAnnotation(input_image,object)
// Detect human body parts like face ,nose,eye and ear.
//
// Calling Sequence
// output_image = insertObjectAnnotation(input_image,object);
//
// Parameters
//
// input_image : image matrix on which Cascadeobject detection has to be performed
// object : An input string which specify the name of the object which has to be detect
// output_image : an output image in which object are bounded by rectangle.
//
// Description 
// This function uses the Viola-Jones algorithm to detect the object from given input image.The function first finds the target object on the image then makes a  rectangle on detected part of the image.To detect the object it uses already trained data  which are stored in .xml file.User can use own trained data for better object detection.
//
// Examples 
// a = imread('lena.jpeg');
// b = insertObjectAnnotation(a,'face');
// imshow(b)
//
// Authors.
//
//Diwakar Bhardwaj
         input_image1=mattolist(input_image);
         a=opencv_CascadeObjectDetector(input_image,object);
         dimension=size(a)
         for i = 1:dimension
              out(:,:,i)=a(i);
         end
     
endfunction;
