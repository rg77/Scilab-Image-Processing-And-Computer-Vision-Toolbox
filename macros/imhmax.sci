unction[dstMat] = imhmax(srcImg, threshold, connectivity)
	
	[lhs, rhs] = argn(0)
	
	srcMat = mattolist(srcImg)

	chanel_srcMat = size(srcMat)
	
	if chanel_srcMat!=1 then
		error(msprintf("The input image must be 1 channel"))
	end
	
	out = opencv_imhmax(srcMat, threshold, connectivity)
	
	channel = size(out)
	
	for i = 1: channel
		dstMat(:,:,i) = out(i)
	end
	
endfunction
