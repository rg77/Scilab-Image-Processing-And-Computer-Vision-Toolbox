src=imread("../images/image_0197.jpg.jpeg");
mask = roiFreeHand(src);
c=illuminationChange(src,mask,1.9,0.1);
imshow(c);
