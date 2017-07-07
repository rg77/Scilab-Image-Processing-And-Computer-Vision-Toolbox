 src = imread("../images/color2.jpeg"); 
 mask = roiFreeHand(src);
 c=illuminationChange(src,mask,1.3,0.6,7) //wrong no of input arg
imshow(c);
