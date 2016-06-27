function new_image = undistortImage(image, cameramatrix, varargin)
// Transforms an image to compensate for lens distortion.
//
// Calling Sequence
// newimage = undistortImage(image, cameramatrix)
// newimage = undistortImage(image, cameramatrix, ["Parameter1", value1,["Parameter2", value2]])
//
// Parameters
// image : Distorted input image
// cameramatrix : Input 3 * 3 cameramatrix
// distcoeffs : Input vector of distortion coefficients of 4, 5, or 8 elements. If the vector is empty, the zero distortion coefficients are assumed.
// newCameraMatrix : Camera matrix of the distorted image. By default, it is same as cameramatrix.
// newimage : Corrected image that has same size and type as original image
//
// Description
// The function returns a newimage containing the input image with lens distortion removed.
//
// Examples
// J = undistortImage(I, cameramatrix)
// J = undistortImage(I, cameramatrix, "distcoeffs", distcoeffs)
//
// Authors
// Suraj Prakash

	[ lhs rhs ] = argn(0)
	
	if lhs > 1 then
		error(msprintf("Too many output arguments"))
	end
	
	if rhs > 6 then
		error(msprintf("Too many input arguments"))
	end
	
	if rhs < 2 then
		error(msprintf("Too less input arguments"))
	end
	
	if modulo(rhs, 2) <> 0
		error(msprintf("Incorrect input arguments"))
	end
	
	image_list = mattolist(image)
	
	if rhs == 2 then
		
		temp = opencv_undistortImage(image_list, cameramatrix)
	
	elseif rhs == 4 then
		
		temp = opencv_undistortImage(image_list, cameramatrix, varargin(1), varargin(2))
	
	elseif rhs == 6 then
		
		temp = opencv_undistortImage(image_list, cameramatrix, varargin(1), varargin(2), varargin(3), varargin(4))
	
	else
		error(msprintf("Error in input arguments"))	
	end
	
	sz = size(temp)
	
	for i=1:sz
		new_image(:, :, i) = temp(i)
	end
		

endfunction
