function new_image = cvtColor(image, code, varargin)
	
	[ lhs, rhs ] = argn(0)
	
	image_list = mattolist(image)
	
	select rhs
		case 2 then
			out = opencv_cvtColor(image_list, code)
		
		case 3 then
			out = opencv_cvtColor(image_list, code, varargin(1))
	end
	
	sz = size(out)
	for i=1:sz
		new_image(:, :, i) = out(i)
	end

endfunction
