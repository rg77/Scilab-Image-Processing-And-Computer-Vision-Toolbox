function [featureVector ValidPoints]=extractFeatures(image,points,class,varargin)
// This function is used to extract interest point descriptors
//
// Calling Sequence
//   [features, validPoints] = extractFeatures(Image, points, Class, Name, Value... )
//
// Parameters
//  Image: Input Image, should be a 2-D grayscale. The Input Image should be real
//  points: M-by-2 vector containing the list of Interest Points
//  Class: The Name of the feature detecting algorithm used for interest point detection
//  Method: The desired algorithm to be used for feature vector calculation
//  Metric: M-by-1 vector containing the strength of eack interest Point
//  BlockSize: An odd integer scalar for defining a square neighbourhood centered at each interest point for Block Detection Method
//  Upright: Rotation Invariance Flag specified as a boolean 'True' or 'False'. Default Value: False(0)
//  SURFSize: Length of SURF Feature Vector. Values: [64(default)|128]
//  Orientation: M-by-1 vector containing the orientation of the detected feature in radians
//  Axes: M-by-2 vector containing the axes length values detected for MSER Object
//  Scale: M-by-1 vector containing the scale of detection of each Interest Point
//  SignOfLaplacian: M-by-1 vector containing the values of Sign of Laplacian of a SURF Object
//  features: M-by-N Feature Matrix containing M feature vectors each of length N
//  validPoints: M-by-2 Matrix containing the set of valid points whose features have been extracted
//
// Description
//  The function derives the descriptors from pixels surrounding an interest point, where each interest point specifies the location of the detected region center.
//
// Examples
//  image = imread('sample.jpg');
//  [corners count metric] = detectHarrisFeatures(image);
//  [features, valid_corners] = extractFeatures(image, corners, "Class", "cornerPoints", "Metric", metric);
//
// Authors
//  Umang Agrawal
//  Sridhar Reddy

    [lhs rhs]=argn(0);
    if lhs>2
         error(msprintf(" Too many output arguments"));
    elseif rhs>13
        error(msprintf(" Too many input arguments"));
    end
    imageList=mattolist(image);

    //default values
    blockSize=0;
    upright=1;
    surfSize=64;
    if (class=="cornerPoints" | class=="BRISKPoints")
    	method="FREAK";
    elseif (class=="MSERRegions" | class=="SURFPoints")
    	method="SURF";
    elseif (class=="Matrix")
    	method="Block";
    else
    	error(msprintf(" Invalid class type, check documentation for the list of valid types"));
    end

    //Count to ensure argument called only once
    count_metric=0,count_orientation,count_scale,count_axes,count_signOfLaplacian;
    count_method=0, count_block=0, count_surf=0, count_upright=0;

    //Parsing input arguments
	for i=1:rhs-3
		if (varargin(i)=="Metric")
			if(count_metric==0)
				i=i+1;
				metric=varargin(i);
			else
				error(msprintf(" Metric argument has been called twice"));
			end
		elseif (varargin(i)=="Orientation")
			if(count_orientation==0)
				i=i+1;
				orientation=varargin(i);
			else
				error(msprintf(" Orientation argument has been called twice"));
			end
		elseif (varargin(i)=="Scale")
			if(count_scale==0)
				i=i+1;
				scale=varargin(i);
			else
				error(msprintf(" Scale argument has been called twice"));
			end
		elseif (varargin(i)=="Axes")
			if(count_axes==0)
				i=i+1;
				axes=varargin(i);
			else
				error(msprintf(" Axes argument has been called twice"));
			end
		elseif (varargin(i)=="SignOfLaplacian")
			if(count_signOfLaplacian==0)
				i=i+1;
				signOfLaplacian=varargin(i);
			else
				error(msprintf("SignOfLaplacian argument has been called twice"));
			end
		elseif (varargin(i)=="Method")
			if(count_method==0)
				i=i+1;
				method=varargin(i);
			else
				error(msprintf("Method argument has been called twice"));
			end
		elseif (varargin(i)=="BlockSize")
			if(count_block==0)
				i=i+1;
				blockSize=varargin(i);
			else
				error(msprintf("BlockSize argument has been called twice"));
			end
		elseif (varargin(i)=="SurfSize")
			if(count_surf==0)
				i=i+1;
				surfSize=varargin(i);
			else
				error(msprintf("SurfSize argument has been called twice"));
			end
		elseif (varargin(i)=="Upright")
			if(count_upright==0)
				i=i+1;
				upright=varargin(i);
			else
				error(msprintf("Upright argument has been called twice"));
			end
		else
			error(msprintf("Wrong argument, check documentation for the list of valid arguments"));
	end

	select class
		case "cornerPoints" then
			if(count_metric==0)
				error(msprintf("For cornerPoints Class Metric argument is necessary"));
			end
			[featureVectors ValidPoints numBits numFeatures count metric]=extractFeatures(imageList, points, "Class", class, "Method", method, "surfSize", surfSize, "blockSize", blockSize, "upright", upright, "Metric",metric);

		case "BRISKPoints" then
			if(count_metric==0 | count_scale==0 | count_orientation==0)
				error(msprintf("For BRISKPoints Class Metric, Orientation and Scale arguments are necessary"));
			end
			[featureVectors ValidPoints numBits numFeatures scale metrix orientation count]=extractFeatures(imageList, points, "Class", class, "Method", method, "surfSize", surfSize, "blockSize", blockSize, "upright", upright, "Metric",metric,"Orientation",orientation,"Scale",scale);

		case "MSERRegions" then
			if( count_orientation==0 | count_axes==0)
				error(msprintf("For MSERRegions Class Orientation Matrix and Axes Matrix arguments are necessary."));
			end
			[featureVectors ValidPoints numBits numFeatures count axes orientation]=extractFeatures(imageList, points, "Class", class, "Method", method, "surfSize", surfSize, "blockSize", blockSize, "upright", upright, "Metric",metric,"Orientation",orientation,"Axes",axes);

		case "SURFPoints" then
			if(count_scale==0 | count_metric==0 | count_signOfLaplacian==0 | count_orientation==0)
				error(msprintf("For SURFPoints Class Metric, Orientation, Scale and SignOfLaplacian arguments are necessary"));
			end
			[featureVectors ValidPoints numBits numFeatures count scale metrix signOfLaplacian orientation]=extractFeatures(imageList, points, "Class", class, "Method", method, "surfSize", surfSize, "blockSize", blockSize, "upright", upright, "Metric",metric,"Orientation",orientation,"Scale",scale,"SignOfLaplacian",signOfLaplacian);

		case "Matrix" then
			if(count_block==0)
				error(msprintf("For Matrix Class  BlockSize argument is necessary"));
			end
			[featureVectors ValidPoints numBits numFeatures]=extractFeatures(imageList, points, "Class", class, "Method", method, "surfSize", surfSize, "blockSize", blockSize, "upright", upright,"BlockSize",blockSize);



    featureVector=struct("Type","binaryFeatures",'Features',featureVectors,'NumBits',numBits,'NumFeatures',numFeatures);

endfunction
