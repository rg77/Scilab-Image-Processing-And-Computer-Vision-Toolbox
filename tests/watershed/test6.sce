src= imread("../images/watershed3.jpg");
mask=roiFreeHand(src); //creating markers - selecting each coin separately using the function roiFreeHand
w2=watershed(src,mask); //retrieve the segmented image
imshow(w2);

