dst = imread("../images/sky.jpg");
src = imread("../images/gs1.jpg");
mask = roiFreeHand(src);
output = seamlessClone(src,dst,mask,[350 180], 1 );
imshow(output);
