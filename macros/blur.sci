function [out]=blur(input_image ,ksize_width,ksize_height,anchorX,anchorY)
         input_image1=mattolist(input_image);
         a=opencv_blur(input_image1, ksize_width,ksize_height,anchorX,anchorY);
         dimension=size(a)
         for i = 1:dimension
              out(:,:,i)=a(i);
         end
     
endfunction;
