function [out]=mean1(image)
	 image1=mattolist(image);
         out=opencv_mean1(image1);
endfunction;
