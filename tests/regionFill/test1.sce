src= imread("../images/regionFill_test5.jpg");
mask = roiFreeHand(src);
p=regionFill(src,3,mask);
imshow(p);

