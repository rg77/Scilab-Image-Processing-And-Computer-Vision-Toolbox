//changing kernel size
src = imread("../images/color2.jpeg");
[rows cols] = size(src);
mask = zeros(rows,cols);
mask(1:rows,1:cols/2) = 255;
output = textureFlattening(src,mask,70,150, 7 );
imshow(output);
