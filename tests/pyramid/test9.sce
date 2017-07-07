inputimage1 = imread('image_0197.jpg'); //read an image
 a=pyramid(inputimage1,'reduce',6); //perform 'expand' operation
 imshow(a); //view the output
