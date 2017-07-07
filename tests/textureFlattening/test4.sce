src = imread("../images/color2.jpeg");
[rows cols] = size(src);
mask = zeros(rows,cols);
mask(1:rows/2,1:cols) = 255;
output = textureFlattening(src,mask,70,1000, 7 );//out of range values
imshow(output);
