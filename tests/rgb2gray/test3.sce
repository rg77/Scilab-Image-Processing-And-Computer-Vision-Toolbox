src = imread("../images/color3.jpg");
gray1=rgb2gray(src);
gray2=rgb2gray(gray1); //error : input gray image
imshow(gray);
