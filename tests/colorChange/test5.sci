src=imread("../images/color1.jpg");
[row col] = size(src)
mask = zeros(row,col) //creating a mask
mask(100:140,100:150) =255
imshow(mask);
c=colorChange(src,mask,2.5,0.5,100);//out of range values
imshow(c);
