dst = imread("../images/color3.jpg");
src = imread("../images/cartoon.jpg");
mask = roiFreeHand(src);
output = seamlessClone(src,dst,mask,[75 120], 1 );
imshow(output);
