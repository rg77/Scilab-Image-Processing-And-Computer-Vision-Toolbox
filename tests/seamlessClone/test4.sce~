dst = imread("../images/color3.jpg");
src = imread("../images/cartoon.jpg");
mask = roiFreeHand(src);
output = seamlessClone(src,dst,mask,[7500 120], 1 );//error because src image does placed outside the destination image
imshow(output);
