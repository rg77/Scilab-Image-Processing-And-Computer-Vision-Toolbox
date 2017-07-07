
src = imread("../images/color2.jpeg");
[rows cols] = size(src)
mask = zeros(rows,cols);
mask(1:rows,1:cols/2) = 255;
output = textureFlattening(src,mask,50 , 180.5 );//providing value for 1st and 2nd optional argument
imshow(output);

