src= imread("../images/image_0197.jpg"); //another image
gray = rgb2gray(src);
loc=localMaximaFinder(gray,[1 3],4,1);
