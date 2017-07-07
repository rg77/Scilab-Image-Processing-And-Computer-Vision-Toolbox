src = imread("../images/color2.jpeg"); //reading an image
depth = getDepth(src) ; //get the depth
disp(depth) ; //view the output
size(src)//view no of channels
src = convert(src,'CV_8U'); //converting to 8 bit unsigned int
depth = getDepth(src) ; //get the depth
disp(depth) ; //view the output
size(src)//no. of channel remains same

