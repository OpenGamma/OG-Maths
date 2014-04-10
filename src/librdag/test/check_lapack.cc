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


// Check successful templating of dgesvd.
TEST(LAPACKTest, dgesvd) {
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

  lapack::xgesvd<real16>(lapack::A, lapack::A, &m, &n, Acpy, &m, S, U, &m, VT, &n, &INFO);

  EXPECT_TRUE(ArrayFuzzyEquals(expectedU,U,9,1e-14,1e-14));
  EXPECT_TRUE(ArrayFuzzyEquals(expectedS,S,2,1e-14,1e-14));
  EXPECT_TRUE(ArrayFuzzyEquals(expectedVT,VT,4,1e-14,1e-14));

  // check throw on bad arg
  std::copy(A,A+m*n,Acpy);
  m = -1;
  EXPECT_THROW(lapack::xgesvd<real16>(lapack::A, lapack::A, &m, &n, Acpy, &m, S, U, &m, VT, &n, &INFO), rdag_error);

  delete[] U;
  delete[] S;
  delete[] VT;
  delete[] expectedU;
  delete[] expectedS;
  delete[] expectedVT;
  delete[] A;
  delete[] Acpy;
}
