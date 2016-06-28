function [isepi, varargout ] = isEpipoleInImage(fundamental_matrix, imagesize)
// Find whether image contains epipole.
//
// Calling Sequence
// isepi = isEpipoleInImage(F, imagesize)
// [isepi, epipole] = isEpipoleInImage(F, imagesize)
//
// Parameters
// F : A 3 * 3 fundamental matrix computed from stereo images. It should be double or single
// imagesize : The size of the image
// isepi : Logical value true / false denoting whether the image contains epipole
// epipole : Location of the epipole. It is 1 * 2 vector.
// 
// Description
// The function determines whether the image with fundamental matrix F contains the epipole or not. It also gives the position of the epipole. 
//
// Examples
// F = estimateFundamentalMatrix(matchedPoints1, matchedPoints2)	
// imageSize = [200 300]
// isepi = isEpipoleInImage(F, imagesize)
// [isepi, epipole] = isEpipoleInImage(F, imagesize)
//
// Authors
// Suraj Prakash
	
	[ lhs, rhs ] = argn(0)
	
	if lhs > 2 then
		error(msprintf("Too many output arguments"));	
	end
	/// If there is more than one output parameter
	[rows cols] = size(fundamental_matrix)
	if rows ~= 3 | cols ~=3 then
		error(msprintf("Invalid size of fundamental matrix\n"));
	end
	[rows1 col2] = size(imagesize)
	if rows1 ~=1 | cols ~= 2 then
		error(msprintf("Invalid image size matrix\n"));
	end
	if lhs == 2 then
		[isepi, temp ] = opencv_isEpipoleInImage(fundamental_matrix, imagesize);
		varargout(1) = temp;
	/// if there is only one output parameter
	else
		isepi = opencv_isEpipoleInImage(fundamental_matrix, imagesize);
	end
	
endfunction
