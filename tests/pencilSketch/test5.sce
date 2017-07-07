src = imread("../images/cow2.jpg");
[output1 output2] = pencilSketch(src,100,[],0.07); //using default values for sigma_s
imshow(output1);
//imshow(output2);

