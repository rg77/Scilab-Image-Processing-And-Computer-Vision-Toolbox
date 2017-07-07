a=imread("../images/color1.jpg");
mask=roiFreeHand(a);
imshow(mask);
c=colorChange(a,mask,2.4 ,2.0, 2.2);
imshow(c);
