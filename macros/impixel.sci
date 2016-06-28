function [out]=impixel(image,value1,value2)
    image1=mattolist(image);
         out=opencv_impixel(image1,value1,value2);
         
endfunction;
