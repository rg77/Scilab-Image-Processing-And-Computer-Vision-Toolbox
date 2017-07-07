//using flag RECURS_FILTER = 1 ; increasing sigma r
src = imread("../images/color2.jpeg");
output = edgePreservingFilter(src,1,50, 0.6 );
imshow(output);
