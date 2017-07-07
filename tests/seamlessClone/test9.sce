//both are grayscale images
dst = imread("../images/gs2.bmp");
src = imread("../images/gs1.jpg");
mask = roiFreeHand(src);
output = seamlessClone(src,dst,mask,[175 120], 1 );
imshow(output);
