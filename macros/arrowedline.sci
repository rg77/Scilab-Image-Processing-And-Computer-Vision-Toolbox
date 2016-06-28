function[dstImg] = arrowedline(srcImg, x1, y1, x2, y2, R, G, B, varargin)

	[lhs, rhs] = argn(0)
	
	srcMat = mattolist(srcImg)

	select rhs
		case 8 then
			out = arrowedline(srcMat, x1, y1, x2, y2, R, G, B)
		case 9 then
			out = arrowedline(srcMat, x1, y1, x2, y2, R, G, B,varargin(1))
		case 10 then
			out = arrowedline(srcMat, x1, y1, x2, y2, R, G, B, varargin(1),varargin(2))
		case 11 then
			out = arrowedline(srcMat, x1, y1, x2, y2, R, G, B, varargin(1),varargin(2), varargin(3))
		case 12 then
			out = arrowedline(srcMAt, x1, y1, x2, y2, R, G, B, varargin(1),varargin(2), varargin(3), varargin(4))
	end
	
	channels = size(out)
	
	for i = 1:channels
		dstImg(:,:,1) = out(1)
	end
endfunction
