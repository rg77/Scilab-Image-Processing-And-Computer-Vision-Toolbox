dst = imread("../images/sky.jpg");
src = imread("../images/gs1.jpg");
mask = roiFreeHand(src);
output = seamlessClone(src,dst,mask,[150 120], 2 );
imshow(output);
