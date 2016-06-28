function [output] = rgb2xyz(img)
	image = mattolist(img);
	a = opencv_rgb2xyz(image);
	d = size(a);
	for i=1:d
		output(:,:,i) = a(i);
	end
endfunction
