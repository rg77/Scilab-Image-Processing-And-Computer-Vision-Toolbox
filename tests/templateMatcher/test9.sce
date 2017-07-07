src= imread("../images/cars.jpg");
gray_src = rgb2gray(src);
tem = imread("../images/red_car.jpg");
tem = rgb2gray(tem);
imshow(tem);
[j x y] =templateMatcher(gray_src,tem);
[rows cols] =size(tem)
t = rectangle(src,x,y,x+cols,y+rows,0,0,255,2,8,0);
imshow(t)

