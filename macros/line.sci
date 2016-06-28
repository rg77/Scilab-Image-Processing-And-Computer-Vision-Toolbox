function outputImg= line(inputImage , x1 , y1 , x2,y2,r_value,g_value,b_value,thickness,linetype,shift)
	inputList=mattolist(inputImage);
    outputList=opencv_line(inputList , x1 , y1 , x2,y2,r_value,g_value,b_value,thickness,linetype,shift)
    for i=1:size(outputList)
        outputImg(:,:,i)=outputList(i)
    end
endfunction
