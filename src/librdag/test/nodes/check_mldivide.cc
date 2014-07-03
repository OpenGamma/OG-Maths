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
#include "numerictypes.hh"
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
OGRealDenseMatrix::Ptr REAL_A_1 = OGRealDenseMatrix::create({ { 1.00, 2.00, 3.00 }, { 1.00, 2.00, 3.00 }, { 1.00, 2.00, 3.00 } });
OGComplexDenseMatrix::Ptr CMPLX_A_1 = OGComplexDenseMatrix::create( {{{1.0,10.0},{2.0,20.0},{3.0,30.0}},{{1.0,10.0},{2.0,20.0},{3.0,30.0}},{{1.0,10.0},{2.0,20.0},{3.0,30.0}}});

// A_square_symmetric_positive_definite
OGRealDenseMatrix::Ptr REAL_A_2 = OGRealDenseMatrix::create({ { 123.00, 23.00, 23.00 }, { 23.00, 123.00, 23.00 }, { 23.00, 23.00, 123.00 } });
OGComplexDenseMatrix::Ptr CMPLX_A_2 = OGComplexDenseMatrix::create({{{20.0,0.0},{2.0,1.0},{4.0,0.0}},{{2.0,-1.0},{30.0,0.0},{0.0,1.0}},{{4.0,0.0},{-0.0,-1.0},{10.0,0.0}}}
); // is hermitian at machine prec

// A_square_non_symmetric_well_conditioned 
OGRealDenseMatrix::Ptr REAL_A_3 = OGRealDenseMatrix::create({ { 10.00, 2.00, 1.00 }, { 2.00, 3.00, 10.00 }, { 4.00, 10.00, 1.00 } });
OGComplexDenseMatrix::Ptr CMPLX_A_3 = OGComplexDenseMatrix::create({{{10.0,100.0},{2.0,20.0},{1.0,10.0}},{{2.0,20.0},{3.0,30.0},{10.0,100.0}},{{4.0,40.0},{10.0,100.0},{1.0,10.0}}});

// A_rectangular
OGRealDenseMatrix::Ptr REAL_A_4 = OGRealDenseMatrix::create({ { 1.00, 2.00, 3.00, 4.00 }, { 5.00, 6.00, 7.00, 8.00 }, { 9.00, 10.00, 11.00, 12.00 }, { 13.00, 14.00, 15.00, 16.00 }, { 17.00, 18.00, 19.00, 20.00 } });

// A_square_non_symmetric_condition_bad_for_lu_ok_for_qr
OGRealDenseMatrix::Ptr REAL_A_5 = OGRealDenseMatrix::create({ { 1.0000000000000009, 2., 20. }, { 1., 2., 0. }, { 1., 2., 40. } });
OGComplexDenseMatrix::Ptr CMPLX_A_5 = OGComplexDenseMatrix::create({{{1.0000000000000009,10.0000000000000089},{2.0,20.0},{20.0,200.0}},{{1.0,10.0},{2.0,20.0},{0.0,0.0}},{{1.0,10.0},{2.0,20.0},{40.0,400.0}}});

// A_upper_triangular
OGRealDenseMatrix::Ptr REAL_A_6 = OGRealDenseMatrix::create({ { 1., 2., 3. }, { 0., 5., 6. }, { 0., 0., 9. } });
OGComplexDenseMatrix::Ptr CMPLX_A_6 = OGComplexDenseMatrix::create({{{1.0,10.0},{2.0,20.0},{3.0,30.0}},{{0.0,0.0},{5.0,50.0},{6.0,60.0}},{{0.0,0.0},{0.0,0.0},{9.0,90.0}}});

// A_unit_upper_triangular
OGRealDenseMatrix::Ptr REAL_A_7 = OGRealDenseMatrix::create({ { 1., 2., 3. }, { 0., 1., 6. }, { 0., 0., 1. } });
OGComplexDenseMatrix::Ptr CMPLX_A_7 = OGComplexDenseMatrix::create({{{1.0,0.0},{2.0,20.0},{3.0,30.0}},{{0.0,0.0},{1.0,0.0},{6.0,60.0}},{{0.0,0.0},{0.0,0.0},{1.0,0.0}}});

// A_lower_triangular
OGRealDenseMatrix::Ptr REAL_A_8 = OGRealDenseMatrix::create({ { 1., 0., 0. }, { 4., 5., 0. }, { 7., 8., 9. } });
OGComplexDenseMatrix::Ptr CMPLX_A_8 = OGComplexDenseMatrix::create(
{{{1.0,10.0},{0.0,0.0},{0.0,0.0}},{{4.0,40.0},{5.0,50.0},{0.0,0.0}},{{7.0,70.0},{8.0,80.0},{9.0,90.0}}});

// A_unit_lower_triangular
OGRealDenseMatrix::Ptr REAL_A_9 = OGRealDenseMatrix::create({ { 1., 0., 0. }, { 4., 1., 0. }, { 7., 8., 1. } });
OGComplexDenseMatrix::Ptr CMPLX_A_9 = OGComplexDenseMatrix::create(
{{{1.0,0.0},{0.0,0.0},{0.0,0.0}},{{4.0,40.0},{1.0,0.0},{0.0,0.0}},{{7.0,70.0},{8.0,80.0},{1.0,0.0}}});

// A_lower_triangular_singular_zero_on_diag 
OGRealDenseMatrix::Ptr REAL_A_10 = OGRealDenseMatrix::create({ { 1., 0., 0. }, { 4., 0., 0. }, { 7., 8., 1. } });
OGComplexDenseMatrix::Ptr CMPLX_A_10 = OGComplexDenseMatrix::create({{{1.0,10.0},{0.0,0.0},{0.0,0.0}},{{4.0,40.0},{0.0,0.0},{0.0,0.0}},{{7.0,70.0},{8.0,80.0},{1.0,10.0}}});

// A_lower_triangular_singular_near_zero_on_diag 
OGRealDenseMatrix::Ptr REAL_A_11 = OGRealDenseMatrix::create(
{ { 1., 0., 0. }, { 4., 1e-15, 0. }, { 7., 8., 1. } });
OGComplexDenseMatrix::Ptr CMPLX_A_11 = OGComplexDenseMatrix::create(
{{{1.0,10.0},{0.0,0.0},{0.0,0.0}},{{4.0,40.0},{1e-15,1e-14},{0.0,0.0}},{{7.0,70.0},{8.0,80.0},{1.0,10.0}}});

// A_square_spd_near_singular 
OGRealDenseMatrix::Ptr REAL_A_12 = OGRealDenseMatrix::create({ { 2.0000e+00, 1.0000e+150, 2.0000e+00 }, { 1.0000e+150, 1.0000e+300, 2.0000e+150 }, { 2.0000e+00, 2.0000e+150, 5.0000e+00 } });
OGComplexDenseMatrix::Ptr CMPLX_A_12 = OGComplexDenseMatrix::create( {{{1.0e36,0.0},{1.0,1.0},{0.0,3.0}},{{1.0,-1.0},{1.0e14,0.0},{2.0,-2.0}},{{-0.0,-3.0},{2.0,2.0},{1.0,0.0}}});

// A_square_symmetric_not_spd
OGRealDenseMatrix::Ptr REAL_A_13 = OGRealDenseMatrix::create({ { 54., 2., 3. }, { 2., 10., 6. }, { 3., 6., -120. } });
OGComplexDenseMatrix::Ptr CMPLX_A_13 = OGComplexDenseMatrix::create({{{54.0,0.0},{2.0,20.0},{3.0,30.0}},{{2.0,-20.0},{10.0,0.0},{6.0,60.0}},{{3.0,-30.0},{6.0,-60.0},{-120.0,0.0}}});

// A_rectangular_with_inf
OGRealDenseMatrix::Ptr REAL_A_14 = OGRealDenseMatrix::create({ { std::numeric_limits<real8>::infinity(), 2.00, 3.00, 4.00 }, { 5.00, 6.00, 7.00, 8.00 }, { 9.00, 10.00, 11.00, 12.00 },{ 13.00, 14.00, 15.00, 16.00 }, { 17.00, 18.00, 19.00, 20.00 } });

// A_square_permuted_upper_triangular 
OGRealDenseMatrix::Ptr REAL_A_15 = OGRealDenseMatrix::create({ { 0., 0., 13., 14., 15. }, { 1., 2., 3., 4., 5. }, { 0., 7., 8., 9., 10. }, { 0., 0., 0., 0., 25. }, { 0., 0., 0., 19., 20. } });
OGComplexDenseMatrix::Ptr CMPLX_A_15 = OGComplexDenseMatrix::create(
{{{0.0,0.0},{0.0,0.0},{13.0,130.0},{14.0,140.0},{15.0,150.0}},{{1.0,10.0},{2.0,20.0},{3.0,30.0},{4.0,40.0},{5.0,50.0}},{{0.0,0.0},{7.0,70.0},{8.0,80.0},{9.0,90.0},{10.0,100.0}},{{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{25.0,250.0}},{{0.0,0.0},{0.0,0.0},{0.0,0.0},{19.0,190.0},{20.0,200.0}}});


// A_square_permuted_upper_triangular_zero_on_diag
OGRealDenseMatrix::Ptr REAL_A_16 = OGRealDenseMatrix::create({ { 0., 6., 7., 8. }, { 0., 0., 0., 16. }, { 1., 2., 3., 4. }, { 0., 0., 0., 12. } });
OGComplexDenseMatrix::Ptr CMPLX_A_16 = OGComplexDenseMatrix::create({{{0.0,0.0},{6.0,60.0},{7.0,70.0},{8.0,80.0}},{{0.0,0.0},{0.0,0.0},{0.0,0.0},{16.0,160.0}},{{1.0,10.0},{2.0,20.0},{3.0,30.0},{4.0,40.0}},{{0.0,0.0},{0.0,0.0},{0.0,0.0},{12.0,120.0}}});

// A_square_almost_permuted_upper_triangular
OGRealDenseMatrix::Ptr REAL_A_17 = OGRealDenseMatrix::create({ { 0., 12., 13., 14., 15. }, { 1., 2., 3., 4., 5. }, { 0., 7., 8., 9., 10. }, { 0., 0., 0., 0., 25. }, { 0., 0., 0., 19., 20. } });
OGComplexDenseMatrix::Ptr CMPLX_A_17 = OGComplexDenseMatrix::create( {{{0.0,0.0},{12.0,120.0},{13.0,130.0},{14.0,140.0},{15.0,150.0}},{{1.0,10.0},{2.0,20.0},{3.0,30.0},{4.0,40.0},{5.0,50.0}},{{0.0,0.0},{7.0,70.0},{8.0,80.0},{9.0,90.0},{10.0,100.0}},{{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{25.0,250.0}},{{0.0,0.0},{0.0,0.0},{0.0,0.0},{19.0,190.0},{20.0,200.0}}});

// A_square_permuted_upper_triangular_singular_to_mach_prec
OGRealDenseMatrix::Ptr REAL_A_18 = OGRealDenseMatrix::create({ { 0., 0., 1.e-300, 14., 15. }, { 1., 2., 3., 4., 5. }, { 0., 7., 8., 9., 10. },{ 0., 0., 0., 0., 25. },{ 0., 0., 0., 19., 20. } });
OGComplexDenseMatrix::Ptr CMPLX_A_18 = OGComplexDenseMatrix::create({{{0.0,0.0},{0.0,0.0},{1.e-300,1.e-299},{14.0,140.0},{15.0,150.0}},{{1.0,10.0},{2.0,20.0},{3.0,30.0},{4.0,40.0},{5.0,50.0}},{{0.0,0.0},{7.0,70.0},{8.0,80.0},{9.0,90.0},{10.0,100.0}},{{0.0,0.0},{0.0,0.0},{0.0,0.0},{0.0,0.0},{25.0,250.0}},{{0.0,0.0},{0.0,0.0},{0.0,0.0},{19.0,190.0},{20.0,200.0}}});

// A_short_row
OGRealDenseMatrix::Ptr REAL_A_19 = OGRealDenseMatrix::create({ { 2, 3, 4 } });
OGComplexDenseMatrix::Ptr CMPLX_A_19 = OGComplexDenseMatrix::create({{{2,20},{3,30},{4,40}}});

// A_short_2_x_row
OGRealDenseMatrix::Ptr REAL_A_20 = OGRealDenseMatrix::create({ { 2e-6,3,4e6 },{ 2e-6,3,4e6 } });
OGComplexDenseMatrix::Ptr CMPLX_A_20 = OGComplexDenseMatrix::create({{{2e-6,0},{3,1},{4e6,1e40}},{{2e-6,0},{3,1},{4e6,1e40}}});

OGRealDenseMatrix::Ptr REAL_A_21 = OGRealDenseMatrix::create({ { 0., 0., 0. }, { 0., 0., 0. }, { 0., 0., 0. }});
OGComplexDenseMatrix::Ptr CMPLX_A_21 = OGComplexDenseMatrix::create({ { 0., 0., 0. }, { 0., 0., 0. }, { 0., 0., 0. }});


// B_rectangular
OGRealDenseMatrix::Ptr REAL_B_1 = OGRealDenseMatrix::create({ { 1.00, 2.00, 3.00 }, { 1.00, 2.00, 3.00 }, { 1.00, 2.00, 3.00 }, { 1.00, 2.00, 3.00 }, { 1.00, 2.00, 3.00 } });

// C_4len_col_vector
OGRealDenseMatrix::Ptr REAL_C_1 = OGRealDenseMatrix::create({{1}, {2}, {3}, {4} });

// C_5len__shuffled_col_matrix
OGRealDenseMatrix::Ptr REAL_C_2 = OGRealDenseMatrix::create({{3.e0,10.e0},{1.e0,20.e0},{2.e0,30.e0},{5.e0,40.e0},{4.e0,50.e0}});
OGComplexDenseMatrix::Ptr CMPLX_C_2 = OGComplexDenseMatrix::create({{{3.0,30.0},{10.0,100.0}},{{1.0,10.0},{20.0,200.0}},{{2.0,20.0},{30.0,300.0}},{{5.0,50.0},{40.0,400.0}},{{4.0,40.0},{50.0,500.0}}});

// A more stable LHS for complex space
OGComplexDenseMatrix::Ptr CMPLX_C_3 = OGComplexDenseMatrix::create({{{1.0,-2.0},{10.0,-20.0}},{{-1.0,2.0},{-10.0,20.0}},{{1.0,-2.0},{10.0,-20.0}},{{-1.0,2.0},{-10.0,20.0}},{{1.0,-2.0},{10.0,-20.0}}});

// 4 row single col variant of CMPLX_C_3
OGComplexDenseMatrix::Ptr CMPLX_C_4 = OGComplexDenseMatrix::create({{{1.0,-2.0}},{{-1.0,2.0}},{{1.0,-2.0}},{{-1.0,2.0}}});

// stable complex space 3x3, like CMPLX_A_1
OGComplexDenseMatrix::Ptr CMPLX_C_5 = OGComplexDenseMatrix::create({{{1.0,0.0},{2.0,0.0},{3.0,0.0}},{{4.0,0.0},{0.0,5.0},{1.0,2.0}},{{-4.0,-1.0},{3.0,1.0},{-0.0,-5.0}}});





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

  //test catch of all zeros Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    REAL_A_21,
    REAL_A_1,
    // expected
    OGRealDenseMatrix::create( { { getPosInf(), getPosInf(), getPosInf()}, { getPosInf(), getPosInf(), getPosInf()}, { getPosInf(), getPosInf(), getPosInf()} }),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    CMPLX_A_21,
    CMPLX_A_1,
    // expected
    OGComplexDenseMatrix::create({ { getPosInf(), getPosInf(), getPosInf()}, { getPosInf(), getPosInf(), getPosInf()}, { getPosInf(), getPosInf(), getPosInf()} }),
    MATHSEQUAL
  ),

  //test UpperTri Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    REAL_A_6,
    REAL_A_1,
    // expected
    OGRealDenseMatrix::create( { { 0.5333333333333334, 1.0666666666666669, 1.6000000000000001 }, { 0.0666666666666667, 0.1333333333333334, 0.2000000000000000 },
      { 0.1111111111111111, 0.2222222222222222, 0.3333333333333333 } }),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    CMPLX_A_6,
    CMPLX_A_1,
    // expected
    OGComplexDenseMatrix::create({{{0.5333333333333334,0.0},{1.0666666666666669,0.0},{1.6000000000000001,0.0}},{{0.0666666666666667,0.0},{0.1333333333333334,0.0},{0.2000000000000000,0.0}},{{0.1111111111111111,0.0},{0.2222222222222222,0.0},{0.3333333333333333,0.0}}}),
    MATHSEQUAL
  ),

  //test UnitUpperTri Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    REAL_A_7,
    REAL_A_1,
    // expected
    OGRealDenseMatrix::create( { {8.0000000000000000, 16.0000000000000000, 24.0000000000000000 }, {-5.0000000000000000, -10.0000000000000000, -15.0000000000000000 },
      {1.0000000000000000, 2.0000000000000000, 3.0000000000000000 } }),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    CMPLX_A_7,
    CMPLX_A_1,
    // expected
    OGComplexDenseMatrix::create({{{-3092.0,-11730.0},{-6184.0,-23460.0},{-9276.0,-35190.0}},{{595.0,-110.0},{1190.0,-220.0},{1785.0,-330.0}},{{1.0,10.0},{2.0,20.0},{3.0,30.0}}}),
    MATHSEQUAL
  ),

  //test LowerTri Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    REAL_A_8,
    REAL_A_1,
    // expected
    OGRealDenseMatrix::create( { {1.0000000000000000, 2.0000000000000000, 3.0000000000000000 }, {-0.6000000000000000, -1.2000000000000000, -1.8000000000000000 },
      {-0.1333333333333334, -0.2666666666666667, -0.4000000000000000 } }),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    CMPLX_A_8,
    CMPLX_A_1,
    // expected
    OGComplexDenseMatrix::create({{{1.0,0.0},{2.0,0.0},{3.0,0.0}},{{-0.6000000000000000,0.0},{-1.2000000000000000,0.0},{-1.8000000000000000,0.0}},{{-0.1333333333333333,0.0},{-0.2666666666666666,0.0},{-0.4000000000000000,-0.0}}}),
    MATHSEQUAL
  ),

  //test UnitLowerTri Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    REAL_A_9,
    REAL_A_1,
    // expected
    OGRealDenseMatrix::create( { {1.0000000000000000, 2.0000000000000000, 3.0000000000000000 }, {-3.0000000000000000, -6.0000000000000000, -9.0000000000000000 },
      {18.0000000000000000, 36.0000000000000000, 54.0000000000000000 } }),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    CMPLX_A_9,
    CMPLX_A_1,
    // expected
    OGComplexDenseMatrix::create({{{1.0,10.0},{2.0,20.0},{3.0,30.0}},{{397.0,-70.0},{794.0,-140.0},{1191.0,-210.0}},{{-8082.0,-31330.0},{-16164.0,-62660.0},{-24246.0,-93990.0}}}),
    MATHSEQUAL
  ),

  //test PermutedUpperTriangular Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    REAL_A_15,
    REAL_C_2,
    // expected
    OGRealDenseMatrix::create( { {0.0000000000000000, 8.1445922498554086 }, {0.0000000000000000, 3.1787160208212839 }, {0.0000000000000000, -2.0971659919028340 },
      {0.0000000000000000, 0.9473684210526315 }, {0.2000000000000000, 1.6000000000000001 } }),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    CMPLX_A_15,
    CMPLX_C_2,
    // expected
    OGComplexDenseMatrix::create({{{0.0,0.0},{8.1445922498554069,-0.0000000000000002}},{{0.0,0.0},{3.1787160208212835,0.0000000000000001}},{{0.0,0.0},{-2.0971659919028340,-0.0}},{{0.0,0.0},{0.9473684210526316,0.0}},{{0.2000000000000000,0.0},{1.6000000000000001,0.0}}}),
    MATHSEQUAL
  ),

  //test AlmostPermutedUpperTriangular Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    REAL_A_17,
    REAL_C_2,
    // expected
    OGRealDenseMatrix::create({ {0.0000000000000002, -29.9999999999998650 }, {0.0000000000000004, -57.8526315789472108 }, {-0.0000000000000004, 51.3052631578945864 },
      {0.0000000000000000, 0.9473684210526315 }, {0.2000000000000000, 1.6000000000000001 } }),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    CMPLX_A_17,
    CMPLX_C_3,
    // expected
    OGComplexDenseMatrix::create({{{0.3762376237623766,0.2376237623762379},{3.7623762376237662,2.3762376237623792}},{{0.1853465346534657,0.1170609692548206},{1.8534653465346569,1.1706096925482055}},{{-0.1750495049504954,-0.1105575820739972},{-1.7504950495049536,-1.1055758207399717}},{{-0.0178217821782178,-0.0112558624283481},{-0.1782178217821782,-0.1125586242834810}},{{0.0075247524752475,0.0047524752475248},{0.0752475247524752,0.0475247524752475}}}),
    MATHSEQUAL
  ),

  //test PermutedUpperTriangularZeroOnDiag Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    REAL_A_16,
    REAL_C_1,
    // expected
    OGRealDenseMatrix::create({ {2.0475247524752480 }, {-0.7168316831683170 }, {0.5287128712871273 }, {0.2000000000000003 } }),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    CMPLX_A_16,
    CMPLX_C_4,
    // expected
    OGComplexDenseMatrix::create({{{-0.1067620821488090,-0.0674286834624056}},{{0.0144534849524556,0.0091285168120772}},{{-0.0543123223213409,-0.0343025193608469}},{{0.0131683168316832,0.0083168316831683}}}),
    MATHSEQUAL
  ),

  //test PermutedUpperTriangularSingularToMachPrec Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    REAL_A_18,
    REAL_C_2,
    // expected
    OGRealDenseMatrix::create({ {-0.0000000000000001, 5.7657712505229739 }, {-0.0000000000000001, -1.3196460795883791 }, {-0.0000000000000001, 2.6102410879868061 },
      {0.0000000000000000, 0.2699985638374270 }, {0.2000000000000001, 1.5925606778687353 } }),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    CMPLX_A_18,
    CMPLX_C_3,
    // expected
    OGComplexDenseMatrix::create( {{{0.2109968569492433,0.1332611728100484},{2.1099685694924331,1.3326117281004841}},{{-0.0803459698947434,-0.0507448230914169},{-0.8034596989474336,-0.5074482309141690}},{{0.0588880750840383,0.0371924684741295},{0.5888807508403830,0.3719246847412949}},{{-0.0191009622289243,-0.0120637656182680},{-0.1910096222892430,-0.1206376561826800}},{{0.0075107036770028,0.0047436023223175},{0.0751070367700277,0.0474360232231754}}}),
    MATHSEQUAL
  ),


  //test TriDefinedSingular Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    REAL_A_10,
    REAL_A_1,
    // expected
    OGRealDenseMatrix::create( { {0.2941176470588236, 0.5882352941176472, 0.8823529411764708 }, {-0.1303167420814479, -0.2606334841628958, -0.3909502262443438 },
      {-0.0162895927601810, -0.0325791855203620, -0.0488687782805430 } }),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    CMPLX_A_10,
    CMPLX_A_1,
    // expected
    // NOTE: the "zero" values here are highly dubious as they are suceptible to fp issues
    OGComplexDenseMatrix::create({{{0.2941176470588236,-0.0000000000000001},{0.5882352941176472,-0.0000000000000002},{0.8823529411764708,-0.0000000000000005}},{{-0.1303167420814480,0.0000000000000001},{-0.2606334841628959,0.0000000000000002},{-0.3909502262443439,0.0000000000000006}},{{-0.0162895927601810,0.0},{-0.0325791855203620,0.0},{-0.0488687782805430,0.0}}}),
    MATHSEQUAL
  ),

  //test TriMachPrecSingular Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    REAL_A_11,
    REAL_A_1,
    // expected
    OGRealDenseMatrix::create({ {0.2941176470588236, 0.5882352941176472, 0.8823529411764708 }, {-0.1303167420814479, -0.2606334841628958, -0.3909502262443438 },      {-0.0162895927601810, -0.0325791855203620, -0.0488687782805430 } }),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    CMPLX_A_11,
    CMPLX_C_5,
    // expected
    // NOTE: numbers straight from plumbing into fortran
    OGComplexDenseMatrix::create({{{  0.990099009900991325E-02, -0.990099009900990285E-01},{  0.117647058823529424E+00, -0.543923179043097550E-17},{  0.506697728596388758E-01, -0.361094933022714157E-01}},{{ -0.255902513328255879E-01,  0.132825590251332781E+00},
    { -0.855158819049325336E-01, -0.353389185072353201E-01},{ -0.104583127996057484E+00,  0.250168003225661913E-01}},{{ -0.319878141660320066E-02,  0.166031987814166011E-01},{ -0.106894852381165684E-01, -0.441736481340441502E-02},{ -0.130728909995071873E-01,  0.312710004032077304E-02}}}),
    MATHSEQUAL
  ),

  //test LUP Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    REAL_A_3,
    REAL_A_1,
    // expected
    OGRealDenseMatrix::create({ {0.0812641083521445, 0.1625282167042890, 0.2437923250564334 }, {0.0609480812641084, 0.1218961625282167, 0.1828442437923251 },
      {0.0654627539503386, 0.1309255079006772, 0.1963882618510158 } }),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    CMPLX_A_3,
    CMPLX_A_1,
    // expected
    OGComplexDenseMatrix::create({{{0.0812641083521445,0.0},{0.1625282167042889,0.0},{0.2437923250564334,0.0}},{{0.0609480812641084,0.0},{0.1218961625282167,0.0},{0.1828442437923251,0.0}},{{0.0654627539503386,0.0},{0.1309255079006772,0.0},{0.1963882618510158,0.0}}}),
    MATHSEQUAL
  ),

  //test LUPBranchFallToLSQ Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    REAL_A_5,
    REAL_A_1,
    // expected
    OGRealDenseMatrix::create({ {0.2000000000000003, 0.4000000000000006, 0.6000000000000013 }, {0.4000000000000002, 0.8000000000000004, 1.2000000000000017 },
      {-0.0000000000000001, -0.0000000000000001, 0.0000000000000000 } }),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    CMPLX_A_5,
    CMPLX_A_1,
    // expected
    OGComplexDenseMatrix::create({{{0.2000000000000012,-0.0},{0.4000000000000024,-0.0},{0.6000000000000035,-0.0000000000000001}},{{0.4000000000000009,0.0},{0.8000000000000017,0.0},{1.2000000000000024,0.0}},{{0.0,-0.0},{0.0,-0.0},{0.0000000000000002,-0.0}}}),
    MATHSEQUAL
  ),

   //test Chol Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    REAL_A_2,
    REAL_A_1,
    // expected
    OGRealDenseMatrix::create({ {0.0059171597633136, 0.0118343195266272, 0.0177514792899408 }, {0.0059171597633136, 0.0118343195266272, 0.0177514792899408 },
      {0.0059171597633136, 0.0118343195266272, 0.0177514792899408 } }),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    CMPLX_A_2,
    CMPLX_A_1,
    // expected
    OGComplexDenseMatrix::create({{{0.0510841602352076,0.2881293642043367},{0.1021683204704153,0.5762587284086734},{0.1532524807056229,0.8643880926130100}},{{0.0499816244027931,0.3142227122381478},{0.0999632488055862,0.6284454244762956},{0.1499448732083793,0.9426681367144433}},{{0.0481440646821022,0.8897464167585445},{0.0962881293642043,1.7794928335170890},{0.1444321940463065,2.6692392502756337}}}),
    MATHSEQUAL
  ),

   //test Chol Not SPD Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    REAL_A_13,
    REAL_A_1,
    // expected
    OGRealDenseMatrix::create({ {0.0150267040825563, 0.0300534081651127, 0.0450801122476691 }, {0.0988051054584955, 0.1976102109169910, 0.2964153163754866 },
      {-0.0030174104583446, -0.0060348209166893, -0.0090522313750339 } }),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    CMPLX_A_13,
    CMPLX_A_1,
    // expected
    OGComplexDenseMatrix::create({{{0.1277309984054606,0.1753993342098637},{0.2554619968109212,0.3507986684197275},{0.3831929952163817,0.5261980026295914}},{{-0.2394186924776904,0.2617002825411923},{-0.4788373849553809,0.5234005650823845},{-0.7182560774330713,0.7851008476235766}},{{0.1575889818259807,0.0219132607864529},{0.3151779636519615,0.0438265215729059},{0.4727669454779422,0.0657397823593588}}}),
    MATHSEQUAL
  ),

   //test Chol Singular Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    REAL_A_12,
    REAL_A_1,
    // expected
    OGRealDenseMatrix::create({ {0, 0, 0 }, {1e-300, 2e-300, 3e-300 }, {0, 0, 0 } }),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    CMPLX_A_12,
    CMPLX_A_1,
    // expected
    // NOTE: getting here is quite hard...
    // need something Hermitian AND p.d. AND with a bad 1 norm condition estimate
    OGComplexDenseMatrix::create({{{0.0e0,0.0e0},{0.0e0,0.0e0},{0.0e0,0.0e0}},{{0.0e0,0.0e0},{0.0e0,0.0e0},{0.0e0,0.0e0}},{{0.0e0,0.0e0},{0.0e0,0.0e0},{0.0e0,0.0e0}}}),
    MATHSEQUAL
  ),

  //test DGELS branch Expansion
  // this branch tests the necessary expansion of the "B" matrix if the solution to the system is larger than the mallocd space
  new CheckBinary<MLDIVIDE>
  (
    // input
    REAL_A_19,
    REAL_A_19,
    // expected
    OGRealDenseMatrix::create({ {0.1379310344827586, 0.2068965517241379, 0.2758620689655172 },
      {0.2068965517241379, 0.3103448275862069, 0.4137931034482758 }, {0.2758620689655172, 0.4137931034482757, 0.5517241379310344 } }),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    CMPLX_A_19,
    CMPLX_A_19,
    // expected
    OGComplexDenseMatrix::create({{{0.1379310344827586,0.0},{0.2068965517241380,0.0},{0.2758620689655172,0.0}},{{0.2068965517241379,0.0},{0.3103448275862069,0.0},{0.4137931034482759,0.0}},{{0.2758620689655172,0.0},{0.4137931034482760,0.0},{0.5517241379310345,0.0}}}),
    MATHSEQUAL
  ),

   //test SVD Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    REAL_A_1,
    REAL_A_2,
    // expected
    OGRealDenseMatrix::create({ {4.0238095238095219, 4.0238095238095228, 4.0238095238095219 }, {8.0476190476190457, 8.0476190476190474, 8.0476190476190457 },
      {12.0714285714285658, 12.0714285714285694, 12.0714285714285676 } }),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    CMPLX_A_1,
    CMPLX_A_2,
    // expected
    // NOTE: Solving this system on Fedora boxes manually via LAPACK calls and not using
    // our LAPACK/BLAS will likely result in wild results. Fedora uses ATLAS LAPACK and
    // ATLAS BLAS if linked with just -llapack. To obtain the result below, one needs to link
    // with -llapack -lblas such that the system /usr/lib64/libblas.so BLAS is used as it has
    // superior (IIRC, and this is stretching my memory back 2 years), Householder routines).
    OGComplexDenseMatrix::create({{{0.0037718057520038,-0.0615275813295615},{0.0075436115040075,-0.0754361150400754},{0.0056577086280057,-0.0327675624705328}},{{0.0075436115040076,-0.1230551626591231},{0.0150872230080151,-0.1508722300801509},{0.0113154172560113,-0.0655351249410655}},{{0.0113154172560114,-0.1845827439886846},{0.0226308345120226,-0.2263083451202263},{0.0169731258840169,-0.0983026874115983}}}),
    MATHSEQUAL
  ),

   //test QR fail, then SVD xgelsd answer size re-uses memory Branch
  new CheckBinary<MLDIVIDE>
  (
    // input
    REAL_A_20,
    REAL_A_20,
    // expected
    OGRealDenseMatrix::create({{0.0,0.0,0.0000000000005000},{0.0,0.0000000000005625,0.0000007500000000},{0.0000000000005000,0.0000007500000000,0.9999999999994372}}),
    MATHSEQUAL
  ),
  new CheckBinary<MLDIVIDE>
  (
    // input
    CMPLX_A_20,
    CMPLX_A_20,
    // expected
    OGComplexDenseMatrix::create({{{0.0,0.0},{0.0,0.0},{0.0,0.0}},{{0.0,-0.0},{0.0,0.0},{0.0,0.0}},{{0.0,-0.0},{0.0,-0.0},{1.0000000000000002,0.0}}}),
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
  OGExpr::Ptr op = MLDIVIDE::create(REAL_A_4, REAL_B_1);
  runtree(op);
  OGExpr::Ptr  reconstr = PLUS::create(NEGATE::create(REAL_B_1), MTIMES::create(REAL_A_4, op->getRegs()[0]->asOGTerminal()));
  runtree(reconstr);
  OGTerminal::Ptr zeros = OGRealDenseMatrix::create({{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}});
  reconstr->getRegs()[0]->asOGTerminal()->debug_print();
  EXPECT_TRUE(zeros->mathsequals(reconstr->getRegs()[0]->asOGTerminal(),1e-14,1e-14));
}