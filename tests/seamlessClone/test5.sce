dst = imread("../images/color3.jpg");
mask = imread("../images/mask_cartoon.jpg");
src = imread("../images/cartoon.jpg");
output = seamlessClone(src,dst,mask,[2 3], 2 ); //error because src image does not fit into destination image completely
imshow(output);
