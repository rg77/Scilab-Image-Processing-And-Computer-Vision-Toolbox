function[stats] = graycoprops(srcImg, varargin)

	[lhs, rhs] = argn(0)
	
	srcMat = mattolist(srcImg)

	select rhs
		case 1 then
			out = graycoprops(srcMat)
		case 2 then
			out = graycoprops(srcMat, varargin(1))
		end
	
	channels = size(out)
	
	for i = 1:channels
		stats(:,:,1) = out(1)
	end
endfunction
