function new_image = imfuse(image1, image2, method, scaling)
	
	image_list1 = mattolist(image1)
	image_list2 = mattolist(image2)
	out = opencv_imfuse(image_list1, image_list2, method, scaling)
	
	sz = size(out)
	
	for i=1:sz
		new_image(:, :, i) = out(i)
	end
	
endfunction
