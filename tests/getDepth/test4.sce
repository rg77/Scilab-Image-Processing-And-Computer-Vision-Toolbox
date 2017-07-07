src = imread("../images/color2.jpeg"); //reading an image
gray = rgb2gray(src); //converting to grayscale
depth = getDepth(gray) ; //get the depth
disp(depth) ; //view the output
