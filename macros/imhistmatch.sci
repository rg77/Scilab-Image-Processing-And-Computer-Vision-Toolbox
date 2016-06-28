function [outputImg]=imhistmatch(inputImage,refImage,varargin)
     [lhs rhs]=argn(0);
    if rhs>3
        error(msprintf(" Too many input arguments"));
    end
    inputList=mattolist(inputImage);
    refList=mattolist(refImage);
    select rhs
        case 2 then
            outputList=opencv_imhistmatch(inputList,refList);
        case 3 then
            outputList=opencv_imhistmatch(inputList,refList,varargin(1));
   end
    for i=1:size(outputList)
        outputImg(:,:,i)=outputList(i)
    end
endfunction
