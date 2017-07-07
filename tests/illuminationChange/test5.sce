src=imread("../images/color2.jpeg");
[row col] = size(src)
mask = zeros(row,col) //creating a mask
mask(100:140,100:150) =255
c=illuminationChange(a,b,7,8);//out of range input value
