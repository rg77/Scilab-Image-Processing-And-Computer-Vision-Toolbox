function [output] = rgb2ntsc(img)
	image = mattolist(img);
	a = opencv_rgb2ntsc(image);
	d = size(a);
	for i=1:d
		output(:,:,i) = a(i);
	end
endfunction
