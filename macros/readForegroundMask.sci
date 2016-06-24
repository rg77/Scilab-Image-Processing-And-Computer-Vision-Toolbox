function varargout=readForegroundMask(varargin)
[lhs,rhs]=argn(0)
    if rhs<1 then
         error(msprintf(" Not enough input arguments"))
    end
    if rhs>1 then
          error(msprintf(" Too many input arguments to the function"))     
    end
    if lhs>1 then
         error(msprintf(" Too many output arguments"))
    end
    varargout(1) = ocv_readForegroundMask(varargin(1));
endfunction
