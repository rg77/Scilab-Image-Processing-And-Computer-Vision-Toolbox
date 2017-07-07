 I = imread('../images/color2.jpeg');
c1 = [75 125 150 50]
r1 = [ 50 50 100 100] 
BW = roiPoly(I,c1,r1);
imshow(BW)
