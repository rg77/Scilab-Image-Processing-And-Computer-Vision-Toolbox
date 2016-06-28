function [outputImg]=imgaussfilt3(inputImage, filter_height, filter_width, sigmaX, sigmaY)
	inputList=mattolist(inputImage);
    outputList=opencv_imgaussfilt3(inputList, filter_height, filter_width, sigmaX, sigmaY);
    for i=1:size(outputList)
        outputImg(:,:,i)=outputList(i)
    end
endfunction
