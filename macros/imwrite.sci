/// image will come in form of matrix
function imwrite(image_matrix, image_path)
	
	image_list = list()
	
	[rows, cols, dimen] = size(image_matrix)
	
	for i=1:dimen
		image_list(i) = image_matrix(:, :, i)
	end
	
	opencv_imwrite(image_list, image_path)
	
endfunction
