src= imread("../images/regionFill_test.jpg");
mask = roiFreeHand(src);
p=regionFill(src,5,mask);
imshow(p);

