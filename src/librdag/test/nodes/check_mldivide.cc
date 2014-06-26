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
{ { 1., 0., 0. }, { 4., 1e-15, 0. }, { 7., 8., 1. } });

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
  //test UnitUpperTri Branch
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
  //test LowerTri Branch
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
  //test UnitLowerTri Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    A9,
    A1,
    // expected
    OGRealDenseMatrix::create( { {1.0000000000000000, 2.0000000000000000, 3.0000000000000000 }, {-3.0000000000000000, -6.0000000000000000, -9.0000000000000000 },
      {18.0000000000000000, 36.0000000000000000, 54.0000000000000000 } }),
    MATHSEQUAL
  ),
  //test PermutedUpperTriangular Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    A15,
    C2,
    // expected
    OGRealDenseMatrix::create( { {0.0000000000000000, 8.1445922498554086 }, {0.0000000000000000, 3.1787160208212839 }, {0.0000000000000000, -2.0971659919028340 },
      {0.0000000000000000, 0.9473684210526315 }, {0.2000000000000000, 1.6000000000000001 } }),
    MATHSEQUAL
  ),
  //test AlmostPermutedUpperTriangular Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    A17,
    C2,
    // expected
    OGRealDenseMatrix::create({ {0.0000000000000002, -29.9999999999998650 }, {0.0000000000000004, -57.8526315789472108 }, {-0.0000000000000004, 51.3052631578945864 },
      {0.0000000000000000, 0.9473684210526315 }, {0.2000000000000000, 1.6000000000000001 } }),
    MATHSEQUAL
  ),
  //test PermutedUpperTriangularZeroOnDiag Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    A16,
    C1,
    // expected
    OGRealDenseMatrix::create({ {2.0475247524752480 }, {-0.7168316831683170 }, {0.5287128712871273 }, {0.2000000000000003 } }),
    MATHSEQUAL
  ),
  //test PermutedUpperTriangularSingularToMachPrec Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    A18,
    C2,
    // expected
    OGRealDenseMatrix::create({ {-0.0000000000000001, 5.7657712505229739 }, {-0.0000000000000001, -1.3196460795883791 }, {-0.0000000000000001, 2.6102410879868061 },
      {0.0000000000000000, 0.2699985638374270 }, {0.2000000000000001, 1.5925606778687353 } }),
    MATHSEQUAL
  ),
  //test TriDefinedSingular Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    A10,
    A1,
    // expected
    OGRealDenseMatrix::create( { {0.2941176470588236, 0.5882352941176472, 0.8823529411764708 }, {-0.1303167420814479, -0.2606334841628958, -0.3909502262443438 },
      {-0.0162895927601810, -0.0325791855203620, -0.0488687782805430 } }),
    MATHSEQUAL
  ),
  //test TriMachPrecSingular Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    A11,
    A1,
    // expected
    OGRealDenseMatrix::create({ {0.2941176470588236, 0.5882352941176472, 0.8823529411764708 }, {-0.1303167420814479, -0.2606334841628958, -0.3909502262443438 },
      {-0.0162895927601810, -0.0325791855203620, -0.0488687782805430 } }),
    MATHSEQUAL
  ),
  //test LUP Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    A3,
    A1,
    // expected
    OGRealDenseMatrix::create({ {0.0812641083521445, 0.1625282167042890, 0.2437923250564334 }, {0.0609480812641084, 0.1218961625282167, 0.1828442437923251 },
      {0.0654627539503386, 0.1309255079006772, 0.1963882618510158 } }),
    MATHSEQUAL
  ),
  //test LUPBranchFallToLSQ Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    A5,
    A1,
    // expected
    OGRealDenseMatrix::create({ {0.2000000000000003, 0.4000000000000006, 0.6000000000000013 }, {0.4000000000000002, 0.8000000000000004, 1.2000000000000017 },
      {-0.0000000000000001, -0.0000000000000001, 0.0000000000000000 } }),
    MATHSEQUAL
  ),
   //test Chol Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    A2,
    A1,
    // expected
    OGRealDenseMatrix::create({ {0.0059171597633136, 0.0118343195266272, 0.0177514792899408 }, {0.0059171597633136, 0.0118343195266272, 0.0177514792899408 },
      {0.0059171597633136, 0.0118343195266272, 0.0177514792899408 } }),
    MATHSEQUAL
  ),
   //test Chol Not SPD Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    A13,
    A1,
    // expected
    OGRealDenseMatrix::create({ {0.0150267040825563, 0.0300534081651127, 0.0450801122476691 }, {0.0988051054584955, 0.1976102109169910, 0.2964153163754866 },
      {-0.0030174104583446, -0.0060348209166893, -0.0090522313750339 } }),
    MATHSEQUAL
  ),
   //test Chol Singular Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    A12,
    A1,
    // expected
    OGRealDenseMatrix::create({ {0, 0, 0 }, {1e-300, 2e-300, 3e-300 }, {0, 0, 0 } }),
    MATHSEQUAL
  ),
   //test SVD Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    A1,
    A2,
    // expected
    OGRealDenseMatrix::create({ {4.0238095238095219, 4.0238095238095228, 4.0238095238095219 }, {8.0476190476190457, 8.0476190476190474, 8.0476190476190457 },
      {12.0714285714285658, 12.0714285714285694, 12.0714285714285676 } }),
    MATHSEQUAL
  ),
   //test DGELS branch Expansion
  // this branch tests the necessary expansion of the "B" matrix if the solution to the system is larger than the mallocd space
  new CheckBinary<MLDIVIDE>
  (
    // input
    A19,
    A19,
    // expected
    OGRealDenseMatrix::create({ {0.1379310344827586, 0.2068965517241379, 0.2758620689655172 },
      {0.2068965517241379, 0.3103448275862069, 0.4137931034482758 }, {0.2758620689655172, 0.4137931034482757, 0.5517241379310344 } }),
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

// QR: this is a right pain to test because it's rank deficient the results are pulled 
// about by floating point behaviour in deficient part of Q
// we test by reconstruction opposed to absolute value
TEST(MLDIVIDETests, test_rank_deficient_QR_Branch) {
  OGExpr::Ptr op = MLDIVIDE::create(A4, B1);
  runtree(op);
  OGExpr::Ptr  reconstr = PLUS::create(NEGATE::create(B1), MTIMES::create(A4, op->getRegs()[0]->asOGTerminal()));
  runtree(reconstr);
  OGTerminal::Ptr zeros = OGRealDenseMatrix::create({{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}});
  reconstr->getRegs()[0]->asOGTerminal()->debug_print();
  EXPECT_TRUE(zeros->mathsequals(reconstr->getRegs()[0]->asOGTerminal(),1e-14,1e-14));
}