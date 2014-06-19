/**
 * Copyright (C) 2012 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.maths.testnodes;

import static org.testng.Assert.assertTrue;

import java.util.Arrays;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.maths.DOGMA;
import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.OGTerminal;
import com.opengamma.maths.datacontainers.lazy.OGExpr;
import com.opengamma.maths.datacontainers.matrix.OGArray;
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.maths.datacontainers.other.ComplexArrayContainer;
import com.opengamma.maths.exceptions.MathsException;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.exceptions.MathsExceptionNonConformance;
import com.opengamma.maths.helpers.FuzzyEquals;
import com.opengamma.maths.materialisers.Materialisers;
import com.opengamma.maths.nodes.MLDIVIDE;
import com.opengamma.maths.nodes.MTIMES;
import com.opengamma.maths.nodes.NEGATE;
import com.opengamma.maths.nodes.PLUS;

/**
 * Test for mldivide()
 */
@Test
public class TestMldivideMaterialise {
  boolean debugStatements = false; // eyeballable output switch

  private double[][] A_square_singular = new double[][] { { 1.00, 2.00, 3.00 }, { 1.00, 2.00, 3.00 }, { 1.00, 2.00, 3.00 } };
  private double[][] A_square_symmetric_positive_definite = new double[][] { { 123.00, 23.00, 23.00 }, { 23.00, 123.00, 23.00 }, { 23.00, 23.00, 123.00 } };
  private double[][] A_square_non_symmetric_well_conditioned = new double[][] { { 10.00, 2.00, 1.00 }, { 2.00, 3.00, 10.00 }, { 4.00, 10.00, 1.00 } };
  private double[][] A_rectangular = new double[][] { { 1.00, 2.00, 3.00, 4.00 }, { 5.00, 6.00, 7.00, 8.00 }, { 9.00, 10.00, 11.00, 12.00 }, { 13.00, 14.00, 15.00, 16.00 },
    { 17.00, 18.00, 19.00, 20.00 } };
  private double[][] A_square_non_symmetric_condition_bad_for_lu_ok_for_qr = new double[][] { { 1.0000000000000009, 2., 20. }, { 1., 2., 0. }, { 1., 2., 40. } };
  private double[][] A_upper_triangular = new double[][] { { 1., 2., 3. }, { 0., 5., 6. }, { 0., 0., 9. } };;
  private double[][] A_unit_upper_triangular = new double[][] { { 1., 2., 3. }, { 0., 1., 6. }, { 0., 0., 1. } };
  private double[][] A_lower_triangular = new double[][] { { 1., 0., 0. }, { 4., 5., 0. }, { 7., 8., 9. } };
  private double[][] A_unit_lower_triangular = new double[][] { { 1., 0., 0. }, { 4., 1., 0. }, { 7., 8., 1. } };
  private double[][] A_lower_triangular_singular_zero_on_diag = new double[][] { { 1., 0., 0. }, { 4., 0., 0. }, { 7., 8., 1. } };
  private double[][] A_lower_triangular_singular_near_zero_on_diag = new double[][] { { 1., 0., 0. }, { 4., 0.0000000000000001, 0. }, { 7., 8., 1. } };
  private double[][] A_square_spd_near_singular = new double[][] { { 2.0000e+00, 1.0000e+150, 2.0000e+00 }, { 1.0000e+150, 1.0000e+300, 2.0000e+150 }, { 2.0000e+00, 2.0000e+150, 5.0000e+00 } };
  private double[][] A_square_symmetric_not_spd = new double[][] { { 54., 2., 3. }, { 2., 10., 6. }, { 3., 6., -120. } };
  private double[][] A_rectangular_with_inf = new double[][] { { Double.POSITIVE_INFINITY, 2.00, 3.00, 4.00 }, { 5.00, 6.00, 7.00, 8.00 }, { 9.00, 10.00, 11.00, 12.00 },
    { 13.00, 14.00, 15.00, 16.00 },
    { 17.00, 18.00, 19.00, 20.00 } };
  private double[][] A_square_permuted_upper_triangular = new double[][] { { 0., 0., 13., 14., 15. }, { 1., 2., 3., 4., 5. }, { 0., 7., 8., 9., 10. }, { 0., 0., 0., 0., 25. },
    { 0., 0., 0., 19., 20. } };;
  private double[][] A_square_permuted_upper_triangular_zero_on_diag = new double[][] { { 0., 6., 7., 8. }, { 0., 0., 0., 16. }, { 1., 2., 3., 4. }, { 0., 0., 0., 12. } };
  private double[][] A_square_almost_permuted_upper_triangular = new double[][] { { 0., 12., 13., 14., 15. }, { 1., 2., 3., 4., 5. }, { 0., 7., 8., 9., 10. }, { 0., 0., 0., 0., 25. },
    { 0., 0., 0., 19., 20. } };;
  private double[][] A_square_permuted_upper_triangular_singular_to_mach_prech = new double[][] { { 0., 0., 1.e-300, 14., 15. }, { 1., 2., 3., 4., 5. }, { 0., 7., 8., 9., 10. },
    { 0., 0., 0., 0., 25. },
    { 0., 0., 0., 19., 20. } };;
  private double[][] A_short_row = new double[][] { { 2, 3, 4 } };

  private double[][] B_rectangular = new double[][] { { 1.00, 2.00, 3.00 }, { 1.00, 2.00, 3.00 }, { 1.00, 2.00, 3.00 }, { 1.00, 2.00, 3.00 }, { 1.00, 2.00, 3.00 } };

  private double[] C_4len_col_vector = new double[] { 1, 2, 3, 4 };
  private double[] C_5len__shuffled_col_matrix = new double[] { 3, 1, 2, 5, 4, 10, 20, 30, 40, 50 };

  OGTerminal A1 = new OGRealDenseMatrix(A_square_singular);
  OGTerminal A2 = new OGRealDenseMatrix(A_square_symmetric_positive_definite);
  OGTerminal A3 = new OGRealDenseMatrix(A_square_non_symmetric_well_conditioned);
  OGTerminal A4 = new OGRealDenseMatrix(A_rectangular);
  OGTerminal A5 = new OGRealDenseMatrix(A_square_non_symmetric_condition_bad_for_lu_ok_for_qr);
  OGTerminal A6 = new OGRealDenseMatrix(A_upper_triangular);
  OGTerminal A7 = new OGRealDenseMatrix(A_unit_upper_triangular);
  OGTerminal A8 = new OGRealDenseMatrix(A_lower_triangular);
  OGTerminal A9 = new OGRealDenseMatrix(A_unit_lower_triangular);
  OGTerminal A10 = new OGRealDenseMatrix(A_lower_triangular_singular_zero_on_diag);
  OGTerminal A11 = new OGRealDenseMatrix(A_lower_triangular_singular_near_zero_on_diag);
  OGTerminal A12 = new OGRealDenseMatrix(A_square_spd_near_singular);
  OGTerminal A13 = new OGRealDenseMatrix(A_square_symmetric_not_spd);
  OGTerminal A14 = new OGRealDenseMatrix(A_rectangular_with_inf);
  OGTerminal A15 = new OGRealDenseMatrix(A_square_permuted_upper_triangular);
  OGTerminal A16 = new OGRealDenseMatrix(A_square_permuted_upper_triangular_zero_on_diag);
  OGTerminal A17 = new OGRealDenseMatrix(A_square_almost_permuted_upper_triangular);
  OGTerminal A18 = new OGRealDenseMatrix(A_square_permuted_upper_triangular_singular_to_mach_prech);
  OGTerminal A19 = new OGRealDenseMatrix(A_short_row);

  OGTerminal B1 = new OGRealDenseMatrix(B_rectangular);
  OGTerminal C1 = new OGRealDenseMatrix(C_4len_col_vector, 4, 1);
  OGTerminal C2 = new OGRealDenseMatrix(C_5len__shuffled_col_matrix, 5, 2);

  @Test(expectedExceptions = MathsExceptionNonConformance.class)
  public void testNonConformance() {
    DOGMA.toDoubleArrayOfArrays(new MLDIVIDE(A1, B1));
  }

  // valid ops for which we have answers
  @DataProvider
  public Object[][] realDataContainer() {
    Object[][] obj = new Object[18][2];

    //test UpperTri Branch
    obj[0][0] = new MLDIVIDE(A6, A1);
    obj[0][1] = new OGRealDenseMatrix(new double[][] { { 0.5333333333333334, 1.0666666666666669, 1.6000000000000001 }, { 0.0666666666666667, 0.1333333333333334, 0.2000000000000000 },
      { 0.1111111111111111, 0.2222222222222222, 0.3333333333333333 } });

    // test Unit UpperTri Branch
    obj[1][0] = new MLDIVIDE(A7, A1);
    obj[1][1] = new OGRealDenseMatrix(new double[][] { { 8.0000000000000000, 16.0000000000000000, 24.0000000000000000 },
      { -5.0000000000000000, -10.0000000000000000, -15.0000000000000000 },
      { 1.0000000000000000, 2.0000000000000000, 3.0000000000000000 } });

    // test LowerTri Branch
    obj[2][0] = new MLDIVIDE(A8, A1);
    obj[2][1] = new OGRealDenseMatrix(new double[][] { { 1.0000000000000000, 2.0000000000000000, 3.0000000000000000 },
      { -0.6000000000000000, -1.2000000000000000, -1.8000000000000000 },
      { -0.1333333333333334, -0.2666666666666667, -0.4000000000000000 } });

    // test Unit LowerTri Branch
    obj[3][0] = new MLDIVIDE(A9, A1);
    obj[3][1] = new OGRealDenseMatrix(new double[][] { { 1.0000000000000000, 2.0000000000000000, 3.0000000000000000 },
      { -3.0000000000000000, -6.0000000000000000, -9.0000000000000000 },
      { 18.0000000000000000, 36.0000000000000000, 54.0000000000000000 } });

    // test Permuted Upper Triangular Branch
    obj[4][0] = new MLDIVIDE(A15, C2);
    obj[4][1] = new OGRealDenseMatrix(new double[][] { { 0.0000000000000000, 8.1445922498554086 }, { 0.0000000000000000, 3.1787160208212839 },
      { 0.0000000000000000, -2.0971659919028340 },
      { 0.0000000000000000, 0.9473684210526315 }, { 0.2000000000000000, 1.6000000000000001 } });

    // test Almost Permuted Upper Triangular Branch
    obj[5][0] = new MLDIVIDE(A17, C2);
    obj[5][1] = new OGRealDenseMatrix(new double[][] { { 0.0000000000000002, -29.9999999999998650 }, { 0.0000000000000004, -57.8526315789472108 },
      { -0.0000000000000004, 51.3052631578945864 },
      { 0.0000000000000000, 0.9473684210526315 }, { 0.2000000000000000, 1.6000000000000001 } });

    // test Permuted Upper Triangular Zero On Diag Branch
    obj[6][0] = new MLDIVIDE(A16, C1);
    obj[6][1] = new OGRealDenseMatrix(new double[][] { { 2.0475247524752480 }, { -0.7168316831683170 }, { 0.5287128712871273 }, { 0.2000000000000003 } });

    // test Permuted Upper Triangular Singular To Mach Prec Branch
    obj[7][0] = new MLDIVIDE(A18, C2);
    obj[7][1] = new OGRealDenseMatrix(new double[][] { { -0.0000000000000001, 5.7657712505229739 }, { -0.0000000000000001, -1.3196460795883791 },
      { -0.0000000000000001, 2.6102410879868061 },
      { 0.0000000000000000, 0.2699985638374270 }, { 0.2000000000000001, 1.5925606778687353 } });

    // test Tri Defined Singular Branch
    obj[8][0] = new MLDIVIDE(A10, A1);
    obj[8][1] = new OGRealDenseMatrix(new double[][] { { 0.2941176470588236, 0.5882352941176472, 0.8823529411764708 },
      { -0.1303167420814479, -0.2606334841628958, -0.3909502262443438 },
      { -0.0162895927601810, -0.0325791855203620, -0.0488687782805430 } });

    // test Tri Mach Prec Singular Branch
    obj[9][0] = new MLDIVIDE(A11, A1);
    obj[9][1] = new OGRealDenseMatrix(new double[][] { { 0.2941176470588236, 0.5882352941176472, 0.8823529411764708 },
      { -0.1303167420814479, -0.2606334841628958, -0.3909502262443438 },
      { -0.0162895927601810, -0.0325791855203620, -0.0488687782805430 } });

    // test LUP Branch
    obj[10][0] = new MLDIVIDE(A3, A1);
    obj[10][1] = new OGRealDenseMatrix(new double[][] { { 0.0812641083521445, 0.1625282167042890, 0.2437923250564334 }, { 0.0609480812641084, 0.1218961625282167, 0.1828442437923251 },
      { 0.0654627539503386, 0.1309255079006772, 0.1963882618510158 } });

    // test LUP Branch Fall To LSQ() {
    obj[11][0] = new MLDIVIDE(A5, A1);
    obj[11][1] = new OGRealDenseMatrix(new double[][] { { 0.2000000000000003, 0.4000000000000006, 0.6000000000000013 }, { 0.4000000000000002, 0.8000000000000004, 1.2000000000000017 },
      { -0.0000000000000001, -0.0000000000000001, 0.0000000000000000 } });

    // test Chol Branch
    obj[12][0] = new MLDIVIDE(A2, A1);
    obj[12][1] = new OGRealDenseMatrix(new double[][] { { 0.0059171597633136, 0.0118343195266272, 0.0177514792899408 }, { 0.0059171597633136, 0.0118343195266272, 0.0177514792899408 },
      { 0.0059171597633136, 0.0118343195266272, 0.0177514792899408 } });

    // test Chol Not SPD Branch() {
    obj[13][0] = new MLDIVIDE(A13, A1);
    obj[13][1] = new OGRealDenseMatrix(new double[][] { { 0.0150267040825563, 0.0300534081651127, 0.0450801122476691 }, { 0.0988051054584955, 0.1976102109169910, 0.2964153163754866 },
      { -0.0030174104583446, -0.0060348209166893, -0.0090522313750339 } });

    // test Chol Singular Branch
    obj[14][0] = new MLDIVIDE(A12, A1);
    obj[14][1] = new OGRealDenseMatrix(new double[][] { { 0, 0, 0 }, { 1e-300, 2e-300, 3e-300 }, { 0, 0, 0 } });

    // test QR Branch
    // this is a right pain to test because its rank deficient the results are pulled about by floating point behaviour in deficient part of Q
    // we test by reconstruction opposed to value
    obj[15][0] = new PLUS(B1, new NEGATE(new MTIMES(A4, new MLDIVIDE(A4, B1))));
    obj[15][1] = new OGRealDenseMatrix(new double[B_rectangular.length][B_rectangular[0].length]);

    // test SVD Branch
    obj[16][0] = new MLDIVIDE(A1, A2);
    obj[16][1] = new OGRealDenseMatrix(new double[][] { { 4.0238095238095219, 4.0238095238095228, 4.0238095238095219 }, { 8.0476190476190457, 8.0476190476190474, 8.0476190476190457 },
      { 12.0714285714285658, 12.0714285714285694, 12.0714285714285676 } });

    // this branch tests the necessary expansion of the "B" matrix if the solution to the system is larger than the mallocd space
    // test DGELS branch Expansion
    obj[17][0] = new MLDIVIDE(A19, A19);
    obj[17][1] = new OGRealDenseMatrix(new double[][] { { 0.1379310344827586, 0.2068965517241379, 0.2758620689655172 },
      { 0.2068965517241379, 0.3103448275862069, 0.4137931034482758 }, { 0.2758620689655172, 0.4137931034482757, 0.5517241379310344 } });

    return obj;
  }

  //  @Test
  //  public void testUpperTriBranch() {
  //    OGTerminal answer = new MLDIVIDE(A6, A1);
  //    OGRealDenseMatrix expected = new OGRealDenseMatrix(new double[][] { {0.5333333333333334, 1.0666666666666669, 1.6000000000000001 }, {0.0666666666666667, 0.1333333333333334, 0.2000000000000000 },
  //      {0.1111111111111111, 0.2222222222222222, 0.3333333333333333 } });
  //    assertTrue(answer.fuzzyequals(expected, 10 * D1mach.four()));
  //    if (debugStatements) {
  //      System.out.println(A6.toString());
  //      System.out.println(A1.toString());
  //      System.out.println(answer.toString());
  //    }
  //  }

  //  @Test
  //  public void testUnitUpperTriBranch() {
  //    OGNumeric answer = new MLDIVIDE(A7, A1);
  //    OGRealDenseMatrix expected = new OGRealDenseMatrix(new double[][] { { 8.0000000000000000, 16.0000000000000000, 24.0000000000000000 },
  //      { -5.0000000000000000, -10.0000000000000000, -15.0000000000000000 },
  //      { 1.0000000000000000, 2.0000000000000000, 3.0000000000000000 } });
  //    assertTrue(answer.fuzzyequals(expected, 10 * D1mach.four()));
  //    if (debugStatements) {
  //      System.out.println(A7.toString());
  //      System.out.println(A1.toString());
  //      System.out.println(answer.toString());
  //    }
  //  }

  //  @Test
  //  public void testLowerTriBranch() {
  //    OGNumeric answer = new MLDIVIDE(A8, A1);
  //    OGRealDenseMatrix expected = new OGRealDenseMatrix(new double[][] { { 1.0000000000000000, 2.0000000000000000, 3.0000000000000000 },
  //      { -0.6000000000000000, -1.2000000000000000, -1.8000000000000000 },
  //      { -0.1333333333333334, -0.2666666666666667, -0.4000000000000000 } });
  //    assertTrue(answer.fuzzyequals(expected, 10 * D1mach.four()));
  //    if (debugStatements) {
  //      System.out.println(A8.toString());
  //      System.out.println(A1.toString());
  //      System.out.println(answer.toString());
  //    }
  //  }

  //  @Test
  //  public void testUnitLowerTriBranch() {
  //    OGNumeric answer = new MLDIVIDE(A9, A1);
  //    OGRealDenseMatrix expected = new OGRealDenseMatrix(new double[][] { { 1.0000000000000000, 2.0000000000000000, 3.0000000000000000 },
  //      { -3.0000000000000000, -6.0000000000000000, -9.0000000000000000 },
  //      { 18.0000000000000000, 36.0000000000000000, 54.0000000000000000 } });
  //    assertTrue(answer.fuzzyequals(expected, 10 * D1mach.four()));
  //    if (debugStatements) {
  //      System.out.println(A9.toString());
  //      System.out.println(A1.toString());
  //      System.out.println(answer.toString());
  //    }
  //  }

  //  @Test
  //  public void testPermutedUpperTriangularBranch() {
  //    OGNumeric answer = new MLDIVIDE(A15, C2);
  //    OGRealDenseMatrix expected = new OGRealDenseMatrix(new double[][] { { 0.0000000000000000, 8.1445922498554086 }, { 0.0000000000000000, 3.1787160208212839 },
  //      { 0.0000000000000000, -2.0971659919028340 },
  //      { 0.0000000000000000, 0.9473684210526315 }, { 0.2000000000000000, 1.6000000000000001 } });
  //    assertTrue(answer.fuzzyequals(expected, 10 * D1mach.four()));
  //    if (debugStatements) {
  //      System.out.println("A15" + A15.toString());
  //      System.out.println("C2" + C2.toString());
  //      System.out.println(answer.toString());
  //    }
  //  }

  //  @Test
  //  public void testAlmostPermutedUpperTriangularBranch() {
  //    OGNumeric answer = new MLDIVIDE(A17, C2);
  //    OGRealDenseMatrix expected = new OGRealDenseMatrix(new double[][] { { 0.0000000000000002, -29.9999999999998650 }, { 0.0000000000000004, -57.8526315789472108 },
  //      { -0.0000000000000004, 51.3052631578945864 },
  //      { 0.0000000000000000, 0.9473684210526315 }, { 0.2000000000000000, 1.6000000000000001 } });
  //    assertTrue(answer.fuzzyequals(expected, 100 * D1mach.four()));
  //    if (debugStatements) {
  //      System.out.println("A17" + A17.toString());
  //      System.out.println("C2" + C2.toString());
  //      System.out.println(answer.toString());
  //    }
  //  }

  //  @Test
  //  public void testPermutedUpperTriangularZeroOnDiagBranch() {
  //    OGNumeric answer = new MLDIVIDE(A16, C1);
  //    OGRealDenseMatrix expected = new OGRealDenseMatrix(new double[][] { { 2.0475247524752480 }, { -0.7168316831683170 }, { 0.5287128712871273 }, { 0.2000000000000003 } });
  //    assertTrue(answer.fuzzyequals(expected, 10 * D1mach.four()));
  //    if (debugStatements) {
  //      System.out.println(A16.toString());
  //      System.out.println(C1.toString());
  //      System.out.println(answer.toString());
  //    }
  //  }

  //  @Test
  //  public void testPermutedUpperTriangularSingularToMachPrecBranch() {
  //    OGNumeric answer = new MLDIVIDE(A18, C2);
  //    OGRealDenseMatrix expected = new OGRealDenseMatrix(new double[][] { { -0.0000000000000001, 5.7657712505229739 }, { -0.0000000000000001, -1.3196460795883791 },
  //      { -0.0000000000000001, 2.6102410879868061 },
  //      { 0.0000000000000000, 0.2699985638374270 }, { 0.2000000000000001, 1.5925606778687353 } });
  //    assertTrue(answer.fuzzyequals(expected, 100 * D1mach.four()));
  //    if (debugStatements) {
  //      System.out.println(A18.toString());
  //      System.out.println(C2.toString());
  //      System.out.println(answer.toString());
  //    }
  //  }

  //  @Test
  //  public void testTriDefinedSingularBranch() {
  //    OGNumeric answer = new MLDIVIDE(A10, A1);
  //    OGRealDenseMatrix expected = new OGRealDenseMatrix(new double[][] { { 0.2941176470588236, 0.5882352941176472, 0.8823529411764708 },
  //      { -0.1303167420814479, -0.2606334841628958, -0.3909502262443438 },
  //      { -0.0162895927601810, -0.0325791855203620, -0.0488687782805430 } });
  //    assertTrue(answer.fuzzyequals(expected, 10 * D1mach.four()));
  //    if (debugStatements) {
  //      System.out.println(A10.toString());
  //      System.out.println(A1.toString());
  //      System.out.println(answer.toString());
  //    }
  //  }

  //  @Test
  //  public void testTriMachPrecSingularBranch() {
  //    OGNumeric answer = new MLDIVIDE(A11, A1);
  //    OGRealDenseMatrix expected = new OGRealDenseMatrix(new double[][] { { 0.2941176470588236, 0.5882352941176472, 0.8823529411764708 },
  //      { -0.1303167420814479, -0.2606334841628958, -0.3909502262443438 },
  //      { -0.0162895927601810, -0.0325791855203620, -0.0488687782805430 } });
  //    assertTrue(answer.fuzzyequals(expected, 10 * D1mach.four()));
  //    if (debugStatements) {
  //      System.out.println(A11.toString());
  //      System.out.println(A1.toString());
  //      System.out.println(answer.toString());
  //    }
  //  }
  //
  //  @Test
  //  public void testLUPBranch() {
  //    OGNumeric answer = new MLDIVIDE(A3, A1);
  //    OGRealDenseMatrix expected = new OGRealDenseMatrix(new double[][] { { 0.0812641083521445, 0.1625282167042890, 0.2437923250564334 }, { 0.0609480812641084, 0.1218961625282167, 0.1828442437923251 },
  //      { 0.0654627539503386, 0.1309255079006772, 0.1963882618510158 } });
  //    assertTrue(answer.fuzzyequals(expected, 10 * D1mach.four()));
  //    if (debugStatements) {
  //      System.out.println(A3.toString());
  //      System.out.println(A1.toString());
  //      System.out.println(answer.toString());
  //    }
  //  }

  //  @Test
  //  public void testLUPBranchFallToLSQ() {
  //    OGNumeric answer = new MLDIVIDE(A5, A1);
  //    OGRealDenseMatrix expected = new OGRealDenseMatrix(new double[][] { { 0.2000000000000003, 0.4000000000000006, 0.6000000000000013 }, { 0.4000000000000002, 0.8000000000000004, 1.2000000000000017 },
  //      { -0.0000000000000001, -0.0000000000000001, 0.0000000000000000 } });
  //    assertTrue(answer.fuzzyequals(expected, 10 * D1mach.four()));
  //    if (debugStatements) {
  //      System.out.println(A5.toString());
  //      System.out.println(A1.toString());
  //      System.out.println(answer.toString());
  //    }
  //  }
  //
  //  @Test
  //  public void testCholBranch() {
  //    OGNumeric answer = new MLDIVIDE(A2, A1);
  //    OGRealDenseMatrix expected = new OGRealDenseMatrix(new double[][] { { 0.0059171597633136, 0.0118343195266272, 0.0177514792899408 }, { 0.0059171597633136, 0.0118343195266272, 0.0177514792899408 },
  //      { 0.0059171597633136, 0.0118343195266272, 0.0177514792899408 } });
  //    assertTrue(answer.fuzzyequals(expected, 10 * D1mach.four()));
  //    if (debugStatements) {
  //      System.out.println(A1.toString());
  //      System.out.println(A1.toString());
  //      System.out.println(answer.toString());
  //    }
  //  }

  //  @Test
  //  public void testCholNotSPDBranch() {
  //    OGNumeric answer = new MLDIVIDE(A13, A1);
  //    OGRealDenseMatrix expected = new OGRealDenseMatrix(new double[][] { { 0.0150267040825563, 0.0300534081651127, 0.0450801122476691 }, { 0.0988051054584955, 0.1976102109169910, 0.2964153163754866 },
  //      { -0.0030174104583446, -0.0060348209166893, -0.0090522313750339 } });
  //    assertTrue(answer.fuzzyequals(expected, 10 * D1mach.four()));
  //    if (debugStatements) {
  //      System.out.println(A13.toString());
  //      System.out.println(A1.toString());
  //      System.out.println(answer.toString());
  //    }
  //  }

  //  @Test
  //  public void testCholSingularBranch() {
  //    OGNumeric answer = new MLDIVIDE(A12, A1);
  //    OGRealDenseMatrix expected = new OGRealDenseMatrix(new double[][] { { 0, 0, 0 }, { 1e-300, 2e-300, 3e-300 }, { 0, 0, 0 } });
  //    assertTrue(answer.fuzzyequals(expected, 10 * D1mach.four()));
  //    if (debugStatements) {
  //      System.out.println(A12.toString());
  //      System.out.println(A1.toString());
  //      System.out.println(answer.toString());
  //    }
  //  }

  // this is a right pain to test because its rank deficient the results are pulled about by floating point behaviour in deficient part of Q
  // we test by reconstruction opposed to absolute value
  //  @Test
  //  public void testQRBranch() {
  //    OGNumeric answer = new MLDIVIDE(A4, B1);
  //    OGArray<? extends Number> zeros = DOGMA.zeros(B1.getNumberOfRows(), B1.getNumberOfColumns());
  //    OGArray<? extends Number> reconstruct = DOGMA.minus(B1, DOGMA.mtimes(A4, answer));
  //    assertTrue(reconstruct.fuzzyequals(zeros, 100 * D1mach.four()));
  //    if (debugStatements) {
  //      System.out.println(answer.toString());
  //    }
  //  }

  //  @Test
  //  public void testSVDBranch() {
  //    OGNumeric answer = new MLDIVIDE(A1, A2);
  //    OGRealDenseMatrix expected = new OGRealDenseMatrix(new double[][] { { 4.0238095238095219, 4.0238095238095228, 4.0238095238095219 }, { 8.0476190476190457, 8.0476190476190474, 8.0476190476190457 },
  //      { 12.0714285714285658, 12.0714285714285694, 12.0714285714285676 } });
  //    assertTrue(answer.fuzzyequals(expected, 10 * D1mach.four()));
  //    if (debugStatements) {
  //      System.out.println(answer.toString());
  //    }
  //  }

  //  @Test
  //  // this branch tests the necessary expansion of the "B" matrix if the solution to the system is larger than the mallocd space
  //  public void testDGELSbranchExpansion() {
  //    OGNumeric answer = new MLDIVIDE(A19, A19);
  //    OGRealDenseMatrix expected = new OGRealDenseMatrix(new double[][] { { 0.1379310344827586, 0.2068965517241379, 0.2758620689655172 },
  //      { 0.2068965517241379, 0.3103448275862069, 0.4137931034482758 }, { 0.2758620689655172, 0.4137931034482757, 0.5517241379310344 } });
  //    assertTrue(answer.fuzzyequals(expected, 10 * D1mach.four()));
  //    if (debugStatements) {
  //      System.out.println(answer.toString());
  //    }
  //  }

  @Test(dataProvider = "realDataContainer")
  public void materialiseToJDoubleArray(OGNumeric input, OGTerminal expected) {
    double[][] answer = Materialisers.toDoubleArrayOfArrays(input);
    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toDoubleArrayOfArrays(expected), answer)) {
      throw new MathsException("Arrays not equal. Got: " + new OGRealDenseMatrix(answer).toString() + " Expected: " + expected.toString());
    }
  }

  @Test(dataProvider = "complexDataContainer")
  public void materialiseToComplexArrayContainer(OGNumeric input, OGTerminal expected) {
    ComplexArrayContainer answer = Materialisers.toComplexArrayContainer(input);
    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toComplexArrayContainer(expected).getReal(), answer.getReal())) {
      throw new MathsException("Arrays not equal. Got: " + new OGRealDenseMatrix(answer.getReal()).toString() + " Expected: " + Arrays.toString(expected.getData()));
    }
    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toComplexArrayContainer(expected).getImag(), answer.getImag())) {
      throw new MathsException("Arrays not equal. Got: " + new OGRealDenseMatrix(answer.getImag()).toString() + " Expected: " + Arrays.toString(expected.getData()));
    }
  }

  @Test(dataProvider = "jointdataContainer")
  public void materialiseToOGTerminal(OGNumeric input, OGTerminal expected) {
    OGTerminal answer = Materialisers.toOGTerminal(input);
    if (!expected.mathsequals(answer)) {
      throw new MathsException("Terminals not equal. Got: " + answer.toString() + " Expected: " + expected.toString());
    }
  }

  @Test(dataProvider = "jointdataContainer", expectedExceptions = MathsExceptionIllegalArgument.class)
  public void outsideArgBound(OGExpr input, OGTerminal expected) {
    input.getArg(1);
  }

}
