 I = imread('../images/color3.jpg');
 c = [200 250 250 200] ;  //defining the column indices of the vertices of the ROI
 r = [100 100 170 170 ] ;  //defining the corresponding row indices of the vertices of the ROI
 BW = roiPoly(I, c, r); //creating a mask
 imshow(BW)   //viewing the mask
 c=colorChange(I,BW,2.4 ,2.0, 2.2); //using this mask in colorChange function
 imshow(c);
