/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "gtest/gtest.h"
#include "warningmacros.h"
#include "equals.hh"
#include "lapack.hh"
#include "exceptions.hh"

using namespace std;
using namespace librdag;

/**
 * These tests are present to loosly check the *wiring* of LAPACK not LAPACK itself.
 * Our bindings are templated so the tests where applicable will check one real and
 * one complex variant of a routine.
 */

// Some general matrices for input.
real8 r5x4[20] = {1,5,9,13,17,2,6,10,14,18,3,7,11,15,19,4,8,12,16,20};
complex16 c5x4[20] =  {{1,10}, {5,50}, {9,90}, {13,130}, {17,170}, {2,20}, {6,60}, {10,100}, {14,140}, {18,180}, {3,30}, {7,70}, {11,110}, {15,150}, {19,190}, {4,40}, {8,80}, {12,120}, {16,160}, {20,200}};
real8 r4x2[8] = {2,5,11,17,3,7,13,19};
complex16 c4x2[8] = {{2.,20.}, {5.,50.}, {11.,110.}, {17.,170.}, {3.,30.}, {7.,70.}, {13.,130.}, {19.,190.}};
real8 r4x1[4] = {1,2,3,4};
complex16 c4x1[4] = {{1,10},{2,20},{3,30},{4,40}};

// tri upper
real8 rutri4[16] = {1.,0.,0.,0.,2.,6.,0.,0.,3.,7.,11.,0.,4.,8.,12.,16.};
complex16 cutri4[16] = {{1,10}, 0,0,0,{2,20}, {6,60}, 0,0,{3,30}, {7,70}, {11,110}, 0,{4,40}, {8,80}, {12,120}, {16,160}};

// SPD
real8 rspd[16] =  {5,9,10,8,9,17,19,15,10,19,29,21,8,15,21,28};
complex16 cspd[16] = {{ 50,   0}, { 90,  -9}, {100, -10}, { 80,  -8}, { 90,   9}, {170,   0}, {190, -19}, {150, -15}, {100,  10}, {190,  19}, {290,   0}, {210, -21}, { 80,   8}, {150,  15}, {210,  21}, {280,   0}};

// ok cond 5x4
real8 rcondok5x4[20] =  {5,9,10,8,1,9,17,19,15,2,10,19,29,21,3,8,15,21,28,4};
complex16 ccondok5x4[20] = {{5.,-10.}, {9.,-18.}, {10.,-20.}, {8.,-16.}, {1.,-2.}, {9.,-18.}, {17.,-34.}, {19.,-38.}, {15.,-30.}, {2.,-4.}, {10.,-20.}, {19.,-38.}, {29.,-58.}, {21.,-42.}, {3.,-6.}, {8.,-16.}, {15.,-30.}, {21.,-42.}, {28.,-56.}, {4.,-8.}};

// singular 3x3
real8 rsingular3x3[9] = {1,10,1,2,20,2,3,30,3};
complex16 csingular3x3[9] = {{1,10},{10,100},{1,10},{2,20},{20,200},{2,20},{3,30},{30,300},{3,30}};

// Check successful templating of dscal.
TEST(LAPACKTest_xscal, dscal) {
  int4 N = 4;
  real8 alpha = 10.e0;
  real8 * x = new real8[4]{1,2,3,4};
  real8 * expected = new real8[4]{10,20,30,40};
  lapack::xscal(&N, &alpha, x, lapack::ione);
  EXPECT_TRUE(ArrayFuzzyEquals(expected,x,4));

  delete[] x;
  delete[] expected;
}

// Check successful templating of zscal.
TEST(LAPACKTest_xscal, zscal) {
  int4 N = 4;
  complex16 alpha = {2,10};
  complex16 * x = new complex16[4]{{1,10},{2,20},{3,30},{4,40}};
  complex16 * expected = new complex16[4]{{-98.,30.}, {-196.,60.}, {-294.,90.}, {-392.,120.}};
  lapack::xscal(&N, &alpha, x, lapack::ione);
  EXPECT_TRUE(ArrayFuzzyEquals(expected,x,4));

  delete[] x;
  delete[] expected;
}

// Check successful templating of dswap.
TEST(LAPACKTest_xswap, dswap) {
  int4 N = 4;
  real8 * x = new real8[8]{1,2,3,4,5,6,7,8};
  real8 * y = new real8[8]{-1,-2,-3,-4,-5,-6,-7,-8};
  real8 * expectedx = new real8[8]{-1,-2,-3,-4,5,6,7,8};
  real8 * expectedy = new real8[8]{1,2,3,4,-5,-6,-7,-8};

  lapack::xswap(&N, x, lapack::ione, y, lapack::ione);
  EXPECT_TRUE(ArrayFuzzyEquals(expectedx,x,8));
  EXPECT_TRUE(ArrayFuzzyEquals(expectedy,y,8));

  delete[] x;
  delete[] y;
  delete[] expectedx;
  delete[] expectedy;
}

// Check successful templating of zswap.
TEST(LAPACKTest_xswap, zswap) {
  int4 N = 4;
  complex16 * x = new complex16[8]{{1,10},{2,20},{3,30},{4,40},{5,50},{6,60},{7,70},{8,80}};
  complex16 * y = new complex16[8]{{-1,-10},{-2,-20},{-3,-30},{-4,-40},{-5,-50},{-6,-60},{-7,-70},{-8,-80}};
  complex16 * expectedx = new complex16[8]{{-1,-10},{-2,-20},{-3,-30},{-4,-40},{5,50},{6,60},{7,70},{8,80}};
  complex16 * expectedy = new complex16[8]{{1,10},{2,20},{3,30},{4,40},{-5,-50},{-6,-60},{-7,-70},{-8,-80}};

  lapack::xswap(&N, x, lapack::ione, y, lapack::ione);
  EXPECT_TRUE(ArrayFuzzyEquals(expectedx,x,8));
  EXPECT_TRUE(ArrayFuzzyEquals(expectedy,y,8));

  delete[] x;
  delete[] y;
  delete[] expectedx;
  delete[] expectedy;
}

// Check successful templating of dgemv
TEST(LAPACKTest_xgemv, dgemv) {

  int4 m = 5;
  int4 n = 4;

  // copy in A
  real8 * A = new real8[20];
  std::copy(r5x4,r5x4+m*n,A);

  real8 alpha = 2.e0;

  // copy in x
  real8 * x = new real8[4];
  std::copy(r4x1,r4x1+n,x);

  real8 beta = 3.e0;

  real8 y[5] = {-1,-1,-1,-1,-1};

  real8 * expected = new real8[5]{57.,137.,217.,297.,377.};

  //check call
  lapack::xgemv(lapack::N, &m, &n, &alpha, A, &m, x, lapack::ione, &beta, y, lapack::ione);
  EXPECT_TRUE(ArrayFuzzyEquals(expected,y,m));

  delete[] A;
  delete[] x;
  delete[] expected;
}

// Check successful templating of zgemv
TEST(LAPACKTest_xgemv, zgemv) {

  int4 m = 5;
  int4 n = 4;

  // copy in A
  complex16 * A = new complex16[20];
  std::copy(c5x4,c5x4+m*n,A);

  complex16 alpha = {2.e0,3.e0};

  // copy in x
  complex16 * x = new complex16[4];
  std::copy(c4x1,c4x1+n,x);

  complex16 beta = {3.e0,4.e0};

  complex16 y[5] = {{-1,-2},{-1,-2},{-1,-2},{-1,-2},{-1,-2}};

  complex16 * expected = new complex16[5] {{  -7735.,-7720.}, { -18055.,  -18000.}, { -28375.,  -28280.}, { -38695.,  -38560.}, { -49015.,  -48840.}};

  //check call
  lapack::xgemv(lapack::N, &m, &n, &alpha, A, &m, x, lapack::ione, &beta, y, lapack::ione);
  EXPECT_TRUE(ArrayFuzzyEquals(expected,y,m));

  delete[] A;
  delete[] x;
  delete[] expected;
}

// Check successful templating of dgemm
TEST(LAPACKTest_xgemm, dgemm) {

  int4 m = 5;
  int4 k = 4;
  int4 n = 2;

  // copy in A
  real8 * A = new real8[20];
  std::copy(r5x4,r5x4+m*k,A);

  // copy in B
  real8 * B = new real8[8];
  std::copy(r4x2,r4x2+n*k,B);

  real8 alpha = 2.e0;

  real8 beta = 3.e0;

  // create C
  real8 C[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

  real8 * expected = new real8[10]{223.,503.,783.,1063.,1343.,261.,597.,933.,1269.,1605.};

  //check call
  lapack::xgemm(lapack::N,lapack::N, &m, &n, &k, &alpha, A, &m, B, &k, &beta, C, &m);
  EXPECT_TRUE(ArrayFuzzyEquals(expected,C,m*n));

  delete[] A;
  delete[] B;
  delete[] expected;
}

// Check successful templating of dgemm
TEST(LAPACKTest_xgemm, zgemm) {

  int4 m = 5;
  int4 k = 4;
  int4 n = 2;

  // copy in A
  complex16 * A = new complex16[20];
  std::copy(c5x4,c5x4+m*k,A);

  // copy in B
  complex16 * B = new complex16[8];
  std::copy(c4x2,c4x2+n*k,B);

  complex16 alpha = {2.e0,3.e0};

  complex16 beta = {3.e0,4.e0};

  // create C
  complex16 C[10] = {{-1,-2},{-1,-2},{-1,-2},{-1,-2},{-1,-2},{-1,-2},{-1,-2},{-1,-2},{-1,-2},{-1,-2}};

  complex16 * expected = new complex16[10]{{ -29149,  -29051}, { -65269,  -65031}, {-101389, -101011}, {-137509, -136991}, {-173629, -172971}, { -34051,  -33934}, { -77395,  -77110}, {-120739, -120286}, {-164083, -163462}, {-207427, -206638}};

  //check call
  lapack::xgemm(lapack::N,lapack::N, &m, &n, &k, &alpha, A, &m, B, &k, &beta, C, &m);
  EXPECT_TRUE(ArrayFuzzyEquals(expected,C,m*n));

  delete[] A;
  delete[] B;
  delete[] expected;
}

// Check successful templating of dnrm2
TEST(LAPACKTest_xnrm2, dnrm2) {

  int4 n = 4;
  real8 answer = lapack::xnrm2(&n, r4x1, lapack::ione);
  real8 expected = 5.4772255750516612e0;
  EXPECT_TRUE(SingleValueFuzzyEquals(expected,answer));
}

// Check successful templating of znrm2
TEST(LAPACKTest_xnrm2, znrm2) {

  int4 n = 4;
  real8 answer = lapack::xnrm2(&n, c4x1, lapack::ione);
  real8 expected = 55.0454357780915302e0;
  EXPECT_TRUE(SingleValueFuzzyEquals(expected,answer));
}


// Check successful templating of dgesvd.
TEST(LAPACKTest_xgesvd, dgesvd) {
  int4 m = 3;
  int4 n = 2;
  int4 INFO;

  real8* expectedU = new real8[9] {-0.2298476964000714,-0.5247448187602936,-0.8196419411205156,0.8834610176985253,0.2407824921325463,-0.4018960334334317,0.4082482904638627,-0.8164965809277263,0.4082482904638631};
  real8* expectedS = new real8[2] {9.525518091565107,  0.514300580658644};
  real8 * expectedVT = new real8[4]{-0.6196294838293402,-0.7848944532670524,-0.7848944532670524,0.6196294838293402};

  real8* U  = new real8[9]();
  real8* S  = new real8[2]();
  real8* VT = new real8[4]();

  real8 * A = new real8[6] {1,3,5,2,4,6};
  real8 * Acpy = new real8[6];
  std::copy(A,A+m*n,Acpy);

  lapack::xgesvd<real8,lapack::OnInputCheck::isfinite>(lapack::A, lapack::A, &m, &n, Acpy, &m, S, U, &m, VT, &n, &INFO);

  EXPECT_TRUE(ArrayFuzzyEquals(expectedU,U,9,1e-14,1e-14));
  EXPECT_TRUE(ArrayFuzzyEquals(expectedS,S,2,1e-14,1e-14));
  EXPECT_TRUE(ArrayFuzzyEquals(expectedVT,VT,4,1e-14,1e-14));

  // check errors on bad arg (m=-1)
  std::copy(A,A+m*n,Acpy);
  m = -1;
  // will throw as data length is bad for finite domain check
  EXPECT_THROW((lapack::xgesvd<real8,lapack::OnInputCheck::isfinite>(lapack::A, lapack::A, &m, &n, Acpy, &m, S, U, &m, VT, &n, &INFO)), rdag_unrecoverable_error);
  // will throw from xerbla as m is negative
  EXPECT_THROW((lapack::xgesvd<real8,lapack::OnInputCheck::nothing>(lapack::A, lapack::A, &m, &n, Acpy, &m, S, U, &m, VT, &n, &INFO)), rdag_unrecoverable_error);

  // check errors with input checking (A[0] = NaN).
  m = 3;
  std::copy(A,A+m*n,Acpy);
  Acpy[0]=std::numeric_limits<real8>::signaling_NaN();
  // will throw as finite domain check sees NaN
  EXPECT_THROW((lapack::xgesvd<real8,lapack::OnInputCheck::isfinite>(lapack::A, lapack::A, &m, &n, Acpy, &m, S, U, &m, VT, &n, &INFO)), rdag_unrecoverable_error);
  // will not throw
  EXPECT_NO_THROW((lapack::xgesvd<real8,lapack::OnInputCheck::nothing>(lapack::A, lapack::A, &m, &n, Acpy, &m, S, U, &m, VT, &n, &INFO)));

  delete[] U;
  delete[] S;
  delete[] VT;
  delete[] expectedU;
  delete[] expectedS;
  delete[] expectedVT;
  delete[] A;
  delete[] Acpy;
}

// Check successful templating of zgesvd.
TEST(LAPACKTest_xgesvd, zgesvd) {
  int4 m = 3;
  int4 n = 2;
  int4 INFO;

  complex16* expectedU = new complex16[9] {{-0.0228707006002169,-0.2287070060021659}, {-0.0522140610036492,-0.5221406100364927}, {-0.0815574214070819,-0.8155742140708198}, {-0.0879076568710847,-0.8790765687107978}, {-0.0239587534423321,-0.2395875344233279}, {0.0399901499864153, 0.3999014998641418}, {0.3147401422050641, 0.2600102104752862}, {-0.6294802844101258,-0.5200204209505755}, {0.3147401422050625, 0.2600102104752883}};
  real8* expectedS = new real8[2] {95.7302720469661779,5.1686568674896343};
  complex16 * expectedVT = new complex16[4]{{-0.6196294838293402,0},{0.7848944532670524,-0.e0},{-0.7848944532670524,0},{-0.6196294838293402,0}};

  complex16* U  = new complex16[9]();
  real8* S  = new real8[2]();
  complex16* VT = new complex16[4]();

  complex16 * A = new complex16[6]{{1,10}, {3,30}, {5,50}, {2,20}, {4,40}, {6,60}};
  complex16 * Acpy = new complex16[6];
  std::copy(A,A+m*n,Acpy);

  lapack::xgesvd<complex16,lapack::OnInputCheck::isfinite>(lapack::A, lapack::A, &m, &n, Acpy, &m, S, U, &m, VT, &n, &INFO);

  EXPECT_TRUE(ArrayFuzzyEquals(expectedU,U,9,1e-14,1e-14));
  EXPECT_TRUE(ArrayFuzzyEquals(expectedS,S,2,1e-14,1e-14));
  EXPECT_TRUE(ArrayFuzzyEquals(expectedVT,VT,4,1e-14,1e-14));

  // check throw on bad arg
  std::copy(A,A+m*n,Acpy);
  m = -1;
  EXPECT_THROW((lapack::xgesvd<complex16,lapack::OnInputCheck::isfinite>(lapack::A, lapack::A, &m, &n, Acpy, &m, S, U, &m, VT, &n, &INFO)), rdag_unrecoverable_error);

  delete[] U;
  delete[] S;
  delete[] VT;
  delete[] expectedU;
  delete[] expectedS;
  delete[] expectedVT;
  delete[] A;
  delete[] Acpy;
}


// Check successful templating of dgetrf.
TEST(LAPACKTest_xgetrf, dgetrf) {
  int4 m = 5;
  int4 n = 4;
  const int4 minmn = m > n ? n : m;
  int4 INFO = 0;

  real8 * expectedA = new real8[20]{10.,0.5,0.9,0.8,0.1,19.,-0.5,0.2000000000000028,0.4000000000000021,-0.1999999999999997,29.,-4.5,-6.1999999999999886,0.0645161290322571,0.1290322580645162,21.,-2.5,-3.3999999999999950,12.4193548387096779,0.1480519480519481};

  int4 * expectedIPIV = new int4[4]{3,3,3,4};

  real8 * Acpy = new real8[20];
  std::copy(rcondok5x4,rcondok5x4+m*n,Acpy);
  int4 * ipiv = new int4[minmn]();
  lapack::xgetrf<real8, lapack::OnInputCheck::nothing>(&m,&n,Acpy,&m,ipiv,&INFO);

  EXPECT_TRUE(ArrayBitEquals(expectedIPIV,ipiv,4));
  EXPECT_TRUE(ArrayFuzzyEquals(expectedA,Acpy,20));

  // check throw on bad arg
  std::copy(rcondok5x4,rcondok5x4+m*n,Acpy);
  m = -1;
  EXPECT_THROW((lapack::xgetrf<real8, lapack::OnInputCheck::nothing>(&m,&n,Acpy,&m,ipiv,&INFO)), rdag_error);

  // check throw on singular
  m=3;
  n=3;
  std::copy(rsingular3x3,rsingular3x3+m*n,Acpy);
  EXPECT_THROW((lapack::xgetrf<real8, lapack::OnInputCheck::nothing>(&m,&n,Acpy,&m,ipiv,&INFO)), rdag_error);

  delete [] ipiv;
  delete [] expectedIPIV;
  delete [] Acpy;
  delete [] expectedA;
}

// Check successful templating of zgetrf.
TEST(LAPACKTest_xgetrf, zgetrf) {
  int4 m = 5;
  int4 n = 4;
  const int4 minmn = m > n ? n : m;
  int4 INFO = 0;

  complex16 * expectedA = new complex16[20] {{10.,-20.}, { 0.5, 0.}, { 0.8999999999999999,  0.}, { 0.8, 0.}, { 0.1,  0.}, {19.,-38.}, {-0.5,  1.}, { 0.1999999999999957,  0.}, { 0.4000000000000021,  0.}, {-0.1999999999999998,  0.}, {29.,-58.}, {-4.5,  9.}, {-6.2000000000000171, 12.4000000000000341}, { 0.0645161290322568,  0.}, { 0.1290322580645157,  0.}, {21.,-42.}, {-2.5000000000000000,  5.}, {-3.4000000000000092,  6.8000000000000185}, {12.4193548387096779,-24.8387096774193559}, { 0.1480519480519480,  0.}};

  int4 * expectedIPIV = new int4[4]{3,3,3,4};

  complex16 * Acpy = new complex16[20];
  std::copy(ccondok5x4,ccondok5x4+m*n,Acpy);
  int4 * ipiv = new int4[minmn]();
  lapack::xgetrf<complex16, lapack::OnInputCheck::nothing>(&m,&n,Acpy,&m,ipiv,&INFO);

  EXPECT_TRUE(ArrayBitEquals(expectedIPIV,ipiv,4));
  EXPECT_TRUE(ArrayFuzzyEquals(expectedA,Acpy,20));

  // check throw on bad arg
  std::copy(rcondok5x4,rcondok5x4+m*n,Acpy);
  m = -1;
  EXPECT_THROW((lapack::xgetrf<complex16, lapack::OnInputCheck::nothing>(&m,&n,Acpy,&m,ipiv,&INFO)), rdag_error);

  // check throw on singular
  m=3;
  n=3;
  std::copy(csingular3x3,csingular3x3+m*n,Acpy);
  EXPECT_THROW((lapack::xgetrf<complex16, lapack::OnInputCheck::nothing>(&m,&n,Acpy,&m,ipiv,&INFO)), rdag_error);

  delete [] ipiv;
  delete [] expectedIPIV;
  delete [] Acpy;
  delete [] expectedA;
}

// Check successful templating of dtrcon.
TEST(LAPACKTest_xtrcon, dtrcon) {
  int4 n = 4;
  real8 rcond = 0;
  int4 INFO = 0;
  lapack::xtrcon(lapack::ONE, lapack::U, lapack::N, &n, rutri4, &n, &rcond, &INFO );
  real8 expected = 0.025e0;
  EXPECT_TRUE(SingleValueFuzzyEquals(expected,rcond));

  // check throw on bad arg (first throw)
  n=-1;
  EXPECT_THROW(lapack::xtrcon(lapack::ONE, lapack::U, lapack::N, &n, rutri4, &n, &rcond, &INFO ), rdag_error);

  // check throw on bad arg (second throw)
  n=4;
  int4 lda = -1;
  EXPECT_THROW(lapack::xtrcon(lapack::ONE, lapack::U, lapack::N, &n, rutri4, &lda, &rcond, &INFO ), rdag_error);

}

// Check successful templating of ztrcon.
TEST(LAPACKTest_xtrcon, ztrcon) {
  int4 n = 4;
  real8 rcond = 0;
  int4 INFO = 0;
  lapack::xtrcon(lapack::ONE, lapack::U, lapack::N, &n, cutri4, &n, &rcond, &INFO );
  real8 expected = 0.025e0;
  EXPECT_TRUE(SingleValueFuzzyEquals(expected,rcond));

  // check throw on bad arg (first throw)
  n=-1;
  EXPECT_THROW(lapack::xtrcon(lapack::ONE, lapack::U, lapack::N, &n, cutri4, &n, &rcond, &INFO ), rdag_error);

  // check throw on bad arg (second throw)
  n=4;
  int4 lda = -1;
  EXPECT_THROW(lapack::xtrcon(lapack::ONE, lapack::U, lapack::N, &n, cutri4, &lda, &rcond, &INFO ), rdag_error);
}

// Check successful templating of dtrtrs.
TEST(LAPACKTest_xtrtrs, dtrtrs) {
  int4 n = 4;
  real8 * RHS = new real8[4]{10,8,2,-1};
  int4 INFO = 0;
  lapack::xtrtrs(lapack::U, lapack::N, lapack::N, &n, lapack::ione, rutri4, &n, RHS, &n, &INFO );
  real8 expected[4] = {7.25,1.125,0.25,-0.0625};
  EXPECT_TRUE(ArrayFuzzyEquals(expected,RHS,n));

  // check throw on singluar matrix
  real8 * A = new real8[16];
  std::copy(rutri4,rutri4+n*n,A);
  A[5] = 0.e0; // 2nd diagonal
  EXPECT_THROW(lapack::xtrtrs(lapack::U, lapack::N, lapack::N, &n, lapack::ione, A, &n, RHS, &n, &INFO ), rdag_error);

  // check throw on bad arg
  n=-1;
  EXPECT_THROW(lapack::xtrtrs(lapack::U, lapack::N, lapack::N, &n, lapack::ione, rutri4, &n, RHS, &n, &INFO ), rdag_error);
  delete [] RHS;
  delete [] A;
}

// Check successful templating of ztrtrs.
TEST(LAPACKTest_xtrtrs, ztrtrs) {
  int4 n = 4;
  complex16 * RHS = new complex16[4]{{10.,-2.}, {-7.,3.}, {3.,-6.}, {-2.,-5.}};
  int4 INFO = 0;
  lapack::xtrtrs(lapack::U, lapack::N, lapack::N, &n, lapack::ione, cutri4, &n, RHS, &n, &INFO );
  complex16 expected[4] = {{-0.1212121212121212,-1.2348484848484849}, {0.0997599759975998, 0.1577032703270327}, {-0.0162016201620162,-0.0425292529252925}, {-0.0321782178217822, 0.0092821782178218}};
  EXPECT_TRUE(ArrayFuzzyEquals(expected,RHS,n));

  // check throw on singluar matrix
  complex16 * A = new complex16[16];
  std::copy(cutri4,cutri4+n*n,A);
  A[5] = 0.e0; // 2nd diagonal
  EXPECT_THROW(lapack::xtrtrs(lapack::U, lapack::N, lapack::N, &n, lapack::ione, A, &n, RHS, &n, &INFO ), rdag_error);

  // check throw on bad arg
  n=-1;
  EXPECT_THROW(lapack::xtrtrs(lapack::U, lapack::N, lapack::N, &n, lapack::ione, cutri4, &n, RHS, &n, &INFO ), rdag_error);
  delete [] RHS;
  delete [] A;
}


// Check successful templating of dgetri.
TEST(LAPACKTest_xgetri, dgetri) {
  int4 n = 4;
  int4 INFO = 0;

  // this is the decomp of rcondok5x4[1:16] from dgetrf
  real8 * A = new real8[16]{10,0.5000000000000000,0.9000000000000000,0.8000000000000000,19,-0.5000000000000000,0.2000000000000028,0.4000000000000021,29,-4.5000000000000000,-6.1999999999999886,0.0645161290322571,21,-2.5000000000000000,-3.3999999999999950,12.4193548387096779};

  real8 * expectedA = new real8[16]{4.2701298701298649,-2.2883116883116852,0.0493506493506489,-0.0311688311688309,-2.2883116883116852,1.4519480519480503,-0.1584415584415583,-0.0051948051948053,0.0493506493506489,-0.1584415584415583,0.1532467532467533,-0.0441558441558442,-0.0311688311688309,-0.0051948051948053,-0.0441558441558442,0.0805194805194805};

  // this is the decomp of rsingular3x3 from dgetrf
  real8 * Asingular = new real8[9] {10., 0.1, 0.10000000000000001, 20., -0., -0., 30.,-0.,-0.};

  int4 * ipiv = new int4[4]{3,3,3,4};

  real8 * Acpy = new real8[16];
  std::copy(A,A+n*n,Acpy);
  lapack::xgetri(&n,Acpy,&n,ipiv,&INFO);

  EXPECT_TRUE(ArrayFuzzyEquals(expectedA,Acpy,16,1e-14,1e-14));

  // check throw on bad arg
  std::copy(A,A+n*n,Acpy);
  n = -1;
  EXPECT_THROW(lapack::xgetri(&n,Acpy,&n,ipiv,&INFO), rdag_error);

  // check throw on singular
  n=3;
  ipiv[0]=2;ipiv[1]=2;ipiv[2]=3;
  std::copy(Asingular,Asingular+n*n,Acpy);
  EXPECT_THROW(lapack::xgetri(&n,Acpy,&n,ipiv,&INFO), rdag_error);

  delete [] ipiv;
  delete [] A;
  delete [] Acpy;
  delete [] expectedA;
  delete [] Asingular;
}

// Check successful templating of zgetri.
TEST(LAPACKTest_xgetri, zgetri) {
  int4 n = 4;
  int4 INFO = 0;

  // this is the decomp of ccondok5x4[1:16] from zgetrf
  complex16 * A = new complex16[16]{{10,-20}, {0.5,0}, {0.8999999999999999,0}, {      0.8,0}, {19,-38}, {-0.5,1}, {0.1999999999999957,0}, {0.4000000000000021,0}, {29,-58}, {     -4.5,9}, {     -6.2000000000000171,12.4000000000000341}, {0.0645161290322568,0}, {21,-42}, {-2.5,5}, {     -3.4000000000000092,6.8000000000000185}, {12.4193548387096779,-24.8387096774193559}};


  complex16 * expectedA = new complex16[16]{{0.8540259740259719, 1.7080519480519438}, {-0.4576623376623363,-0.9153246753246725}, {0.0098701298701297, 0.0197402597402595}, {-0.0062337662337663,-0.0124675324675326}, {-0.4576623376623364,-0.9153246753246728}, {0.2903896103896096, 0.5807792207792192}, {-0.0316883116883116,-0.0633766233766232}, {-0.0010389610389610,-0.0020779220779220}, {0.0098701298701298, 0.0197402597402595}, {-0.0316883116883116,-0.0633766233766233}, {0.0306493506493506, 0.0612987012987013}, {-0.0088311688311688,-0.0176623376623377}, {-0.0062337662337662,-0.0124675324675325}, {-0.0010389610389610,-0.0020779220779221}, {-0.0088311688311688,-0.0176623376623377}, {0.0161038961038961, 0.0322077922077922}};

  // this is the decomp of csingular3x3 from zgetrf
  complex16 * Asingular = new complex16[9] {{10.,100.},{0.1,0.1},{20.,200.},{-0.,-0.},{30.,300.}, 0.,0.};

  int4 * ipiv = new int4[4]{3,3,3,4};

  complex16 * Acpy = new complex16[16];
  std::copy(A,A+n*n,Acpy);
  lapack::xgetri(&n,Acpy,&n,ipiv,&INFO);

  EXPECT_TRUE(ArrayFuzzyEquals(expectedA,Acpy,16,1e-14,1e-14));

  // check throw on bad arg
  std::copy(A,A+n*n,Acpy);
  n = -1;
  EXPECT_THROW(lapack::xgetri(&n,Acpy,&n,ipiv,&INFO), rdag_error);

  // check throw on singular
  n=3;
  ipiv[0]=2;ipiv[1]=2;ipiv[2]=3;
  std::copy(Asingular,Asingular+n*n,Acpy);
  EXPECT_THROW(lapack::xgetri(&n,Acpy,&n,ipiv,&INFO), rdag_error);

  delete [] ipiv;
  delete [] A;
  delete [] Acpy;
  delete [] expectedA;
  delete [] Asingular;
}



// Check successful templating of dpotrf.
TEST(LAPACKTest_xpotrf, dpotrf) {
  int4 n=4;
  int4 INFO = 0;
  real8 * A = new real8[16];

  std::copy(rspd,rspd+n*n,A);
  lapack::xpotrf(lapack::U, &n, A, &n, &INFO);

  real8 * expected = new real8[16]{2.2360679774997898,9,10,8,4.0249223594996213,0.8944271909999163,19,15,4.4721359549995796,1.1180339887498942,2.7838821814150103,21,3.5777087639996634,0.6708203932499381,1.5266450672275866,3.5241105031922135};
  EXPECT_TRUE(ArrayFuzzyEquals(expected,A,n*n));

  // check throw on non SPD
  std::copy(rspd,rspd+n*n,A);
  A[9] = 400;
  EXPECT_THROW(lapack::xpotrf(lapack::U, &n, A, &n, &INFO), rdag_error);
  EXPECT_TRUE(INFO==3);

  // check throw on bad arg
  std::copy(rspd,rspd+n*n,A);
  n = -1;
  EXPECT_THROW(lapack::xpotrf(lapack::U, &n, A, &n, &INFO), rdag_error);


  delete[] A;
  delete[] expected;
}

// Check successful templating of zpotrf.
TEST(LAPACKTest_xpotrf, zpotrf) {
  int4 n=4;
  int4 INFO = 0;
  complex16 * A = new complex16[16];

  std::copy(cspd,cspd+n*n,A);
  lapack::xpotrf(lapack::U, &n, A, &n, &INFO);

  complex16 * expected = new complex16[16]{{  7.0710678118654755,   0}, { 90,   -9}, {100,  -10}, { 80,   -8}, { 12.7279220613578552,   1.2727922061357855}, {  2.5258661880630169,   0}, {190,  -19}, {150,  -15}, { 14.1421356237309492,   1.4142135623730949}, {  3.2464110881060808,   7.5221720334165125}, {  4.5692168854967257,   0}, {210,  -21}, { 11.3137084989847594,   1.1313708498984760}, {  1.8053212880199752,   5.9385568684867200}, { -0.4665266489696448,   3.3487008435600085}, { 10.0380731673420662,   0}};
  EXPECT_TRUE(ArrayFuzzyEquals(expected,A,n*n,1e-14,1e-14));

  // check throw on non SPD
  std::copy(cspd,cspd+n*n,A);
  A[9] = 400;
  EXPECT_THROW(lapack::xpotrf(lapack::U, &n, A, &n, &INFO), rdag_error);
  EXPECT_TRUE(INFO==3);

  // check throw on bad arg
  std::copy(cspd,cspd+n*n,A);
  n = -1;
  EXPECT_THROW(lapack::xpotrf(lapack::U, &n, A, &n, &INFO), rdag_error);


  delete[] A;
  delete[] expected;
}

// Check successful templating of dpocon.
TEST(LAPACKTest_xpocon, dpocon) {
  int4 n = 4;
  int4 INFO = 0;
  real8 * A = new real8[16];
  std::copy(rspd,rspd+n*n,A);
  real8 ANORM = lapack::xlansy(lapack::ONE, lapack::U, &n, A, &n);
  lapack::xpotrf(lapack::U, &n, A, &n, &INFO);
  real8 RCOND = 0;
  lapack::xpocon(lapack::U, &n, A, &n, &ANORM, &RCOND, &INFO);
  real8 anorm_expected = 79e0;
  EXPECT_TRUE(ANORM==anorm_expected);
  real8 rcond_expected = 0.00190665795051604e0;
  EXPECT_TRUE(SingleValueFuzzyEquals(rcond_expected, RCOND));

  // check throw on bad arg (first catch to prevent bad alloc)
  n = -1;
  EXPECT_THROW(lapack::xpocon(lapack::U, &n, A, &n, &ANORM, &RCOND, &INFO), rdag_error);

  // check throw on bad arg (second catch, bad input elsewhere)
  n = 4;
  char kew = 'Q';
  EXPECT_THROW(lapack::xpocon(&kew, &n, A, &n, &ANORM, &RCOND, &INFO), rdag_error);

  delete [] A;
}


// Check successful templating of zpocon.
TEST(LAPACKTest_xpocon, zpocon) {
  int4 n = 4;
  int4 INFO = 0;
  complex16 * A = new complex16[16];
  std::copy(cspd,cspd+n*n,A);
  real8 ANORM = lapack::zlanhe(lapack::ONE, lapack::U, &n, A, &n);
  lapack::xpotrf(lapack::U, &n, A, &n, &INFO);
  real8 RCOND = 0;
  lapack::xpocon(lapack::U, &n, A, &n, &ANORM, &RCOND, &INFO);
  real8 anorm_expected = 792.493781056045;
  EXPECT_TRUE(SingleValueFuzzyEquals(ANORM,anorm_expected));
  real8 rcond_expected = 3.21351909155511e-04;
  EXPECT_TRUE(SingleValueFuzzyEquals(rcond_expected, RCOND));

  // check throw on bad arg (first catch to prevent bad alloc)
  n = -1;
  EXPECT_THROW(lapack::xpocon(lapack::U, &n, A, &n, &ANORM, &RCOND, &INFO), rdag_error);

  // check throw on bad arg (second catch, bad input elsewhere)
  n = 4;
  char kew = 'Q';
  EXPECT_THROW(lapack::xpocon(&kew, &n, A, &n, &ANORM, &RCOND, &INFO), rdag_error);

  delete [] A;
}

TEST(LAPACKTest_xlansy, dlansy) {
  int4 n = 4;
  real8 NORM = lapack::xlansy(lapack::ONE, lapack::U, &n, rspd, &n);
  real8 expected = 79;
  EXPECT_TRUE(SingleValueFuzzyEquals(expected, NORM));

  // check throw on bad arg (first catch to prevent bad alloc)
  n = -1;
  EXPECT_THROW(lapack::xlansy(lapack::ONE, lapack::U, &n, rspd, &n), rdag_error);
}

TEST(LAPACKTest_xlansy, zlansy) {
  int4 n = 4;
  // cpsd is Hermitian, but the upper triangle only is addressed so we
  // can pretend it's symmetric.
  real8 NORM = lapack::xlansy(lapack::ONE, lapack::U, &n, cspd, &n);
  real8 expected = 792.493781056045e0;
  EXPECT_TRUE(SingleValueFuzzyEquals(expected, NORM));

  // check throw on bad arg (first catch to prevent bad alloc)
  n = -1;
  EXPECT_THROW(lapack::xlansy(lapack::ONE, lapack::U, &n, cspd, &n), rdag_error);
}


TEST(LAPACKTest_xpotrs, dpotrs) {
  int4 n = 4;
  int4 INFO;
  real8 * A = new real8[16];
  std::copy(rspd,rspd+n*n,A);
  lapack::xpotrf(lapack::U, &n, A, &n, &INFO);
  real8 * RHS = new real8[4];
  std::copy(r4x1, r4x1+n, RHS);
  lapack::xpotrs(lapack::U, &n, lapack::ione, A, &n, RHS, &n, &INFO);
  real8 expected[4] = {-0.2831168831168828,0.1194805194805193,0.0155844155844156,0.1480519480519481};
  EXPECT_TRUE(ArrayFuzzyEquals(expected,RHS,n));

  // check throw on bad arg
  n=-1;
  EXPECT_THROW(lapack::xpotrs(lapack::U, &n, lapack::ione, A, &n, RHS, &n, &INFO), rdag_error);

  delete [] A;
  delete [] RHS;
}

TEST(LAPACKTest_xpotrs, zpotrs) {
  int4 n = 4;
  int4 INFO;
  complex16 * A = new complex16[16];
  std::copy(cspd,cspd+n*n,A);
  lapack::xpotrf(lapack::U, &n, A, &n, &INFO);
  complex16 * RHS = new complex16[4];
  std::copy(c4x1, c4x1+n, RHS);
  lapack::xpotrs(lapack::U, &n, lapack::ione, A, &n, RHS, &n, &INFO);
  complex16 expected[4] =  {{-1.0063762794655238,-1.5989977364878503}, {0.8869421128000980,0.5966984611395660}, {-0.1560914405265542,0.1826679640758723}, {-0.0562376225820731,0.1501070660620698}};
  EXPECT_TRUE(ArrayFuzzyEquals(expected,RHS,n, 1e-14, 1e-14));
  // check throw on bad arg
  n=-1;
  EXPECT_THROW(lapack::xpotrs(lapack::U, &n, lapack::ione, A, &n, RHS, &n, &INFO), rdag_error);

  delete [] A;
  delete [] RHS;
}


TEST(LAPACKTest_xlange, dlange) {
  int4 n = 4;
  real8 * A = new real8[16];
  std::copy(rspd,rspd+n*n,A);
  real8 answer = lapack::xlange(lapack::O, &n, &n, A, &n);
  EXPECT_TRUE(SingleValueFuzzyEquals(answer, 79.e0));
  // check throw on bad arg
  n=-1;
  EXPECT_THROW(lapack::xlange(lapack::O, &n, &n, A, &n), rdag_error);
  delete[] A;
}

TEST(LAPACKTest_xlange, zlange) {
  int4 n = 4;
  complex16 * A = new complex16[16];
  std::copy(cspd,cspd+n*n,A);
  real8 answer = lapack::xlange(lapack::O, &n, &n, A, &n);
  EXPECT_TRUE(SingleValueFuzzyEquals(answer,792.493781056045));
  // check throw on bad arg
  n=-1;
  EXPECT_THROW(lapack::xlange(lapack::O, &n, &n, A, &n), rdag_error);
  delete[] A;
}

TEST(LAPACKTest_xgecon, dgecon) {
  int4 m = 5;
  int4 n = 4;
  const int4 minmn = m > n ? n : m;
  int4 INFO = 0;

  real8 * Acpy = new real8[20];
  std::copy(rcondok5x4,rcondok5x4+m*n,Acpy);

  // need a 1 norm
  real8 anorm = lapack::xlange(lapack::O, &m, &n, Acpy, &m);
  // check it
  EXPECT_TRUE(SingleValueFuzzyEquals(anorm, 82.e0));

  // need a LU decomp
  int4 * ipiv = new int4[minmn]();
  lapack::xgetrf<real8, lapack::OnInputCheck::nothing>(&m,&n,Acpy,&m,ipiv,&INFO);

  // make the call
  real8 rcond = 0;
  lapack::xgecon(lapack::O, &n, Acpy, &m, &anorm, &rcond, &INFO);
  EXPECT_TRUE(SingleValueFuzzyEquals(rcond,1.8369021718386168e-3));

  // check throw on bad arg, malloc guard test
  n=-1;
  EXPECT_THROW(lapack::xgecon(lapack::O, &n, Acpy, &m, &anorm, &rcond, &INFO), rdag_error);
  // xerbla test
  n=5; m=-1;
  EXPECT_THROW(lapack::xgecon(lapack::O, &n, Acpy, &m, &anorm, &rcond, &INFO), rdag_error);

  // clean up
  delete [] ipiv;
  delete [] Acpy;
}


TEST(LAPACKTest_xgecon, zgecon) {
  int4 m = 5;
  int4 n = 4;
  const int4 minmn = m > n ? n : m;
  int4 INFO = 0;

  complex16 * Acpy = new complex16[20];
  std::copy(ccondok5x4,ccondok5x4+m*n,Acpy);

  // need a 1 norm
  real8 anorm = lapack::xlange(lapack::O, &m, &n, Acpy, &m);

  // check it
  EXPECT_TRUE(SingleValueFuzzyEquals(anorm,183.35757415498276e0));

  // need a LU decomp
  int4 * ipiv = new int4[minmn]();
  lapack::xgetrf<complex16, lapack::OnInputCheck::nothing>(&m,&n,Acpy,&m,ipiv,&INFO);

  // make the call
  real8 rcond = 0;
  lapack::xgecon(lapack::O, &n, Acpy, &m, &anorm, &rcond, &INFO);
  EXPECT_TRUE(SingleValueFuzzyEquals(rcond,1.8369021718386248e-3));

  // check throw on bad arg, malloc guard test
  n=-1;
  EXPECT_THROW(lapack::xgecon(lapack::O, &n, Acpy, &m, &anorm, &rcond, &INFO), rdag_error);
  // xerbla test
  n=5; m=-1;
  EXPECT_THROW(lapack::xgecon(lapack::O, &n, Acpy, &m, &anorm, &rcond, &INFO), rdag_error);
  // clean up
  delete [] ipiv;
  delete [] Acpy;
}


TEST(LAPACKTest_xgetrs, dgetrs) {
  int4 m = 5;
  int4 n = 4;
  const int4 minmn = m > n ? n : m;
  int4 INFO = 0;

  real8 * Acpy = new real8[20];
  std::copy(rcondok5x4,rcondok5x4+m*n,Acpy);

  // need a LU decomp
  int4 * ipiv = new int4[minmn]();
  lapack::xgetrf<real8, lapack::OnInputCheck::nothing>(&m,&n,Acpy,&m,ipiv,&INFO);

  // need an RHS
  real8 * rhs = new real8[8]{1,2,3,4,1,2,3,4};

  // expected
  real8 * expected = new real8[8]{-0.2831168831168828,0.1194805194805193,0.0155844155844156,0.1480519480519481,
   -0.2831168831168828,0.1194805194805193,0.0155844155844156,0.1480519480519481
  };

  // make the call
  int4 two = 2;
  lapack::xgetrs(lapack::N, &n, &two, Acpy, &m, ipiv, rhs, &n, &INFO);

  EXPECT_TRUE(ArrayFuzzyEquals(expected,rhs,8,1e-14,1e-14));

  // xerbla test
  n=-1;
  EXPECT_THROW(lapack::xgetrs(lapack::N, &n, &two, Acpy, &m, ipiv, rhs, &n, &INFO),rdag_error);

  // clean up
  delete [] ipiv;
  delete [] Acpy;
  delete [] expected;
  delete [] rhs;
}

TEST(LAPACKTest_xgetrs, zgetrs) {
  int4 m = 5;
  int4 n = 4;
  const int4 minmn = m > n ? n : m;
  int4 INFO = 0;

  complex16 * Acpy = new complex16[20];
  std::copy(ccondok5x4,ccondok5x4+m*n,Acpy);

  // need a LU decomp
  int4 * ipiv = new int4[minmn]();
  lapack::xgetrf<complex16, lapack::OnInputCheck::nothing>(&m,&n,Acpy,&m,ipiv,&INFO);

  // need an RHS
  complex16 * rhs = new complex16[8]{{1.,10.}, {2.,20.}, {3.,30.}, {4.,40.},{1.,10.}, {2.,20.}, {3.,30.}, {4.,40.}};

  // expected
  complex16 * expected = new complex16[8]{{      1.0758441558441547,      -0.6794805194805187}, {     -0.4540259740259733,       0.2867532467532462}, {     -0.0592207792207793,       0.0374025974025975}, {     -0.5625974025974027,       0.3553246753246754}, {      1.0758441558441547,      -0.6794805194805187}, {     -0.4540259740259733,       0.2867532467532462}, {     -0.0592207792207793,       0.0374025974025975}, {     -0.5625974025974027,       0.3553246753246754}};

  // make the call
  int4 two = 2;
  lapack::xgetrs(lapack::N, &n, &two, Acpy, &m, ipiv, rhs, &n, &INFO);

  EXPECT_TRUE(ArrayFuzzyEquals(expected,rhs,8,1e-14,1e-14));

  // xerbla test
  n=-1;
  EXPECT_THROW(lapack::xgetrs(lapack::N, &n, &two, Acpy, &m, ipiv, rhs, &n, &INFO),rdag_error);

  // clean up
  delete [] ipiv;
  delete [] Acpy;
  delete [] expected;
  delete [] rhs;
}

TEST(LAPACKTest_xgels, dgels) {
  int4 m = 5;
  int4 n = 4;
  int4 INFO = 0;

  real8 * Acpy = new real8[20];
  std::copy(rcondok5x4,rcondok5x4+m*n,Acpy);

  // need an RHS
  real8 * rhs = new real8[10]{1,2,3,4,5,1,2,3,4,5};

  // expected
  real8 * expected = new real8[8]{-6.1464892723569555,3.3470608493900018,-0.1305926637544035,0.2247336970641731,-6.1464892723569555,3.3470608493900018,-0.1305926637544035,0.2247336970641731};

  // make the call
  int4 two = 2;
  lapack::xgels(lapack::N, &m, &n, &two, Acpy, &m, rhs, &m, &INFO);

  // answers are striped in "n" length columns in RHS
  EXPECT_TRUE(ArrayFuzzyEquals(expected,rhs,n,1e-13,1e-13));
  EXPECT_TRUE(ArrayFuzzyEquals(&expected[4],&rhs[5],n,1e-13,1e-13));

  // xerbla test
  n=-1;
  EXPECT_THROW(lapack::xgels(lapack::N, &m, &n, &two, Acpy, &m, rhs, &m, &INFO),rdag_error);

  // clean up
  delete [] Acpy;
  delete [] expected;
  delete [] rhs;
}


TEST(LAPACKTest_xgels, zgels) {
  int4 m = 5;
  int4 n = 4;
  int4 INFO = 0;

  complex16 * Acpy = new complex16[20];
  std::copy(ccondok5x4,ccondok5x4+m*n,Acpy);

  // need an RHS
  complex16 * rhs = new complex16[10]{{1.,10.}, {2.,20.}, {3.,30.}, {4.,40.}, {5., 50.},{1.,10.}, {2.,20.}, {3.,30.}, {4.,40.}, {5.,50.}};

  // expected
  complex16 * expected = new complex16[8]{{     23.3566592349563571,     -14.7515742536566901}, {    -12.7188312276819353,       8.0329460385359930}, {      0.4962521222667151,      -0.3134223930105628}, {     -0.8539880488438545,       0.5393608729540187}, {     23.3566592349563571,     -14.7515742536566901}, {    -12.7188312276819353,       8.0329460385359930}, {      0.4962521222667151,      -0.3134223930105628}, {     -0.8539880488438545,       0.5393608729540187}};

  // make the call
  int4 two = 2;
  lapack::xgels(lapack::N, &m, &n, &two, Acpy, &m, rhs, &m, &INFO);

  // answers are striped in "n" length columns in RHS
  EXPECT_TRUE(ArrayFuzzyEquals(expected,rhs,n,1e-13,1e-13));
  EXPECT_TRUE(ArrayFuzzyEquals(&expected[4],&rhs[5],n,1e-13,1e-13));

  // xerbla test
  n=-1;
  EXPECT_THROW(lapack::xgels(lapack::N, &m, &n, &two, Acpy, &m, rhs, &m, &INFO),rdag_error);

  // clean up
  delete [] Acpy;
  delete [] expected;
  delete [] rhs;
}


TEST(LAPACKTest_xgelsd, dgelsd) {
  int4 m = 5;
  int4 n = 4;
  int4 INFO = 0;
  int4 minmn = m > n ? n : m;

  real8 * Acpy = new real8[20];
  std::copy(rcondok5x4,rcondok5x4+m*n,Acpy);

  // need an RHS
  real8 * rhs = new real8[10]{1,2,3,4,5,1,2,3,4,5};

  // expected
  real8 * expected = new real8[8]{-6.1464892723569555,3.3470608493900018,-0.1305926637544035,0.2247336970641731,-6.1464892723569555,3.3470608493900018,-0.1305926637544035,0.2247336970641731};
  real8 * expected_S = new real8[4] {66.4703325718839153,9.0396484167794604,3.5978090226222377,0.1881874621350516};

  // need somewhere to write S
  real8 * S = new real8[minmn];
  // condition and rank
  real8 RCOND = - 1; // -1 so rank is computed wrt machine precision
  int4 RANK;

  // make the call
  int4 two = 2;
  lapack::xgelsd(&m, &n, &two, Acpy, &m, rhs, &m, S, &RCOND, &RANK, &INFO);

  // answers are striped in "n" length columns in RHS
  EXPECT_TRUE(ArrayFuzzyEquals(expected,rhs,n,1e-13,1e-13));
  EXPECT_TRUE(ArrayFuzzyEquals(&expected[4],&rhs[5],n,1e-13,1e-13));

  EXPECT_TRUE(ArrayFuzzyEquals(expected_S,S,minmn,1e-13,1e-13));

  EXPECT_EQ(RANK, n);

  // xerbla test
  n=-1;
  EXPECT_THROW(lapack::xgelsd(&m, &n, &two, Acpy, &m, rhs, &m, S, &RCOND, &RANK, &INFO),rdag_error);


  // clean up
  delete [] Acpy;
  delete [] expected;
  delete [] rhs;
  delete [] S;
  delete [] expected_S;
}


TEST(LAPACKTest_xgelsd, zgelsd) {
  int4 m = 5;
  int4 n = 4;
  int4 INFO = 0;
  int4 minmn = m > n ? n : m;

  complex16 * Acpy = new complex16[20];
  std::copy(ccondok5x4,ccondok5x4+m*n,Acpy);

  // need an RHS
  complex16 * rhs = new complex16[10]{{1.,10.}, {2.,20.}, {3.,30.}, {4.,40.}, {5., 50.},{1.,10.}, {2.,20.}, {3.,30.}, {4.,40.}, {5.,50.}};

  // expected
  complex16 * expected = new complex16[8]{{     23.3566592349563571,     -14.7515742536566901}, {    -12.7188312276819353,       8.0329460385359930}, {      0.4962521222667151,      -0.3134223930105628}, {     -0.8539880488438545,       0.5393608729540187}, {     23.3566592349563571,     -14.7515742536566901}, {    -12.7188312276819353,       8.0329460385359930}, {      0.4962521222667151,      -0.3134223930105628}, {     -0.8539880488438545,       0.5393608729540187}};
  real8 * expected_S = new real8[4]{148.6321821177508014,20.2132683526172237,8.0449455446454117,0.4207999578471437};

  // need somewhere to write S
  real8 * S = new real8[minmn];
  // condition and rank
  real8 RCOND = - 1; // -1 so rank is computed wrt machine precision
  int4 RANK;

  // make the call
  int4 two = 2;
  lapack::xgelsd(&m, &n, &two, Acpy, &m, rhs, &m, S, &RCOND, &RANK, &INFO);

  // answers are striped in "n" length columns in RHS
  EXPECT_TRUE(ArrayFuzzyEquals(expected,rhs,n,1e-13,1e-13));
  EXPECT_TRUE(ArrayFuzzyEquals(&expected[4],&rhs[5],n,1e-13,1e-13));

  EXPECT_TRUE(ArrayFuzzyEquals(expected_S,S,minmn,1e-13,1e-13));

  EXPECT_EQ(RANK, n);

  // xerbla test
  n=-1;
  EXPECT_THROW(lapack::xgelsd(&m, &n, &two, Acpy, &m, rhs, &m, S, &RCOND, &RANK, &INFO),rdag_error);

  // clean up
  delete [] Acpy;
  delete [] expected;
  delete [] rhs;
  delete [] S;
  delete [] expected_S;
}


TEST(LAPACKTest_xgeev, dgeev) {
  int4 m = 4;
  int4 n = 4;
  int4 minmn = m > n ? n : m;
  int4 INFO = 0;

  real8 * A = new real8[16] {1.,3.,10.,-2.,3.,6.,-1.,-1.,4.,4.,4.,6.,5.,-5.,7.,-10.};

  // expected
  complex16 * expected_W = new complex16[4]{{-9.6888202793819147,0},{-7.1209208815126370,0},{11.5341107067997743,0},{6.2756304540948005,0}};

  real8 * expected_VL = new real8[16] {0.5674175178806528,-0.0926660731685940,-0.4408469400904874,0.6892781257943167,-0.7391126256525055,0.1682952376639553,0.4565908903993675,-0.4657402692327615,-0.6308032002519173,-0.1585352980899745,-0.6832883559806681,-0.3317693542096654,0.2418102871777062,-0.7172636824241223,0.4394794928726053,0.4836510831527837};
  real8 * expected_VR = new real8[16]{-0.4604743713776651,0.3593615791115275,-0.0515866317850007,0.8100379177445289,-0.6283589719101941,0.3481029226389141,0.1720238421645797,0.6740898718992491,-0.4636997919781236,-0.6132122861110744,-0.6309391734073038,-0.1042542782910278,-0.1550485207601204,-0.9326141205284221,0.2734940465430895,0.1771774954856935};

  // need somewhere to write W, VL and VR
  complex16 * W = new complex16[minmn];
  real8 * VL = new real8[n*n];
  real8 * VR = new real8[n*n];

  // make the call
  lapack::xgeev(lapack::V, lapack::V, &n, A, &m, W, VL, &n, VR, &n, &INFO);

  // check
  EXPECT_TRUE(ArrayFuzzyEquals(expected_W,W,n,1e-14,1e-14));
  EXPECT_TRUE(ArrayFuzzyEquals(expected_VL,VL,n*n,1e-14,1e-14));
  EXPECT_TRUE(ArrayFuzzyEquals(expected_VR,VR,n*n,1e-14,1e-14));

  // xerbla test
  n=-1;
  EXPECT_THROW(lapack::xgeev(lapack::V, lapack::V, &n, A, &m, W, VL, &n, VR, &n, &INFO),rdag_error);

  // clean up
  delete [] A;
  delete [] expected_W;
  delete [] expected_VL;
  delete [] expected_VR;
  delete [] VL;
  delete [] VR;
  delete [] W;
}

TEST(LAPACKTest_xgeev, zgeev) {
  int4 m = 4;
  int4 n = 4;
  int4 minmn = m > n ? n : m;
  int4 INFO = 0;

  complex16 * A = new complex16[16] {{1.,10.}, {3.,30.}, {10.,100.}, {-2.,-20.}, {3.,30.}, {6.,60.}, {-1.,-10.}, {-1.,-10.}, {4.,40.}, {4.,40.}, {4.,40.}, {6.,60.}, {5.,50.}, {-5.,-50.}, {7.,70.}, {-10.,-100.}};

  // expected
  complex16 * expected_W = new complex16[4]{{     -9.6888202793818152,     -96.8882027938191897}, {     -7.1209208815127178,     -71.2092088151264164}, {     11.5341107067997406,     115.3411070679975126}, {      6.2756304540948138,      62.7563045409480083}};

  complex16 * expected_VL = new complex16[16] {{      0.5674175178806546,      -0.0000000000000009}, {     -0.0926660731685946,       0.0000000000000003}, {     -0.4408469400904876,       0.0000000000000004}, 0.6892781257943149,0.7391126256525051,{     -0.1682952376639549,      -0.0000000000000000}, {     -0.4565908903993674,       0.0000000000000004}, {      0.4657402692327622,      -0.0000000000000014}, {      0.6308032002519175,      -0.0000000000000002}, {      0.1585352980899743,      -0.0000000000000000}, 0.6832883559806681,{      0.3317693542096652,       0.0000000000000002}, {     -0.2418102871777065,      -0.0000000000000001}, 0.7172636824241221,{     -0.4394794928726055,       0.0000000000000002}, {     -0.4836510831527837,       0.0000000000000003}};
  complex16 * expected_VR = new complex16[16]{{     -0.4604743713776652,       0.0000000000000003}, {      0.3593615791115276,       0.0000000000000002}, {     -0.0515866317850003,      -0.0000000000000004}, 0.8100379177445289,{     -0.6283589719101929,      -0.0000000000000011}, {      0.3481029226389143,       0.0000000000000005}, {      0.1720238421645784,       0.0000000000000010}, 0.6740898718992501,{      0.4636997919781238,      -0.0000000000000001}, 0.6132122861110739,0.6309391734073042,{      0.1042542782910277,       0.0000000000000001}, {      0.1550485207601204,      -0.0000000000000000}, 0.9326141205284220,{     -0.2734940465430895,       0.0000000000000002}, {     -0.1771774954856937,       0.0000000000000001}};

  // need somewhere to write W, VL and VR
  complex16 * W = new complex16[minmn];
  complex16 * VL = new complex16[n*n];
  complex16 * VR = new complex16[n*n];

  // make the call
  lapack::xgeev(lapack::V, lapack::V, &n, A, &m, W, VL, &n, VR, &n, &INFO);

  // check
  EXPECT_TRUE(ArrayFuzzyEquals(expected_W,W,n,1e-13,1e-13));
  EXPECT_TRUE(ArrayFuzzyEquals(expected_VL,VL,n*n,1e-14,1e-14));
  EXPECT_TRUE(ArrayFuzzyEquals(expected_VR,VR,n*n,1e-14,1e-14));

  // xerbla test
  n=-1;
  EXPECT_THROW(lapack::xgeev(lapack::V, lapack::V, &n, A, &m, W, VL, &n, VR, &n, &INFO),rdag_error);

  // clean up
  delete [] A;
  delete [] expected_W;
  delete [] expected_VL;
  delete [] expected_VR;
  delete [] VL;
  delete [] VR;
  delete [] W;
}


TEST(LAPACKTest_xgeqrf, dgeqrf) {

  int4 m = 5;
  int4 n = 4;
  int4 INFO = 0;
  int4 minmn = m > n ? n : m;

  real8 * TAU = new real8[minmn];

  real8 * Acpy = new real8[20];
  std::copy(rcondok5x4,rcondok5x4+m*n,Acpy);

  real8 * expected = new real8[20] {-16.4620776331543297,0.4193443036519876,0.4659381151688751,0.3727504921351001,0.0465938115168875,-30.9802937007701331,-0.4705339669164644,0.5271543871195484,0.1380458452882069,0.1945542709156689,-41.4285496155396729,-3.2545266045055365,-5.0083264004388974,0.1791773291023356,0.0485226775213250,-37.2370981148472424,-0.8155922093218853,-5.7238016005016057,-10.4867263038294549,0.1036735557501358};

  real8 * expected_TAU = new real8[4]{1.3037283696153934,1.4983520738901965,1.9333778010585592,1.9787321786052121};

  lapack::xgeqrf(&m, &n, Acpy, &m, TAU, &INFO);

  EXPECT_TRUE(ArrayFuzzyEquals(expected,Acpy,m*n,1e-13,1e-13));
  EXPECT_TRUE(ArrayFuzzyEquals(expected_TAU,TAU,n,1e-13,1e-13));

  // xerbla test
  n=-1;
  EXPECT_THROW(lapack::xgeqrf(&m, &n, Acpy, &m, TAU, &INFO),rdag_error);

  delete [] TAU;
  delete [] Acpy;
  delete [] expected;
  delete [] expected_TAU;

}

TEST(LAPACKTest_xgeqrf, zgeqrf) {

  int4 m = 5;
  int4 n = 4;
  int4 INFO = 0;
  int4 minmn = m > n ? n : m;

  complex16 * TAU = new complex16[minmn];

  complex16 * Acpy = new complex16[20];
  std::copy(ccondok5x4,ccondok5x4+m*n,Acpy);

  complex16 * expected = new complex16[20] {{    -36.8103246386119238,       0.0000000000000000}, {      0.3010074912401742,      -0.3585220927405818}, {      0.3344527680446380,      -0.3983578808228687}, {      0.2675622144357104,      -0.3186863046582949}, {      0.0334452768044638,      -0.0398357880822869}, {    -69.2740426778305647,      -0.0000000000000106}, {      1.0521459357478518,       0.0000000000000000}, {      0.2628571444550748,       0.5711000051181361}, {      0.2254041184152971,       0.1052355328720274}, {      0.0187265130198877,       0.2329322361230564}, {    -92.6370531495694678,       0.0000000000000142}, {      7.2773427222559253,       0.0000000000000258}, {    -11.1989582848882208,       0.0000000000000000}, {      0.1933057235253725,      -0.1432100966228572}, {      0.0646250639237787,       0.0899178386939252}, {    -83.2646826696277031,       0.0000000000000000}, {      1.8237196219630949,      -0.0000000000000391}, {    -12.7988094684436007,      -0.0000000000000080}, {    -23.4490328767978156,       0.0000000000000000}, {      0.1118475673158655,      -0.0377993268530200}};

  complex16 * expected_TAU = new complex16[4]{{1.1358314562310403,-0.27166291246208063},{1.1023389842523179,0.49317452258808697},{1.7631082023084017,-0.43191932046510112},{1.3367239865466765,-0.92188119028587789}};

  lapack::xgeqrf(&m, &n, Acpy, &m, TAU, &INFO);

  EXPECT_TRUE(ArrayFuzzyEquals(expected,Acpy,m*n,1e-13,1e-13));
  EXPECT_TRUE(ArrayFuzzyEquals(expected_TAU,TAU,n,1e-13,1e-13));

  // xerbla test
  n=-1;
  EXPECT_THROW(lapack::xgeqrf(&m, &n, Acpy, &m, TAU, &INFO),rdag_error);

  delete [] TAU;
  delete [] Acpy;
  delete [] expected;
  delete [] expected_TAU;

}


TEST(LAPACKTest_xxxgqr, dorgqr) {

  int4 m = 5;
  int4 n = 4;
  int4 INFO = 0;
  int4 minmn = m > n ? n : m;

  real8 * TAU = new real8[minmn];

  real8 * Acpy = new real8[20];
  std::copy(rcondok5x4,rcondok5x4+m*n,Acpy);

  real8 * expected = new real8[20] {-0.3037283696153934,-0.5467110653077083,-0.6074567392307869,-0.4859653913846296,-0.0607456739230787,0.8704878387954510,-0.1333179572929945,-0.3842694063151109,0.1176334917291140,-0.2509514490221096,-0.0499168744229821,0.8153089489086596,-0.5158077023707830,-0.2495843721148959,0.0665558325639719,0.2751778011656992,0.0762869151746490,0.4658950891023217,-0.8173598054426700,-0.1825436898821958};

  // create a packed QR form with elementary reflectors in lower part
  lapack::xgeqrf(&m, &n, Acpy, &m, TAU, &INFO);

  // extract Q from packed form of xgeqrf, scalings are in TAU
  int4 k = n;
  lapack::xxxgqr(&m, &n, &k, Acpy, &m, TAU, &INFO);

  EXPECT_TRUE(ArrayFuzzyEquals(expected,Acpy,m*n,1e-13,1e-13));

  // xerbla test
  n=-1;
  EXPECT_THROW(lapack::xxxgqr(&m, &n, &k, Acpy, &m, TAU, &INFO),rdag_error);


  delete [] TAU;
  delete [] Acpy;
  delete [] expected;
}


TEST(LAPACKTest_xxxgqr, zunrgqr) {

  int4 m = 5;
  int4 n = 4;
  int4 INFO = 0;
  int4 minmn = m > n ? n : m;

  complex16 * TAU = new complex16[minmn];

  complex16 * Acpy = new complex16[20];
  std::copy(ccondok5x4,ccondok5x4+m*n,Acpy);

  complex16 * expected = new complex16[20] {{     -0.1358314562310403,       0.2716629124620806}, {     -0.2444966212158725,       0.4889932424317451}, {     -0.2716629124620806,       0.5433258249241611}, {     -0.2173303299696644,       0.4346606599393290}, {     -0.0271662912462081,       0.0543325824924161}, {     -0.3892939962266967,       0.7785879924534080}, {      0.0596216030257065,      -0.1192432060514235}, {      0.1718505028388125,      -0.3437010056776262}, {     -0.0526072967873878,       0.1052145935747794}, {      0.1122288998131025,      -0.2244577996262045}, {     -0.0223235048868174,       0.0446470097736447}, {      0.3646172464847307,      -0.7292344929694671}, {     -0.2306762171638115,       0.4613524343276225}, {     -0.1116175244341002,       0.2232350488682011}, {      0.0297646731824274,      -0.0595293463648534}, {      0.1230632538610821,      -0.2461265077221643}, {      0.0341165456248574,      -0.0682330912497137}, {      0.2083546179232226,      -0.4167092358464459}, {     -0.3655344174091633,       0.7310688348183267}, {     -0.0816360198880458,       0.1632720397760899}};

  // create a packed QR form with elementary reflectors in lower part
  lapack::xgeqrf(&m, &n, Acpy, &m, TAU, &INFO);

  // extract Q from packed form of xgeqrf, scalings are in TAU
  int4 k = n;
  lapack::xxxgqr(&m, &n, &k, Acpy, &m, TAU, &INFO);

  EXPECT_TRUE(ArrayFuzzyEquals(expected,Acpy,m*n,1e-13,1e-13));

  // xerbla test
  n=-1;
  EXPECT_THROW(lapack::xxxgqr(&m, &n, &k, Acpy, &m, TAU, &INFO),rdag_error);

  delete [] TAU;
  delete [] Acpy;
  delete [] expected;
}

