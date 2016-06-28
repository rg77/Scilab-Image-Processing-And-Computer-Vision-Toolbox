function [out]=erode(input_image ,kernel,actualkernel,anchorX,anchorY)
input_image1=mattolist(input_image);
         a=opencv_erode(input_image1 ,kernel,actualkernel,anchorX,anchorY);
         dimension=size(a)
         for i = 1:dimension
              out(:,:,i)=a(i);
         end
     
endfunction;
