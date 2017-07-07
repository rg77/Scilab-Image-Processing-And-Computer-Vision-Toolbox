 I = imread('../images/color3.jpg');
 c = [ 150 200 250 300 250 200 ] ;  //defining the column indices of the vertices of the ROI
 r = [ 135 100 100 135 170 170 ] ;  //defining the corresponding row indices of the vertices of the ROI
 gray = rgb2gray(I) ;                           //for a grayscale image
 BW = roiPoly(I, c, r); //creating a mask
 imshow(BW)   //viewing the mask
 
 
 
  
