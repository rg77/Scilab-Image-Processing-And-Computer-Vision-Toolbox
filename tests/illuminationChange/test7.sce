src=imread("../images/image_0197.jpg");
mask = roiFreeHand(src);
c=illuminationChange(src,mask,1.2,0.6);
imshow(c);
