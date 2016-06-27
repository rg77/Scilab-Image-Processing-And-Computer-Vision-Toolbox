function [out]=fitellipse(InputArraypoints)
           [t1 t2 t3 t4 t5]= opencv_fitellipse(InputArraypoints);
           out=struct("size1",t1,"size2",t2,"center1",t3,"center2",t4,"angle",t3);
endfunction;
