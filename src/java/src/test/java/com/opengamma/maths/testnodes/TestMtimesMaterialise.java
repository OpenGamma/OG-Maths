/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.testnodes;

import static org.testng.Assert.assertTrue;

import org.testng.annotations.Test;

import com.opengamma.maths.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.maths.datacontainers.other.ComplexArrayContainer;
import com.opengamma.maths.exceptions.MathsExceptionNativeComputation;
import com.opengamma.maths.helpers.FuzzyEquals;
import com.opengamma.maths.materialisers.Materialisers;
import com.opengamma.maths.nodes.MTIMES;

@Test
public class TestMtimesMaterialise {

  double[][] data0 = new double[][] { { 0.1, 0.2, 0.3 }, { 0.4, 0.5, 0.6 }, { 0.7, 0.8, 0.9 }, { 1.0, 1.1, 1.2 } };
  double[][] data1 = new double[][] { { 1., 2., 3. }, { 4., 5., 6. }, { 7., 8., 9. }, { 10., 11., 12. } };
  double[][] data2 = new double[][] { { 10., 20., 30. }, { 40., 50., 60. }, { 70., 80., 90. }, { 100., 110., 120. } };
  double[][] data3 = new double[][] { { 1., 4., 7., 10. }, { 2., 5., 8., 11. }, { 3., 6., 9., 12. } };
  double[][] data4 = new double[][] { { 10., 40., 70., 100. }, { 20., 50., 80., 110. }, { 30., 60., 90., 120. } };
  OGComplexDenseMatrix C_Single = new OGComplexDenseMatrix(new double[] { 10, 20 }, 1, 1);
  OGComplexDenseMatrix C_A = new OGComplexDenseMatrix(data0, data1);
  OGComplexDenseMatrix C_B = new OGComplexDenseMatrix(data1, data2);
  OGComplexDenseMatrix C_C = new OGComplexDenseMatrix(data3, data4);
  OGComplexDenseMatrix C_V = new OGComplexDenseMatrix(new double[][] { { 1. }, { 2. }, { 3. } }, new double[][] { { 10. }, { 20. }, { 30. } });
  OGComplexDenseMatrix C_H = new OGComplexDenseMatrix(new double[][] { { 1., 4., 7., 10. } }, new double[][] { { 10., 40., 70., 100. } });

  double[][] rp, ip;

  OGRealDenseMatrix R_Single = new OGRealDenseMatrix(10);
  OGRealDenseMatrix R_A = new OGRealDenseMatrix(new double[][] { { 1., 2., 3. }, { 4., 5., 6. }, { 7., 8., 9. }, { 10., 11., 12. } });
  OGRealDenseMatrix R_B = new OGRealDenseMatrix(new double[][] { { 10., 20., 30. }, { 40., 50., 60. }, { 70., 80., 90. }, { 100., 110., 120. } });
  OGRealDenseMatrix R_C = new OGRealDenseMatrix(new double[][] { { 1., 4., 7., 10. }, { 2., 5., 8., 11. }, { 3., 6., 9., 12. } });

  @Test
  public void Real_ScalarMtimesMatrixTest() {
    double[][] answer = Materialisers.toDoubleArrayOfArrays(new MTIMES(R_Single, R_A));
    double[][] expected = Materialisers.toDoubleArrayOfArrays(R_B);
    assertTrue(FuzzyEquals.ArrayFuzzyEquals(expected, answer));
  }

  @Test
  public void Real_MatrixMtimesScalarTest() {
    double[][] answer = Materialisers.toDoubleArrayOfArrays(new MTIMES(R_A, R_Single));
    double[][] expected = Materialisers.toDoubleArrayOfArrays(R_B);
    assertTrue(FuzzyEquals.ArrayFuzzyEquals(expected, answer));
  }

  @Test
  public void Real_MatrixMtimesMatrixTest() {
    double[][] expected = new double[][] { { 14., 32., 50., 68. }, { 32., 77., 122., 167. }, { 50., 122., 194., 266. }, { 68., 167., 266., 365. } };
    double[][] answer = Materialisers.toDoubleArrayOfArrays(new MTIMES(R_A, R_C));
    assertTrue(FuzzyEquals.ArrayFuzzyEquals(expected, answer));
  }

  @Test(expectedExceptions = MathsExceptionNativeComputation.class)
  public void Real_MatrixMtimesMatrixNonConformanceTest() {
    Materialisers.toDoubleArrayOfArrays(new MTIMES(R_A, R_B));
  }

  @Test
  public void ScalarMtimesMatrixTest() {
    rp = new double[][] { { -19., -38., -57. }, { -76., -95., -114. }, { -133., -152., -171. }, { -190., -209., -228. } };
    ip = new double[][] { { 12., 24., 36. }, { 48., 60., 72. }, { 84., 96., 108. }, { 120., 132., 144. } };
    ComplexArrayContainer answer = Materialisers.toComplexArrayContainer(new MTIMES(C_Single, C_A));
    assertTrue(FuzzyEquals.ArrayFuzzyEquals(rp, answer.getReal()));
    assertTrue(FuzzyEquals.ArrayFuzzyEquals(ip, answer.getImag()));
  }

  @Test
  public void MatrixMtimesScalarTest() {
    rp = new double[][] { { -19., -38., -57. }, { -76., -95., -114. }, { -133., -152., -171. }, { -190., -209., -228. } };
    ip = new double[][] { { 12., 24., 36. }, { 48., 60., 72. }, { 84., 96., 108. }, { 120., 132., 144. } };
    ComplexArrayContainer answer = Materialisers.toComplexArrayContainer(new MTIMES(C_A, C_Single));
    assertTrue(FuzzyEquals.ArrayFuzzyEquals(rp, answer.getReal()));
    assertTrue(FuzzyEquals.ArrayFuzzyEquals(ip, answer.getImag()));

  }

  @Test
  public void MatrixMtimesVectorTest() {
    rp = new double[][] { { -138.5999999999999943 }, { -316.7999999999999545 }, { -495.0000000000000000 }, { -673.2000000000000455 } };
    ip = new double[][] { { 28.0000000000000000 }, { 64.0000000000000000 }, { 100.0000000000000000 }, { 136.0000000000000000 } };
    ComplexArrayContainer answer = Materialisers.toComplexArrayContainer(new MTIMES(C_A, C_V));
    assertTrue(FuzzyEquals.ArrayFuzzyEquals(rp, answer.getReal()));
    assertTrue(FuzzyEquals.ArrayFuzzyEquals(ip, answer.getImag()));
  }

  @Test
  public void VectorMtimesMatrixTest() {
    rp = new double[][] { { -1643.4000000000000909, -1861.2000000000000455, -2079.0000000000000000 } };
    ip = new double[][] { { 332.0000000000000000, 376.0000000000000000, 420.0000000000000000 } };
    ComplexArrayContainer answer = Materialisers.toComplexArrayContainer(new MTIMES(C_H, C_A));
    assertTrue(FuzzyEquals.ArrayFuzzyEquals(rp, answer.getReal()));
    assertTrue(FuzzyEquals.ArrayFuzzyEquals(ip, answer.getImag()));
  }

  @Test
  public void MatrixMtimesMatrixTest() {
    rp = new double[][] { { -138.5999999999999943, -316.7999999999999545, -495., -673.2000000000000455 },
      { -316.7999999999999545, -762.2999999999999545, -1207.8000000000001819, -1653.2999999999999545 }, { -495., -1207.8000000000001819, -1920.5999999999999091, -2633.4000000000000909 },
      { -673.2000000000000455, -1653.2999999999999545, -2633.4000000000000909, -3613.5000000000000000 } };
    ip = new double[][] { { 28., 64., 100., 136. }, { 64., 154., 244., 334. }, { 100., 244., 388., 532. }, { 136., 334., 532., 730. } };
    ComplexArrayContainer answer = Materialisers.toComplexArrayContainer(new MTIMES(C_A, C_C));
    assertTrue(FuzzyEquals.ArrayFuzzyEquals(rp, answer.getReal()));
    assertTrue(FuzzyEquals.ArrayFuzzyEquals(ip, answer.getImag()));
  }

  @Test(expectedExceptions = MathsExceptionNativeComputation.class)
  public void Complex_MatrixMtimesMatrixNonConformanceTest() {
    Materialisers.toDoubleArrayOfArrays(new MTIMES(C_A, C_B));
  }

}
