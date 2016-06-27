function [output] = rgb2lab(img)
	image = mattolist(img);
	a = opencv_rgb2lab(image);
	d = size(a);
	for i=1:d
		output(:,:,i) = a(i);
	end
endfunction
