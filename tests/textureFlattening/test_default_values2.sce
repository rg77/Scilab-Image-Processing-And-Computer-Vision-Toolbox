
src = imread("../images/color2.jpeg");
[rows cols] = size(src);
mask = zeros(rows,cols);
mask(1:rows,1:floor(cols/2)) = 255;
output = textureFlattening(src,mask,[],[], 7 ); //providing value for only 3rd optional argument
imshow(output);

