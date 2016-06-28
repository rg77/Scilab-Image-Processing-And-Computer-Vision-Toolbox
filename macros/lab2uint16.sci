function [output] = lab2uint16(pstData)
	a = opencv_lab2uint16(pstData);
	d = size(a);
	for i=1:d
		output(:,:,i) = a(i);
	end
endfunction
