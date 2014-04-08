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
real16 r5x4[20] = {1,5,9,13,17,2,6,10,14,18,3,7,11,15,19,4,8,12,16,20};
complex16 c5x4[20] =  {{1,10}, {5,50}, {9,90}, {13,130}, {17,170}, {2,20}, {6,60}, {10,100}, {14,140}, {18,180}, {3,30}, {7,70}, {11,110}, {15,150}, {19,190}, {4,40}, {8,80}, {12,120}, {16,160}, {20,200}};
real16 r4x2[8] = {2,5,11,17,3,7,13,19};
complex16 c4x2[8] = {{2.,20.}, {5.,50.}, {11.,110.}, {17.,170.}, {3.,30.}, {7.,70.}, {13.,130.}, {19.,190.}};
real16 r4x1[4] = {1,2,3,4};
complex16 c4x1[4] = {{1,10},{2,20},{3,30},{4,40}};

// tri upper
real16 rutri4[16] = {1.,0.,0.,0.,2.,6.,0.,0.,3.,7.,11.,0.,4.,8.,12.,16.};
complex16 cutri4[16] = {{1,10}, 0,0,0,{2,20}, {6,60}, 0,0,{3,30}, {7,70}, {11,110}, 0,{4,40}, {8,80}, {12,120}, {16,160}};

// SPD
real16 rspd[16] =  {5,9,10,8,9,17,19,15,10,19,29,21,8,15,21,28};
complex16 cspd[16] = {{ 50,   0}, { 90,  -9}, {100, -10}, { 80,  -8}, { 90,   9}, {170,   0}, {190, -19}, {150, -15}, {100,  10}, {190,  19}, {290,   0}, {210, -21}, { 80,   8}, {150,  15}, {210,  21}, {280,   0}};


// Check successful templating of dscal.
TEST(LAPACKTest_xscal, dscal) {
  int N = 4;
  real16 alpha = 10.e0;
  real16 * x = new real16[4]{1,2,3,4};
  real16 * expected = new real16[4]{10,20,30,40};
  lapack::xscal(&N, &alpha, x, lapack::ione);
  EXPECT_TRUE(ArrayFuzzyEquals(expected,x,4));

  delete[] x;
  delete[] expected;
}

// Check successful templating of zscal.
TEST(LAPACKTest_xscal, zscal) {
  int N = 4;
  complex16 alpha = {2,10};
  complex16 * x = new complex16[4]{{1,10},{2,20},{3,30},{4,40}};
  complex16 * expected = new complex16[4]{{-98.,30.}, {-196.,60.}, {-294.,90.}, {-392.,120.}};
  lapack::xscal(&N, &alpha, x, lapack::ione);
  EXPECT_TRUE(ArrayFuzzyEquals(expected,x,4));

  delete[] x;
  delete[] expected;
}

// Check successful templating of dgemv
TEST(LAPACKTest_xgemv, dgemv) {

  int m = 5;
  int n = 4;

  // copy in A
  real16 * A = new real16[20];
  std::copy(r5x4,r5x4+m*n,A);

  real16 alpha = 2.e0;

  // copy in x
  real16 * x = new real16[4];
  std::copy(r4x1,r4x1+n,x);

  real16 beta = 3.e0;

  real16 y[5] = {-1,-1,-1,-1,-1};

  real16 * expected = new real16[5]{57.,137.,217.,297.,377.};

  //check call
  lapack::xgemv(lapack::N, &m, &n, &alpha, A, &m, x, lapack::ione, &beta, y, lapack::ione);
  EXPECT_TRUE(ArrayFuzzyEquals(expected,y,m));

  delete[] A;
  delete[] x;
  delete[] expected;
}

// Check successful templating of zgemv
TEST(LAPACKTest_xgemv, zgemv) {

  int m = 5;
  int n = 4;

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

  int m = 5;
  int k = 4;
  int n = 2;

  // copy in A
  real16 * A = new real16[20];
  std::copy(r5x4,r5x4+m*k,A);

  // copy in B
  real16 * B = new real16[8];
  std::copy(r4x2,r4x2+n*k,B);

  real16 alpha = 2.e0;

  real16 beta = 3.e0;

  // create C
  real16 C[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

  real16 * expected = new real16[10]{223.,503.,783.,1063.,1343.,261.,597.,933.,1269.,1605.};

  //check call
  lapack::xgemm(lapack::N,lapack::N, &m, &n, &k, &alpha, A, &m, B, &k, &beta, C, &m);
  EXPECT_TRUE(ArrayFuzzyEquals(expected,C,m*n));

  delete[] A;
  delete[] B;
  delete[] expected;
}

// Check successful templating of dgemm
TEST(LAPACKTest_xgemm, zgemm) {

  int m = 5;
  int k = 4;
  int n = 2;

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

  int n = 4;
  real16 answer = lapack::xnrm2(&n, r4x1, lapack::ione);
  real16 expected = 5.4772255750516612e0;
  EXPECT_TRUE(SingleValueFuzzyEquals(expected,answer));
}

// Check successful templating of znrm2
TEST(LAPACKTest_xnrm2, znrm2) {

  int n = 4;
  real16 answer = lapack::xnrm2(&n, c4x1, lapack::ione);
  real16 expected = 55.0454357780915302e0;
  EXPECT_TRUE(SingleValueFuzzyEquals(expected,answer));
}


// Check successful templating of dgesvd.
TEST(LAPACKTest_xgesvd, dgesvd) {
  int m = 3;
  int n = 2;
  int INFO;

  real16* expectedU = new real16[9] {-0.2298476964000714,-0.5247448187602936,-0.8196419411205156,0.8834610176985253,0.2407824921325463,-0.4018960334334317,0.4082482904638627,-0.8164965809277263,0.4082482904638631};
  real16* expectedS = new real16[2] {9.525518091565107,  0.514300580658644};
  real16 * expectedVT = new real16[4]{-0.6196294838293402,-0.7848944532670524,-0.7848944532670524,0.6196294838293402};

  real16* U  = new real16[9]();
  real16* S  = new real16[2]();
  real16* VT = new real16[4]();

  real16 * A = new real16[6] {1,3,5,2,4,6};
  real16 * Acpy = new real16[6];
  std::copy(A,A+m*n,Acpy);

  lapack::xgesvd(lapack::A, lapack::A, &m, &n, Acpy, &m, S, U, &m, VT, &n, &INFO);

  EXPECT_TRUE(ArrayFuzzyEquals(expectedU,U,9,1e-14,1e-14));
  EXPECT_TRUE(ArrayFuzzyEquals(expectedS,S,2,1e-14,1e-14));
  EXPECT_TRUE(ArrayFuzzyEquals(expectedVT,VT,4,1e-14,1e-14));

  // check throw on bad arg
  std::copy(A,A+m*n,Acpy);
  m = -1;
  EXPECT_THROW(lapack::xgesvd(lapack::A, lapack::A, &m, &n, Acpy, &m, S, U, &m, VT, &n, &INFO), rdag_error);

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
  int m = 3;
  int n = 2;
  int INFO;

  complex16* expectedU = new complex16[9] {{-0.0228707006002169,-0.2287070060021659}, {-0.0522140610036492,-0.5221406100364927}, {-0.0815574214070819,-0.8155742140708198}, {-0.0879076568710847,-0.8790765687107978}, {-0.0239587534423321,-0.2395875344233279}, {0.0399901499864153, 0.3999014998641418}, {0.3147401422050641, 0.2600102104752862}, {-0.6294802844101258,-0.5200204209505755}, {0.3147401422050625, 0.2600102104752883}};
  real16* expectedS = new real16[2] {95.7302720469661779,5.1686568674896343};
  complex16 * expectedVT = new complex16[4]{{-0.6196294838293402,0},{0.7848944532670524,-0.e0},{-0.7848944532670524,0},{-0.6196294838293402,0}};

  complex16* U  = new complex16[9]();
  real16* S  = new real16[2]();
  complex16* VT = new complex16[4]();

  complex16 * A = new complex16[6]{{1,10}, {3,30}, {5,50}, {2,20}, {4,40}, {6,60}};
  complex16 * Acpy = new complex16[6];
  std::copy(A,A+m*n,Acpy);

  lapack::xgesvd(lapack::A, lapack::A, &m, &n, Acpy, &m, S, U, &m, VT, &n, &INFO);

  EXPECT_TRUE(ArrayFuzzyEquals(expectedU,U,9,1e-14,1e-14));
  EXPECT_TRUE(ArrayFuzzyEquals(expectedS,S,2,1e-14,1e-14));
  EXPECT_TRUE(ArrayFuzzyEquals(expectedVT,VT,4,1e-14,1e-14));

  // check throw on bad arg
  std::copy(A,A+m*n,Acpy);
  m = -1;
  EXPECT_THROW(lapack::xgesvd(lapack::A, lapack::A, &m, &n, Acpy, &m, S, U, &m, VT, &n, &INFO), rdag_error);

  delete[] U;
  delete[] S;
  delete[] VT;
  delete[] expectedU;
  delete[] expectedS;
  delete[] expectedVT;
  delete[] A;
  delete[] Acpy;
}

// Check successful templating of dtrcon.
TEST(LAPACKTest_xtrcon, dtrcon) {
  int n = 4;
  real16 rcond = 0;
  int INFO = 0;
  lapack::xtrcon(lapack::ONE, lapack::U, lapack::N, &n, rutri4, &n, &rcond, &INFO );
  real16 expected = 0.025e0;
  EXPECT_TRUE(SingleValueFuzzyEquals(expected,rcond));
}

// Check successful templating of ztrcon.
TEST(LAPACKTest_xtrcon, ztrcon) {
  int n = 4;
  real16 rcond = 0;
  int INFO = 0;
  lapack::xtrcon(lapack::ONE, lapack::U, lapack::N, &n, cutri4, &n, &rcond, &INFO );
  real16 expected = 0.025e0;
  EXPECT_TRUE(SingleValueFuzzyEquals(expected,rcond));
}

// Check successful templating of dtrtrs.
TEST(LAPACKTest_xtrtrs, dtrtrs) {
  int n = 4;
  real16 * RHS = new real16[4]{10,8,2,-1};
  int INFO = 0;
  lapack::xtrtrs(lapack::U, lapack::N, lapack::N, &n, lapack::ione, rutri4, &n, RHS, &n, &INFO );
  real16 expected[4] = {7.25,1.125,0.25,-0.0625};
  EXPECT_TRUE(ArrayFuzzyEquals(expected,RHS,n));

  // check throw on singluar matrix
  real16 * A = new real16[16];
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
  int n = 4;
  complex16 * RHS = new complex16[4]{{10.,-2.}, {-7.,3.}, {3.,-6.}, {-2.,-5.}};
  int INFO = 0;
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

// Check successful templating of dpotrf.
TEST(LAPACKTest_xpotrf, dpotrf) {
  int n=4;
  int INFO = 0;
  real16 * A = new real16[16];

  std::copy(rspd,rspd+n*n,A);
  lapack::xpotrf(lapack::U, &n, A, &n, &INFO);

  real16 * expected = new real16[16]{2.2360679774997898,9,10,8,4.0249223594996213,0.8944271909999163,19,15,4.4721359549995796,1.1180339887498942,2.7838821814150103,21,3.5777087639996634,0.6708203932499381,1.5266450672275866,3.5241105031922135};
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
  int n=4;
  int INFO = 0;
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
  int n = 4;
  int INFO = 0;
  real16 * A = new real16[16];
  std::copy(rspd,rspd+n*n,A);
  real16 ANORM = lapack::xlansy(lapack::ONE, lapack::U, &n, A, &n);
  lapack::xpotrf(lapack::U, &n, A, &n, &INFO);
  real16 RCOND = 0;
  lapack::xpocon(lapack::U, &n, A, &n, &ANORM, &RCOND, &INFO);
  real16 anorm_expected = 79e0;
  EXPECT_TRUE(ANORM==anorm_expected);
  real16 rcond_expected = 0.00190665795051604e0;
  EXPECT_TRUE(SingleValueFuzzyEquals(rcond_expected, RCOND));

  // check throw on bad arg
  n = -1;
  EXPECT_THROW(lapack::xpocon(lapack::U, &n, A, &n, &ANORM, &RCOND, &INFO), rdag_error);

  delete [] A;
}


// Check successful templating of zpocon.
TEST(LAPACKTest_xpocon, zpocon) {
  int n = 4;
  int INFO = 0;
  complex16 * A = new complex16[16];
  std::copy(cspd,cspd+n*n,A);
  real16 ANORM = lapack::zlanhe(lapack::ONE, lapack::U, &n, A, &n);
  lapack::xpotrf(lapack::U, &n, A, &n, &INFO);
  real16 RCOND = 0;
  lapack::xpocon(lapack::U, &n, A, &n, &ANORM, &RCOND, &INFO);
  real16 anorm_expected = 792.493781056045;
  EXPECT_TRUE(SingleValueFuzzyEquals(ANORM,anorm_expected));
  real16 rcond_expected = 3.21351909155511e-04;
  EXPECT_TRUE(SingleValueFuzzyEquals(rcond_expected, RCOND));

  // check throw on bad arg
  n = -1;
  EXPECT_THROW(lapack::xpocon(lapack::U, &n, A, &n, &ANORM, &RCOND, &INFO), rdag_error);

  delete [] A;
}
