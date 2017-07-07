src=imread("../images/color2.jpeg");
mask = roiFreeHand(src);
c=illuminationChange(src,mask,1.7); //using default value for 2nd input argument
imshow(c);
