function [outputImg,res]= threshold(inputImage, threshold_value, max_value,thresholdType)
	inputList=mattolist(inputImage);
    [outputList,res]=opencv_threshold(inputList, threshold_value, max_value,thresholdType)
     for i=1:size(outputList)
        outputImg(:,:,i)=outputList(i)
    end
endfunction
