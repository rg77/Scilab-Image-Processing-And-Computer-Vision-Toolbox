function[cropImg] = imcrop(srcImg, x-coor, y-coor, width, height)
	srcMat = mattolist(srcImg)
	cropImg = opencv_imcrop(srcMat, x-coor, y-coor, width, height)
endfunction
