src= imread("../images/regionFill_test6.jpg");
mask = roiFreeHand(src);
p=regionFill(src,5,mask);
imshow(p);

