 I = imread('../images/color3.jpg');
c1 = [200 250 300 250 200 150]
r1 = [ 170 170 135 100 100 135] //declare points in reverse order
 gray = rgb2gray(I) ;
BW = roiPoly(gray,c1,r1);
imshow(BW)
