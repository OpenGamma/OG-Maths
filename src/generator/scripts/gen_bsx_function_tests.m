%
% Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
%
% Please see distribution for license.
%

function [str] = gen_bsx_function_tests(functions)
% generates binary singleton expansion style function test code for C++

  dogma_name = functions{1};
  m_name = functions{2};

  real_data = functions{3};

  str='';

  str = [str,generate_callsites(m_name, real_data, 'real')];

  if(length(functions)==4)
    complex_data = functions{4};
    str=[str,',\n']; % comma appended to end of real functions as we continue with complex
    str = [str,generate_callsites(m_name, complex_data, 'complex')];
  end

end


function [str] = generate_callsites(m_name, data, d_type)
  str='';
  % scalar context
  str=[str, checkunary_impl(m_name,data(1),[d_type,' scalar']),',\n'];

  % row vector
  str=[str, checkunary_impl(m_name,data,[d_type,' row vector']),',\n'];

  % col vector
  str=[str, checkunary_impl(m_name,transpose(data),[d_type,' column vector']),',\n'];

  [factors]=factor(length(data));
  if(length(factors)>2)
    mid = floor(length(factors)/2);
    rows = prod(factors(1:mid));
    cols = prod(factors(mid+1:end));
    str=[str, checkunary_impl(m_name,reshape(data,rows,cols),[d_type,' matrix'])];
  elseif(length(factors)==2)
    rows = factors(1);
    cols = factors(2);
    str=[str, checkunary_impl(m_name,reshape(data,rows,cols),[d_type,' matrix'])];
  else
    % skip, remove ending from col vector impl.
    str=str(1:end-3);
  end
end

function [ret] = runop(op, data)
  ret = eval([op,'(data);']);
end

function [str] = checkunary_impl(op, data, context)
  [ret] = runop(op, data);
  d0=construct(data);
  expected=construct(ret);
  str=['// ',op, '(',context,' of dimension: ',num2str(size(data,1)),'x',num2str(size(data,2)),')\n'];
  str=[str,'new CheckUnary<',upper(op),'>('];
  str=[str, d0,', ',expected,', MATHSEQUAL,\n',...
  '// NOTE: The tolerances used here are unduly slack, this is due to inconsistent libm''s and will be fixed shortly.\n',...
  '1e-13, 1e-13)'];
end