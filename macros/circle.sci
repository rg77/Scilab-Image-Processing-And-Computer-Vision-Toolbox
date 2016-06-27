function [out]=circle(input_image , x_coordinate_of_centre , y_coordinate_centre , radius, r_value,g_value,b_value,thickness,linetype,shift)
           input_image1=mattolist(input_image);
         a=opencv_circle(input_image1 , x_coordinate_of_centre , y_coordinate_centre , radius, r_value,g_value,b_value,thickness,linetype,shift);
         dimension=size(a)
         for i = 1:dimension
              out(:,:,i)=a(i);
         end
     
endfunction;
