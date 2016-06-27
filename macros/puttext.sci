function new_image = puttext(image, x1, y1, fontscale, r_value, g_value, b_value, text, fontface, varargin)
	
	/// varargin(1) = thickness = 1, 
	/// varargin(2) = linetype = 8, 
	/// varargin(3) = bottomleft = false
	
	[ lhs, rhs ] = argn(0)
	
	image_list = mattolist(image)
	
	select rhs
		case 9 then
			out = opencv_puttext(image_list, x1, y1, fontscale, r_value, g_value, b_value, text, fontface)
		
		case 10 then
			out = opencv_puttext(image_list, x1, y1, fontscale, r_value, g_value, b_value, text, fontface, varargin(1))
			
		case 11 then
			out = opencv_puttext(image_list, x1, y1, fontscale, r_value, g_value, b_value, text, fontface, varargin(1), varargin(2))
		
		case 12 then
			out = opencv_puttext(image_list, x1, y1, fontscale, r_value, g_value, b_value, text, fontface, varargin(1), varargin(2), varargin(3))
	end
	
	sz = size(out)
	
	for i = 1 : sz
		new_image(:, :, i) = out(i)
	end
	
endfunction
