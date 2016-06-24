function [varargout] = detectSURFFeatures(image, varargin)
// This function is used to detect SURF(Speeded Up Robust Features) Features in a grayscale Image.
//
// Calling Sequence
//   result = detectSURFFeatures(Image);
//	 result = detectSURFFeatures(Image, Name, Value, ...)
//
// Parameters
// result: SURFPoints struct which contains Location, Orientation, Metric, SignOfLaplacian, Scale and Count of the features.
// Image :  Input image, specified as a A-by-N 2D grayscale.
// MetricThreshold : (Optional) Threshold for the keypoint Detector. (Default -  1000.0)
// NumOctaves : (Optional)The number of a gaussian pyramid octaves that the detector uses. (Default - 3) The Value must be an integer scalar in between 1 and 4.
// NumScaleLevels : (Optional) The number of scale levels for each octave. (Default - 4) The Value must be an integer scalar greater than or equal to 3.
// ROI : (Optional) Region Of Interest. This is taken as a vector [u v width height]. When specified, the function detects the key points within region of area width*height with u and v being the top left corner coordinates.
// Description
// This function return the SURF(Speeded Up Robust Features) Interest Points for a 2D Grayscale image. It is scale- and rotation- invariant point detector and descriptor and its application include Camera Calibration, 3D Reconstruction, Object Recognition to name a few. 
//
// Examples
// image = imread('sample.jpg');
// results = detectSURFFeatures(image);
//
// Authors
//  Shashank Shekhar
	image_list = image;	
	[ lhs, rhs ] = argn(0)
	if rhs > 9 then
		error(msprintf("Too many input arguments"))
	end	
	if lhs > 1 then
		error(msprintf("Not enough input arguments"))
	end
	select rhs 
		case 1 then			
			[a b c d e f] = ocv_detectSURFFeatures(image_list)		
		case 3 then			
			[a b c d e f] = ocv_detectSURFFeatures(image_list, varargin(1), varargin(2))			
		case 5 then		
			[a b c d e f] = ocv_detectSURFFeatures(image_list, varargin(1), varargin(2), varargin(3), varargin(4))			
		case 7 then	
			[a b c d e f] = ocv_detectSURFFeatures(image_list, varargin(1), varargin(2), varargin(3), varargin(4), varargin(5), varargin(6))			
		case 9 then		
			[a b c d e f] = ocv_detectSURFFeatures(image_list, varargin(1), varargin(2), varargin(3), varargin(4), varargin(5), varargin(6), varargin(7), varargin(8))
	end
	varargout(1) = struct('KeyPoints', a, 'Orientation', b, 'Metric', c ,'SignOfLaplacian', d,'Scale', e, 'Count', f );
endfunction