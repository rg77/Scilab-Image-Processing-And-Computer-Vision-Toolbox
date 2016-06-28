function [output] = lab2xyz(vartype,varargin)
	[lhs rhs] = argn(0);
	if(rhs>3)
		eror(msprintf("Too many input arguments"));
	end
	select rhs
	case 1 then
		a = opencv_lab2xyz(vartype);
	case 2 then
		a = opencv_lab2xyz(vartype,varargin(0));
	case 3 then
		a = opencv_lab2xyz(vartype,varargin(0),varargin(1));
	end
	
	d = size(a);
	for i=1:d
		output(:,:,i) = a(i);
	end
endfunction
		
