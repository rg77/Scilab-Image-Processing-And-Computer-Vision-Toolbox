function [out]=extractLBPFeatures(input1,varargin)
         [lhs rhs] = argn(0)
         if rhs>13 then
         error(msprintf("Too many input arguments"))
         elseif rhs==13 then
         a=opencv_extractLBPFeatures(input1,varargin(1),varargin(2),varargin(3),varargin(4),varargin(5),varargin(6),varargin(7),varargin(8),varargin(9),varargin(10),varargin(11),varargin(12))
         out(:,:,1)=a(1)
         elseif rhs==12 then
         a=opencv_extractLBPFeatures(input1,varargin(1),varargin(2),varargin(3),varargin(4),varargin(5),varargin(6),varargin(7),varargin(8),varargin(9),varargin(10),varargin(11))
         out(:,:,1)=a(1)
         elseif rhs==11 then
         a=opencv_extractLBPFeatures(input1,varargin(1),varargin(2),varargin(3),varargin(4),varargin(5),varargin(6),varargin(7),varargin(8),varargin(9),varargin(10))
         out(:,:,1)=a(1)
         elseif rhs==10 then
         a=opencv_extractLBPFeatures(input1,varargin(1),varargin(2),varargin(3),varargin(4),varargin(5),varargin(6),varargin(7),varargin(8),varargin(9))
         out(:,:,1)=a(1)
         elseif rhs==9 then
         a=opencv_extractLBPFeatures(input1,varargin(1),varargin(2),varargin(3),varargin(4),varargin(5),varargin(6),varargin(7),varargin(8))
         out(:,:,1)=a(1)
         elseif rhs==8 then
         a=opencv_extractLBPFeatures(input1,varargin(1),varargin(2),varargin(3),varargin(4),varargin(5),varargin(6),varargin(7))
         out(:,:,1)=a(1)
         elseif rhs==7 then
         a=opencv_extractLBPFeatures(input1,varargin(1),varargin(2),varargin(3),varargin(4),varargin(5),varargin(6))
         out(:,:,1)=a(1)
         elseif rhs==6 then
         a=opencv_extractLBPFeatures(input1,varargin(1),varargin(2),varargin(3),varargin(4),varargin(5))
         out(:,:,1)=a(1)
         elseif rhs==5 then
         a=opencv_extractLBPFeatures(input1,varargin(1),varargin(2),varargin(3),varargin(4))
         out(:,:,1)=a(1)
         elseif rhs==4 then
         a=opencv_extractLBPFeatures(input1,varargin(1),varargin(2),varargin(3))
         out(:,:,1)=a(1)
         elseif rhs==3 then
         a=opencv_extractLBPFeatures(input1,varargin(1),varargin(2))
         out(:,:,1)=a(1)
         elseif rhs==2 then
         a=opencv_extractLBPFeatures(input1,varargin(1))
         out(:,:,1)=a(1)
         elseif rhs==1 then
         a=opencv_extractLBPFeatures(input1)
         out(:,:,1)=a(1)
         end,
      
endfunction;
