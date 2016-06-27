function new_image = sobel(image, ddepth, dx, dy, ksize, scale, delta)
	
	image_list = mattolist(image)
	
	out = opencv_sobel(image_list, ddepth, dx, dy, ksize, scale, delta)
	
	sz = size(out)
	
	for i=1:sz
		new_image(:, :, i) = out(i)
	end
	
endfunction
