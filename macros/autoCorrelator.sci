function relation = autoCorrelator(inputMatrix)
//Calculate 2-D auto correlation of input matrix
//
//Calling Sequence-
//	relation=autoCorrelator(inputMatrix)
//
//Parameters
// inputMatrix: input matrix to calculate auto correlation

//Description
//This function computes 2-D auto correlation of input matrix
//
// Examples
// rel = autoCorrelator([3 1;2 4]) // examples of use

	[r,c] = size(inputMatrix);
	Rep = zeros(r + r*2-2, c + c*2-2);
	for x = r : r+r-1
		for y = c : c+c-1
		    Rep(x,y) = inputMatrix(x-r+1, y-c+1);
		end
	end
	oRows=r+r-1;
	oCols=c+c-1;
    //(mROw,mCols)- It indicates middle value of the output matrix
	mRow=ceil(oRows/2);
	mCol=ceil(oCols/2);
	relation = zeros(oRows,oCols); //output matrix
    //calculating only half of the output matrix
	for x = 1 : oRows
		for y = 1 : oCols
		    for i = 1 : r
		        for j = 1 : c
		            relation(x, y) = relation(x, y) + (Rep(x+i-1, y+j-1) * inputMatrix(i, j));
		        end
		    end
		    if(mRow==x & mCol==y)
		        break;
		    end
		end
		 if(mRow==x & mCol==y)
		        break;
		 end
	end
    //Copying remainig values
	for x = oRows:-1:1
		for y = oCols:-1:1
		    relation(x, y) = relation(r+r-x, c+c-y) 
		end
		if(mRow==x & mCol==y)
		        break;
		    end
	end
endfunction
