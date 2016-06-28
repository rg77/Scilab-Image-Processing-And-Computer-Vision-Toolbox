function[dstMat] = imextendedmin(srcImg, H-min)

	srcMat = mattolist(srcImg)

	out = opencv_imextendedmin(srcMat, H-min)
	
	channel = size(out)
	
	for i = 1: channel
		dstMat(:,:,i) = out(i)
	end
	
endfunction
