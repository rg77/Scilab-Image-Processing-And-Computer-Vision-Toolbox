function guidedFiltered_image = imguidedfilter(varargin)
	
	[lhs, rhs] = argn(0)
	
	image_list = mattolist(varargin(1))
	
	select rhs
		case 1
			out = opencv_imguidedfilter(image_list)
		
		case 2
			out = opencv_imguidedfilter(image_list, varargin(2))
		
		case 3
			out = opencv_imguidedfilter(image_list, varargin(2), varargin(3))
		
		case 4
			out = opencv_imguidedfilter(image_list, varargin(2), varargin(3), varargin(4))
	end
	
	sz = size(out)
	for i=1:sz
		guidedFiltered_image(:, :, i) = out(i)
	end
	
endfunction
