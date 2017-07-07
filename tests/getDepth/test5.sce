src= imread("../images/image_0197.jpg");
gray_src = rgb2gray(src);
tem = imread("../images/face.png");
tem = rgb2gray(tem);
imshow(tem);
[j x y] =templateMatcher(gray_src,tem);
depth = getDepth(j) ; //get the depth
disp(depth) ; //view the output
