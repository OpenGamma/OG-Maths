%
% Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
%
% Please see distribution for license.
%

function [testStr] = gen_bsx_infix_tests(op, data)

testStr='';

%% ops
% scalar/scalar
testStr=[testStr,checkbin_impl(op, data.rs1 , data.rs1),',\n'];
testStr=[testStr,checkbin_impl(op, data.rs1 , data.cs1),',\n'];
testStr=[testStr,checkbin_impl(op, data.cs1 , data.rs1),',\n'];
testStr=[testStr,checkbin_impl(op, data.cs1 , data.cs1),',\n'];

% scalar/vector
testStr=[testStr,checkbin_impl(op, data.rs1 , data.rcv4),',\n'];
testStr=[testStr,checkbin_impl(op, data.rs1 , data.rrv4),',\n'];
testStr=[testStr,checkbin_impl(op, data.cs1 , data.rcv4),',\n'];
testStr=[testStr,checkbin_impl(op, data.cs1 , data.rrv4),',\n'];
testStr=[testStr,checkbin_impl(op, data.rcv4 , data.rs1),',\n'];
testStr=[testStr,checkbin_impl(op, data.rrv4 , data.rs1),',\n'];
testStr=[testStr,checkbin_impl(op, data.rcv4 , data.cs1),',\n'];
testStr=[testStr,checkbin_impl(op, data.rrv4 , data.cs1),',\n'];

testStr=[testStr,checkbin_impl(op, data.rs1 , data.ccv4),',\n'];
testStr=[testStr,checkbin_impl(op, data.rs1 , data.crv4),',\n'];
testStr=[testStr,checkbin_impl(op, data.cs1 , data.ccv4),',\n'];
testStr=[testStr,checkbin_impl(op, data.cs1 , data.crv4),',\n'];
testStr=[testStr,checkbin_impl(op, data.ccv4 , data.rs1),',\n'];
testStr=[testStr,checkbin_impl(op, data.crv4 , data.rs1),',\n'];
testStr=[testStr,checkbin_impl(op, data.ccv4 , data.cs1),',\n'];
testStr=[testStr,checkbin_impl(op, data.crv4 , data.cs1),',\n'];

% scalar/matrix
testStr=[testStr,checkbin_impl(op, data.rs1 , data.r5x4),',\n'];
testStr=[testStr,checkbin_impl(op, data.rs1 , data.r4x5),',\n'];
testStr=[testStr,checkbin_impl(op, data.cs1 , data.r5x4),',\n'];
testStr=[testStr,checkbin_impl(op, data.cs1 , data.r4x5),',\n'];
testStr=[testStr,checkbin_impl(op, data.r5x4 , data.rs1),',\n'];
testStr=[testStr,checkbin_impl(op, data.r4x5 , data.rs1),',\n'];
testStr=[testStr,checkbin_impl(op, data.r5x4 , data.cs1),',\n'];
testStr=[testStr,checkbin_impl(op, data.r4x5 , data.cs1),',\n'];

testStr=[testStr,checkbin_impl(op, data.rs1 , data.c5x4),',\n'];
testStr=[testStr,checkbin_impl(op, data.rs1 , data.c4x5),',\n'];
testStr=[testStr,checkbin_impl(op, data.cs1 , data.c5x4),',\n'];
testStr=[testStr,checkbin_impl(op, data.cs1 , data.c4x5),',\n'];
testStr=[testStr,checkbin_impl(op, data.c5x4 , data.rs1),',\n'];
testStr=[testStr,checkbin_impl(op, data.c4x5 , data.rs1),',\n'];
testStr=[testStr,checkbin_impl(op, data.c5x4 , data.cs1),',\n'];
testStr=[testStr,checkbin_impl(op, data.c4x5 , data.cs1),',\n'];

% matrix/matrix
testStr=[testStr,checkbin_impl(op, data.r5x4 , data.r5x4),',\n'];
testStr=[testStr,checkbin_impl(op, data.r4x5 , data.r4x5),',\n'];
testStr=[testStr,checkbin_impl(op, data.c5x4 , data.c5x4),',\n'];
testStr=[testStr,checkbin_impl(op, data.c4x5 , data.c4x5),'\n'];

end

function [ret] = runop(op, data0, data1)
  ret = eval([op,'(data0, data1);']);
end

function [str] = checkbin_impl(op, data0,data1)
  [ret] = runop(op, data0.val, data1.val);
  d0=construct(data0.val);
  d1=construct(data1.val);
  expected=construct(ret);
  str=['// ',data0.str,' ',op,' ',data1.str,'\n'];
  str=[str,'new CheckBinary<',upper(op),'>('];
  str=[str, d0,', ',d1,', ',expected,', MATHSEQUAL)'];
end


