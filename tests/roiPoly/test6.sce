 I = imread('../images/color2.jpeg');
c1 = [200 250 300 250 200 150 200]
r1 = [ 170 170 135 100 100 135 154] 
BW = roiPoly(I,c1,r1);
imshow(BW)
