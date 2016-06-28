function [out] = ellipse2poly(centre_x,centre_y,width,height,angle,arcstart,arcend,delta)
	out = opencv_ellipse2poly(centre_x,centre_y,width,height,angle,arcstart,arcend,delta);
endfunction
