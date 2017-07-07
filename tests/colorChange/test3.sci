 src = imread("../images/1.jpg"); 
 [row col] = size(src)
mask = zeros(row,col) //
mask(100:140,100:150) =255
imshow(mask); view the mask
gray = rgb2gray(src);  //using a gray image as source 
outputImage = colorChange(gray, mask,0.5 , 1 ) //specifying value of all optional parameter
imshow(outputImage);//view the output

