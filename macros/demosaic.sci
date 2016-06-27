function [out]=demosaic(src,code,dstCn)
         input_image1=mattolist(src);
         a=opencv_demosaic( input_image1,code,dstCn);
         dimension=size(a)
         for i = 1:dimension
              out(:,:,i)=a(i);
         end
endfunction;
