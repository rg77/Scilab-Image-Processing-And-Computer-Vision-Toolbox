src = imread("../images/image_0197.jpg");
[rows cols] = size(src)
mask = zeros(rows,cols);
mask(1:rows,1:(cols/2)) = 255;
output = textureFlattening(src,mask,25,90, 3 );
imshow(output);
