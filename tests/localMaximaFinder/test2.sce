//for a matrix - increasing the neighbourhood
mat = [0 0 0 0 0 0;0 0 0 1 0 0;0 0 7 0 0 0 ;0 0 0 0 0 0];
loc=localMaximaFinder(mat,[3 3],4,1);
loc
