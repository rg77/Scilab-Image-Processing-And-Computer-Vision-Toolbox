function[dstMat] = imextendedmax(srcImg, H-max)

	srcMat = mattolist(srcImg)

	out = opencv_imfill(srcMat, H-max)
	
	channel = size(out)
	
	for i = 1: channel
		dstMat(:,:,i) = out(i)
	end
	
endfunction
