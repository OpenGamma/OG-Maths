/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */


#include "gtest/gtest.h"
#include "terminal.hh"
#include "execution.hh"
#include "dispatch.hh"
#include "testnodes.hh"
#include "runtree.hh"
#include "test/test_utils.hh"

using namespace std;
using namespace librdag;
using namespace testnodes;
using ::testing::TestWithParam;
using ::testing::Values;

/*
 * Check MLDIVIDE node behaves
 */

BINARY_NODE_TEST_SETUP(MLDIVIDE)

// The mass of data cf. java unit tests

// A_square_singular
OGRealDenseMatrix::Ptr A1 = OGRealDenseMatrix::create({ { 1.00, 2.00, 3.00 }, { 1.00, 2.00, 3.00 }, { 1.00, 2.00, 3.00 } });

// A_square_symmetric_positive_definite
OGRealDenseMatrix::Ptr A2 = OGRealDenseMatrix::create({ { 123.00, 23.00, 23.00 }, { 23.00, 123.00, 23.00 }, { 23.00, 23.00, 123.00 } });

// A_square_non_symmetric_well_conditioned 
OGRealDenseMatrix::Ptr A3 = OGRealDenseMatrix::create({ { 10.00, 2.00, 1.00 }, { 2.00, 3.00, 10.00 }, { 4.00, 10.00, 1.00 } });

// A_rectangular 
OGRealDenseMatrix::Ptr A4 = OGRealDenseMatrix::create({ { 1.00, 2.00, 3.00, 4.00 }, { 5.00, 6.00, 7.00, 8.00 }, { 9.00, 10.00, 11.00, 12.00 }, { 13.00, 14.00, 15.00, 16.00 }, { 17.00, 18.00, 19.00, 20.00 } });

// A_square_non_symmetric_condition_bad_for_lu_ok_for_qr
OGRealDenseMatrix::Ptr A5 = OGRealDenseMatrix::create({ { 1.0000000000000009, 2., 20. }, { 1., 2., 0. }, { 1., 2., 40. } });

// A_upper_triangular
OGRealDenseMatrix::Ptr A6 = OGRealDenseMatrix::create({ { 1., 2., 3. }, { 0., 5., 6. }, { 0., 0., 9. } });

// A_unit_upper_triangular
OGRealDenseMatrix::Ptr A7 = OGRealDenseMatrix::create({ { 1., 2., 3. }, { 0., 1., 6. }, { 0., 0., 1. } });

// A_lower_triangular
OGRealDenseMatrix::Ptr A8 = OGRealDenseMatrix::create({ { 1., 0., 0. }, { 4., 5., 0. }, { 7., 8., 9. } });

// A_unit_lower_triangular
OGRealDenseMatrix::Ptr A9 = OGRealDenseMatrix::create({ { 1., 0., 0. }, { 4., 1., 0. }, { 7., 8., 1. } });

// A_lower_triangular_singular_zero_on_diag 
OGRealDenseMatrix::Ptr A10 = OGRealDenseMatrix::create({ { 1., 0., 0. }, { 4., 0., 0. }, { 7., 8., 1. } });

// A_lower_triangular_singular_near_zero_on_diag 
OGRealDenseMatrix::Ptr A11 = OGRealDenseMatrix::create(
{ { 1., 0., 0. }, { 4., 0.0000000000000001, 0. }, { 7., 8., 1. } });

// A_square_spd_near_singular 
OGRealDenseMatrix::Ptr A12 = OGRealDenseMatrix::create({ { 2.0000e+00, 1.0000e+150, 2.0000e+00 }, { 1.0000e+150, 1.0000e+300, 2.0000e+150 }, { 2.0000e+00, 2.0000e+150, 5.0000e+00 } });

// A_square_symmetric_not_spd 
OGRealDenseMatrix::Ptr A13 = OGRealDenseMatrix::create({ { 54., 2., 3. }, { 2., 10., 6. }, { 3., 6., -120. } });

// A_rectangular_with_inf
OGRealDenseMatrix::Ptr A14 = OGRealDenseMatrix::create({ { std::numeric_limits<real8>::infinity(), 2.00, 3.00, 4.00 }, { 5.00, 6.00, 7.00, 8.00 }, { 9.00, 10.00, 11.00, 12.00 },{ 13.00, 14.00, 15.00, 16.00 }, { 17.00, 18.00, 19.00, 20.00 } });

// A_square_permuted_upper_triangular 
OGRealDenseMatrix::Ptr A15 = OGRealDenseMatrix::create({ { 0., 0., 13., 14., 15. }, { 1., 2., 3., 4., 5. }, { 0., 7., 8., 9., 10. }, { 0., 0., 0., 0., 25. }, { 0., 0., 0., 19., 20. } });

// A_square_permuted_upper_triangular_zero_on_diag
OGRealDenseMatrix::Ptr A16 = OGRealDenseMatrix::create({ { 0., 6., 7., 8. }, { 0., 0., 0., 16. }, { 1., 2., 3., 4. }, { 0., 0., 0., 12. } });

// A_square_almost_permuted_upper_triangular
OGRealDenseMatrix::Ptr A17 = OGRealDenseMatrix::create({ { 0., 12., 13., 14., 15. }, { 1., 2., 3., 4., 5. }, { 0., 7., 8., 9., 10. }, { 0., 0., 0., 0., 25. }, { 0., 0., 0., 19., 20. } });

// A_square_permuted_upper_triangular_singular_to_mach_prec
OGRealDenseMatrix::Ptr A18 = OGRealDenseMatrix::create({ { 0., 0., 1.e-300, 14., 15. }, { 1., 2., 3., 4., 5. }, { 0., 7., 8., 9., 10. },{ 0., 0., 0., 0., 25. },{ 0., 0., 0., 19., 20. } });

// A_short_row
OGRealDenseMatrix::Ptr A19 = OGRealDenseMatrix::create({ { 2, 3, 4 } });


// B_rectangular
OGRealDenseMatrix::Ptr B1 = OGRealDenseMatrix::create({ { 1.00, 2.00, 3.00 }, { 1.00, 2.00, 3.00 }, { 1.00, 2.00, 3.00 }, { 1.00, 2.00, 3.00 }, { 1.00, 2.00, 3.00 } });

// C_4len_col_vector
OGRealDenseMatrix::Ptr C1 = OGRealDenseMatrix::create({{1}, {2}, {3}, {4} });

// C_5len__shuffled_col_matrix
OGRealDenseMatrix::Ptr C2 = OGRealDenseMatrix::create({{3.e0,10.e0},{1.e0,20.e0},{2.e0,30.e0},{5.e0,40.e0},{4.e0,50.e0}});



INSTANTIATE_NODE_TEST_CASE_P(MLDIVIDETests,MLDIVIDE,
  Values
  (

  // CHECKING REAL SPACE MATRICES
  new CheckBinary<MLDIVIDE>
  (
    // input
    OGRealScalar::create(2.0),
    OGRealScalar::create(3.0),
    // expected
    OGRealScalar::create(5.0),
    MATHSEQUAL
  ),
  //test UpperTri Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    A6,
    A1,
    // expected
    OGRealDenseMatrix::create( { { 0.5333333333333334, 1.0666666666666669, 1.6000000000000001 }, { 0.0666666666666667, 0.1333333333333334, 0.2000000000000000 },
      { 0.1111111111111111, 0.2222222222222222, 0.3333333333333333 } }),
    MATHSEQUAL
  ),
  //test UnitUpperTriBranch Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    A7,
    A1,
    // expected
    OGRealDenseMatrix::create( { {8.0000000000000000, 16.0000000000000000, 24.0000000000000000 }, {-5.0000000000000000, -10.0000000000000000, -15.0000000000000000 },
      {1.0000000000000000, 2.0000000000000000, 3.0000000000000000 } }),
    MATHSEQUAL
  ),
  //test LowerTriBranch Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    A8,
    A1,
    // expected
    OGRealDenseMatrix::create( { {1.0000000000000000, 2.0000000000000000, 3.0000000000000000 }, {-0.6000000000000000, -1.2000000000000000, -1.8000000000000000 },
      {-0.1333333333333334, -0.2666666666666667, -0.4000000000000000 } }),
    MATHSEQUAL
  ),
  //test UnitLowerTriBranch Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    A9,
    A1,
    // expected
    OGRealDenseMatrix::create( { {1.0000000000000000, 2.0000000000000000, 3.0000000000000000 }, {-3.0000000000000000, -6.0000000000000000, -9.0000000000000000 },
      {18.0000000000000000, 36.0000000000000000, 54.0000000000000000 } }),
    MATHSEQUAL
  )

  ) // end value
);


TEST(MLDIVIDETests, CheckBadCommuteThrows) {
  OGNumeric::Ptr m1 = OGRealDenseMatrix::create(new real8[6]{1,3,5,2,4,6},3,2,OWNER);
  OGNumeric::Ptr m2 = OGRealDenseMatrix::create(new real8[7]{10,30,20,40,50,60,70},1,7,OWNER);
  OGExpr::Ptr node = MLDIVIDE::create(m1, m2);
  ASSERT_THROW(
  runtree(node),
  rdag_error);
}
