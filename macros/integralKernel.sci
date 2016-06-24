function [ kernel ] = integralKernel(bbox, weights)
// This function creates a filter to be used with integral images.
//
// Calling Sequence
//   results = intergralKernel(bbox, weight);
//
// Parameters
// results: A structure kernel which contains bounding boxes, weights, filter coefficients, center of filter, size of filter, orientation of filter
// bboxes: bounding boxes that define the filter
// weights: weights corresponding to each of the bbox
//
// Description
// Integral kernel creates box filter which can be used with integral images.
//
// Examples
// bbox = [1 2 3 4; 5 6 7 8];
// weights = [10 11];
// results = integralKernel(bbox, weights);
//
// Authors
//  Tanmay Chaudhari

    [t1 t2 t3 t4 t5] = opencv_integralKernel(bbox, weights)
	
	kernel = struct("bbox",t1(1),"weights",t2,"coefficients",t3(1),"centre",t4(1),"filterSize",t5)
	
endfunction
