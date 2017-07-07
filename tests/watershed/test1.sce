src= imread("../images/watershed1.jpg");
w2=watershed(src);// markers are created after performing distance transform by default
imshow(w2);

