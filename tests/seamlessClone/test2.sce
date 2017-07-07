//using a complete white mask
dst = imread("../images/color3.jpg");
src = imread("../images/cartoon.jpg");
[rows cols] = size(src)
mask = zeros(rows,cols);
mask(50:100,1:cols) = 255;
output = seamlessClone(src,dst,mask,[75 120], 1 );
imshow(output);
