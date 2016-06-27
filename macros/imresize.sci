function [out]=imresize(image,alpha,beta)
   	image1=mattolist(image);
         a=opencv_imresize(image1,alpha,beta);
         dimension=size(a)
         for i = 1:dimension
              out(:,:,i)=a(i);
         end
endfunction;
