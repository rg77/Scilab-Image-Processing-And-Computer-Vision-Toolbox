unction[dstMat] = imsharpen(SrcImg)
	
	srcMat = mattolist(srcImg)

	out = opencv_imsharpen(SrcMat)
	
	channel = size(out)
	
	for i = 1: channel
		dstMat(:,:,i) = out(i)
	end
	
endfunction
