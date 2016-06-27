function [out]=ssim(image,reference)
         image1=mattolist(image);
         out=opencv_ssim(image1,reference);
     
endfunction;
