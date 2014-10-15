/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.maths.testnodes;

import static org.testng.Assert.assertTrue;

import org.testng.annotations.Test;

import com.opengamma.maths.datacontainers.OGTerminal;
import com.opengamma.maths.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.maths.exceptions.MathsExceptionNativeComputation;
import com.opengamma.maths.materialisers.Materialisers;
import com.opengamma.maths.nodes.MLDIVIDE;

/**
 * Test for mldivide()
 */
@Test
public class TestMldivideMaterialise {

  // NOTE: test written under new guidelines.
  // 1 Test for expected exception
  // 2 Test for expected warn
  // 3 Test for real space
  // 4 Test for complex space

  @Test(expectedExceptions = MathsExceptionNativeComputation.class)
  public void checkThrowOnNonCommute() {
    OGRealDenseMatrix A = new OGRealDenseMatrix(new double[] { 1, 2, 3, 4, 5, 6 }, 3, 2);
    OGRealDenseMatrix B = new OGRealDenseMatrix(new double[] { 1, 2, 3, 4, 5, 6 }, 2, 3);
    MLDIVIDE mldivide = new MLDIVIDE(A, B);
    Materialisers.toOGTerminal(mldivide);
  }

  @Test
  public void expectWarnOnSingular() {
    // TODO: assert warn occurs when we have such a mechanism of checking!
    OGRealDenseMatrix A = new OGRealDenseMatrix(new double[] { 1, 1, 1, 2, 2, 2, 3, 3, 3 }, 3, 3);
    OGRealDenseMatrix B = new OGRealDenseMatrix(new double[] { 1, 2, 3, 4, 5, 6 }, 3, 2);
    MLDIVIDE mldivide = new MLDIVIDE(A, B);
    Materialisers.toOGTerminal(mldivide);
  }

  @Test
  public void checkRealSpaceResult() {
    OGRealDenseMatrix A = new OGRealDenseMatrix(new double[] { 1, 1, 1, 2, 2, 2, 3, 3, 3 }, 3, 3);
    OGRealDenseMatrix B = new OGRealDenseMatrix(new double[] { 1, 2, 3, 4, 5, 6 }, 3, 2);
    MLDIVIDE mldivide = new MLDIVIDE(A, B);
    OGTerminal answer = Materialisers.toOGTerminal(mldivide);
    OGTerminal expected = new OGRealDenseMatrix(new double[][] { { 0.1428571428571428, 0.3571428571428570 }, { 0.2857142857142857, 0.7142857142857144 }, { 0.4285714285714285, 1.0714285714285712 } });
    assertTrue(expected.mathsequals(answer));
  }

  @Test
  public void checkComplexSpaceResult() {
    double[][] rp, ip;
    rp = new double[][] { { 3.0000000000000000, 3.0000000000000000, 5.0000000000000000 }, { 2.0000000000000000, 5.0000000000000000, 3.0000000000000000 },
      { 3.0000000000000000, 2.0000000000000000, 7.0000000000000000 } };
    ip = new double[][] { { 0.0000000000000000, 4.0000000000000000, -6.0000000000000000 }, { -1.0000000000000000, 10.0000000000000000, -2.0000000000000000 },
      { 1.0000000000000000, -1.0000000000000000, -4.0000000000000000 } };
    OGComplexDenseMatrix A = new OGComplexDenseMatrix(rp, ip);
    rp = new double[][] { { 1.0000000000000000, 4.0000000000000000 }, { 2.0000000000000000, 5.0000000000000000 }, { 3.0000000000000000, 6.0000000000000000 } };
    ip = new double[][] { { 2.0000000000000000, 8.0000000000000000 }, { 4.0000000000000000, 10.0000000000000000 }, { 6.0000000000000000, 12.0000000000000000 } };
    OGComplexDenseMatrix B = new OGComplexDenseMatrix(rp, ip);
    MLDIVIDE mldivide = new MLDIVIDE(A, B);
    OGTerminal answer = Materialisers.toOGTerminal(mldivide);
    rp = new double[][] { { -0.7926829268292659, 0.3048780487804908 }, { -0.4786585365853658, -0.2774390243902438 }, { 1.9420731707317069, 2.2530487804878043 } };
    ip = new double[][] { { 5.6158536585365875, 7.9939024390243905 }, { 0.4420731707317078, 0.7530487804878054 }, { -0.5213414634146353, -0.7225609756097574 } };
    OGTerminal expected = new OGComplexDenseMatrix(rp, ip);
    assertTrue(expected.mathsequals(answer, 1e-15, 1e-15));
  }

}
