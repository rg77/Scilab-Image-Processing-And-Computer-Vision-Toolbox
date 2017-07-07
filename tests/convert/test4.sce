src = imread("../images/color2.jpeg"); //reading an image
src = rgb2gray(src);
depth = getDepth(src) ; //get the depth
disp(depth) ; //view the output
src = convert(src,'CV_16S'); //converting to 8 bit unsigned int
depth = getDepth(src) ; //get the depth
disp(depth) ; //view the output
size(src)

