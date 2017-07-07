src= imread("../images/image_0197.jpg"); //error - even neighbourhood
gray = rgb2gray(src);
loc=localMaximaFinder(gray,[2 2],4,1);
