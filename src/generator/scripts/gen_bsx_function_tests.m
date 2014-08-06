%
% Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
%
% Please see distribution for license.
%
function [str] = gen_bsx_function_tests(functions)
  dogma_name = functions{1};
  m_name = functions{2};

  real_data = functions{3};

  str="";

  % scalar context
  str=[str, checkunary_impl(m_name,real_data(1),'scalar'),',\n'];

  % row vector
  str=[str, checkunary_impl(m_name,real_data,'row vector'),',\n'];

  % col vector
  str=[str, checkunary_impl(m_name,transpose(real_data),'column vector'),',\n'];

  [factors]=factor(length(real_data));
  if(length(factors)>2)
    mid = floor(length(factors)/2);
    rows = prod(factors(1:mid));
    cols = prod(factors(mid+1:end));
    str=[str, checkunary_impl(m_name,reshape(real_data,rows,cols),'matrix')];
  elseif(length(factors)==2)
    rows = factors(1);
    cols = factors(2);
    str=[str, checkunary_impl(m_name,reshape(real_data,rows,cols),'matrix')];
  else
    % skip, remove ending from col vector impl.
    str=str(1:end-3);
  end


  if(length(functions)==4)
    complex_data = functions{4};
    str=[str,',\n']; % comma appended to end of real functions as we continue with complex
    str=[str, checkunary_impl(m_name,complex_data,'')];
  end

end

function [ret] = runop(op, data)
  ret = eval([op,'(data);']);
end

function [str] = checkunary_impl(op, data, context)
  [ret] = runop(op, data);
  d0=construct(data);
  expected=construct(ret);
  str=['// ',op, '(',context,')\n'];
  str=[str,'new CheckUnary<',upper(op),'>('];
  str=[str, d0,', ',expected,', MATHSEQUAL,\n',...
  '// NOTE: The tolerances used here are unduly slack, this is due to inconsistent libm''s and will be fixed shortly.\n',...
  '1e-13, 1e-13)'];
end