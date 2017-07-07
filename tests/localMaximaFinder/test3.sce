src= imread("../images/color2.jpeg");//input is grayscale image
gray = rgb2gray(src); 
loc=localMaximaFinder(gray,[1 3],4,1);
