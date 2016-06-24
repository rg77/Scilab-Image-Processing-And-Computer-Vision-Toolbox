function [intImage]=integralImage(image)
// This function returns the integral image of an input.
//
// Calling Sequence
//   intImage = integralImage(image);
//
// Parameters
// image: An intensity image which is specified as an MxN grayscale image.
// intImage: Integral Image of the intensity image.
// Description
// Integral image return the integral image of an intensity image (grayscale image);
//
// Examples
// image = [1 2 3 4 5; 6 7 8 9 10; 11 12 13 14 15; 16 17 18 19 20; 21 22 23 24 25];
// intImage = integralImage(image);
//
// Authors
//  Tanmay Chaudhari

        a=opencv_integralImage(input);
        out(:,:,1)=a(1);
	
endfunction
