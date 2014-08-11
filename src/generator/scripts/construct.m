%
% Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
%
% Please see distribution for license.
%
function [ret] = construct(data)
  domain ='';
  if(iscomplex(data))
    domain = 'Complex';
  else
    domain = 'Real';
  end

  dim = '';
  dsize = '';
  if(isscalar(data))
    dim = 'Scalar';
    if(iscomplex(data))
      d = ['{',printNumber(real(data)),',',printNumber(imag(data)),'}'];
    else
      d = printNumber(data);
    end
    ret = ['OG',domain,dim,'::create(',d,')'];
  else
    dim = 'DenseMatrix';
    ret = ['OG',domain,dim,'::create(',toCPPlist(data),')'];
  end
end

function [list] = toCPPlist(matrix)
  realspace = true;

  if(~isreal(matrix))
    realspace = false;
  end

  [m,n]=size(matrix);
  str='{';
  for i=1:m
          str=[str,'{'];
          for j=1:(n-1)
            if(realspace)
              str=[str,printNumber(matrix(i,j)),','];
            else
              str=[str,'{',printNumber(real(matrix(i,j))),',',printNumber(imag(matrix(i,j))),'},'];
            end
            % keep line lengths down
            if(mod(j,5)==0 && j~=(n-1))
              str=[str,'\n'];
            end
          end
          % end one doesn't need ","
          if(realspace)
            str=[str,printNumber(matrix(i,n)),','];
          else
            str=[str,'{',printNumber(real(matrix(i,n))),',',printNumber(imag(matrix(i,n))),'}}'];
          end
          str=[str(1:end-1),'},'];
          % keep line lengths down
          if(mod(i,5)==0 && i~=m)
            str=[str,'\n'];
          end
  end
  list=[str(1:end-1),'}'];
  % clean up
  list=strrep (list,'.0000000000000000','.0','overlaps',0);
  list=strrep (list,' ','','overlaps',0);
end


function str = printNumber(number)
  str='';
  if(isnan(number))
          str=[str,'std::numeric_limits<real8>::signaling_NaN()'];
  elseif(isinf(number))
          if(sign(number)==-1)
                  str=[str,'-std::numeric_limits<real8>::infinity()'];
          else
                  str=[str,'std::numeric_limits<real8>::infinity()'];
          end
  else
          str=[str,sprintf('%24.16f',number)];
  end
  str=strrep(str,'.0000000000000000','.0','overlaps',0);
end