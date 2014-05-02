/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.testnodes;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.OGTerminal;
import com.opengamma.maths.datacontainers.lazy.OGExpr;
import com.opengamma.maths.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.maths.datacontainers.other.ComplexArrayContainer;
import com.opengamma.maths.datacontainers.scalar.OGComplexScalar;
import com.opengamma.maths.datacontainers.scalar.OGRealScalar;
import com.opengamma.maths.exceptions.MathsException;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.helpers.FuzzyEquals;
import com.opengamma.maths.materialisers.Materialisers;
import com.opengamma.maths.nodes.LU;
import com.opengamma.maths.nodes.MTIMES;

/**
 * A very basic numerical test of the LU decomposition.
 */
public class TestLUMaterialise {

  //ok cond 5x4
  double[] rcondok5x4 = new double[] { 5, 9, 10, 8, 1, 9, 17, 19, 15, 2, 10, 19, 29, 21, 3, 8, 15, 21, 28, 4 };
  double[] ccondok5x4 = new double[] { 5., -10., 9., -18., 10., -20., 8., -16., 1., -2., 9., -18., 17., -34., 19., -38., 15., -30., 2., -4., 10., -20., 19., -38., 29., -58., 21., -42., 3., -6., 8.,
    -16., 15., -30., 21., -42., 28., -56., 4., -8. };

  //ok cond pivot is [5,4,3,2,1], dim 5x3
  double[] rpivot5_1 = new double[] { 1., 4., 7., -10., -13., 2., -5., 8., 11., -14., 3., 6., -9., -12., -15. };

  //singular 3x3
  double[] rsingular3x3 = new double[] { 1, 10, 1, 2, 20, 2, 3, 30, 3 };
  double[] csingular3x3 = new double[] { 1, 10, 10, 100, 1, 10, 2, 20, 20, 200, 2, 20, 3, 30, 30, 300, 3, 30 };

  //1:10 vector
  double[] r1_10 = new double[] { 1., 2., 3., 4., 5., 6., 7., 8., 9., 10 };
  double[] c1_10 = new double[] { 1., 10, 2., 20, 3., 30, 4., 40, 5., 50, 6., 60, 7., 70, 8., 80, 9., 90, 10, 100 };

  static OGRealDenseMatrix real_matrix = new OGRealDenseMatrix(new double[][] { { 1, 2 }, { 3, 4 }, { 5, 6 } });
  static OGComplexDenseMatrix complex_matrix = new OGComplexDenseMatrix(new double[][] { { 1, 2 }, { 3, 4 }, { 5, 6 } }, new double[][] { { 10, 20 }, { 30, 40 }, { 50, 60 } });

  @DataProvider
  public Object[][] realdataContainer() {
    Object[][] obj = new Object[6][3];

    // real scalar test
    obj[0][0] = new LU(new OGRealScalar(13));
    obj[0][1] = new OGRealScalar(1);
    obj[0][2] = new OGRealScalar(13);

    // real 5x4 matrix test
    obj[1][0] = new LU(new OGRealDenseMatrix(rcondok5x4, 5, 4));
    obj[1][1] = new OGRealDenseMatrix(new double[] { 0.5, 0.9, 1, 0.8, 0.1, 1, 0.2, 0, 0.4, -0.2, 0, 1, 0, 0.0645161290322571, 0.1290322580645162, 0, 0, 0, 1, 0.1480519480519481 }, 5, 4);
    obj[1][2] = new OGRealDenseMatrix(new double[] { 10, 0, 0, 0, 19, -0.5, 0, 0, 29, -4.5, -6.2, 0, 21, -2.5, -3.4, 12.4193548387096779 }, 4, 4);

    // real 2x10 matrix test
    obj[2][0] = new LU(new OGRealDenseMatrix(rcondok5x4, 2, 10));
    obj[2][1] = new OGRealDenseMatrix(new double[] { 0.5555555555555555, 1., 1., 0. }, 2, 2);
    obj[2][2] = new OGRealDenseMatrix(new double[] { 9, 0, 8, 5.5555555555555554, 9, -4, 19, 6.4444444444444446, 2, 13.8888888888888893, 19, -0.5555555555555554, 21, 17.3333333333333321, 8,
      -1.4444444444444446, 21, 3.3333333333333321, 4, 25.7777777777777786 }, 2, 10);

    // real reverse pivot in matrix test
    obj[3][0] = new LU(new OGRealDenseMatrix(rpivot5_1, 5, 3));
    obj[3][1] = new OGRealDenseMatrix(new double[] { -0.0769230769230769, -0.3076923076923077, -0.5384615384615385, 0.7692307692307693, 1., 0.0424028268551237, -0.4275618374558304,
      0.0212014134275618, 1., 0, -0.1093167701863354, -0.0695652173913043, 1., 0, 0 }, 5, 3);
    obj[3][2] = new OGRealDenseMatrix(new double[] { -13., 0., 0., -14., 21.7692307692307701, 0., -15., -0.4615384615384617, -17.0671378091872832 }, 3, 3);

    // real horizontal vector test
    obj[4][0] = new LU(new OGRealDenseMatrix(r1_10, 1, 10));
    obj[4][1] = new OGRealDenseMatrix(1);
    obj[4][2] = new OGRealDenseMatrix(r1_10, 1, 10);

    // real vertical vector test 
    obj[5][0] = new LU(new OGRealDenseMatrix(r1_10, 10, 1));
    obj[5][1] = new OGRealDenseMatrix(new double[] { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 }, 10, 1);
    obj[5][2] = new OGRealScalar(10);

    return obj;

  };

  @DataProvider
  public Object[][] complexdataContainer() {
    Object[][] obj = new Object[5][3];

    // complex scalar test
    obj[0][0] = new LU(new OGComplexScalar(13, 37));
    obj[0][1] = new OGRealScalar(1);
    obj[0][2] = new OGComplexScalar(13, 37);

    // complex 5x4 matrix test
    obj[1][0] = new LU(new OGComplexDenseMatrix(ccondok5x4, 5, 4));
    obj[1][1] = new OGRealDenseMatrix(new double[] { 0.5, 0.9, 1., 0.8, 0.1, 1., 0.2, 0., 0.4, -0.2, 0., 1., 0., 0.0645161290322568, 0.1290322580645157, 0., 0., 0., 1., 0.1480519480519480 }, 5, 4);
    obj[1][2] = new OGComplexDenseMatrix(new double[] { 10., -20., 0., 0., 0., 0., 0., 0., 19., -38., -0.5, 1., 0., 0., 0., 0., 29., -58., -4.5, 9., -6.2000000000000171, 12.4000000000000341, 0., 0.,
      21., -42., -2.5, 5., -3.4000000000000092, 6.8000000000000185, 12.4193548387096779, -24.8387096774193559 }, 4, 4);

    // complex 2x10 matrix test
    obj[2][0] = new LU(new OGComplexDenseMatrix(ccondok5x4, 2, 10));
    obj[2][1] = new OGRealDenseMatrix(new double[] { 0.5555555555555555, 1., 1., 0. }, 2, 2);
    obj[2][2] = new OGComplexDenseMatrix(new double[] { 9., -18., 0., 0., 8., -16., 5.5555555555555554, -11.1111111111111107, 9., -18., -4., 8., 19., -38., 6.4444444444444446, -12.8888888888888893,
      2., -4., 13.8888888888888893, -27.7777777777777786, 19., -38., -0.5555555555555554, 1.1111111111111107, 21., -42., 17.3333333333333321, -34.6666666666666643, 8., -16., -1.4444444444444446,
      2.8888888888888893, 21., -42., 3.3333333333333321, -6.6666666666666643, 4., -8., 25.7777777777777786, -51.5555555555555571 }, 2, 10);

    // complex horizontal vector test
    obj[3][0] = new LU(new OGComplexDenseMatrix(c1_10, 1, 10));
    obj[3][1] = new OGRealDenseMatrix(1);
    obj[3][2] = new OGComplexDenseMatrix(c1_10, 1, 10);

    // complex vertical vector test 
    obj[4][0] = new LU(new OGComplexDenseMatrix(c1_10, 10, 1));
    obj[4][1] = new OGRealDenseMatrix(new double[] { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0 }, 10, 1);
    obj[4][2] = new OGComplexScalar(10, 100);

    return obj;
  };

  @DataProvider
  public Object[][] jointdataContainer() {
    Object[][] obj = new Object[realdataContainer().length + complexdataContainer().length][];
    int ptr = 0;
    for (int i = 0; i < realdataContainer().length; i++) {
      obj[ptr++] = realdataContainer()[i];
    }
    for (int i = 0; i < complexdataContainer().length; i++) {
      obj[ptr++] = complexdataContainer()[i];
    }
    return obj;
  }

  /**
   * Check LU will materialise to a double[][] correctly from a real space result.
   */
  @Test(dataProvider = "realdataContainer")
  public void materialiseToJDoubleArray(LU input, OGTerminal expectedL, OGTerminal expectedU) {
    double[][] L = Materialisers.toDoubleArrayOfArrays(input.getL());
    double[][] U = Materialisers.toDoubleArrayOfArrays(input.getU());

    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toDoubleArrayOfArrays(expectedL), L, 1e-14, 1e-14)) {
      throw new MathsException("L not equal. Got: " + Materialisers.toOGTerminal(new OGRealDenseMatrix(L)).toString() + " Expected: " + expectedL.toString());
    }

    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toDoubleArrayOfArrays(expectedU), U, 1e-14, 1e-14)) {
      throw new MathsException("U not equal. Got: " + Materialisers.toOGTerminal(new OGRealDenseMatrix(U)).toString() + " Expected: " + expectedU.toString());
    }
  }

  /**
   * Check LU will materialise to a {@code ComplexArrayContainer} correctly from a complex space result.
   */
  @Test(dataProvider = "complexdataContainer")
  public void materialiseToComplexArrayContainer(LU input, OGTerminal expectedL, OGTerminal expectedU) {
    ComplexArrayContainer L = Materialisers.toComplexArrayContainer(input.getL());
    ComplexArrayContainer U = Materialisers.toComplexArrayContainer(input.getU());

    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toComplexArrayContainer(expectedL).getReal(), L.getReal(), 1e-14, 1e-14)) {
      throw new MathsException("U not equal. Got: " + L.toString() + " Expected: " + expectedL.toString());
    }

    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toComplexArrayContainer(expectedL).getImag(), L.getImag(), 1e-14, 1e-14)) {
      throw new MathsException("U not equal. Got: " + L.toString() + " Expected: " + expectedL.toString());
    }

    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toComplexArrayContainer(expectedU).getReal(), U.getReal(), 1e-14, 1e-14)) {
      throw new MathsException("U not equal. Got: " + U.toString() + " Expected: " + expectedU.toString());
    }

    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toComplexArrayContainer(expectedU).getImag(), U.getImag(), 1e-14, 1e-14)) {
      throw new MathsException("U not equal. Got: " + U.toString() + " Expected: " + expectedU.toString());
    }
  }

  /**
   * Check LU will materialise to a OGTerminal correctly.
   */
  @Test(dataProvider = "jointdataContainer")
  public void materialiseToOGTerminal(LU input, OGTerminal expectedL, OGTerminal expectedU) {
    OGTerminal L = Materialisers.toOGTerminal(input.getL());
    OGTerminal U = Materialisers.toOGTerminal(input.getU());

    if (!expectedL.mathsequals(L, 1e-14, 1e-14)) {
      throw new MathsException("L not equal. Got: " + L.toString() + " Expected: " + expectedL.toString());
    }

    if (!expectedU.mathsequals(U, 1e-14, 1e-14)) {
      throw new MathsException("U not equal. Got: " + U.toString() + " Expected: " + expectedU.toString());
    }
  }

  /**
   * Check LU will reconstruct correctly. i.e. A = L*U;
   */
  @Test(dataProvider = "jointdataContainer")
  public void reconstructionTest(LU input, OGTerminal expectedL, OGTerminal expectedU) {

    OGNumeric L = input.getL();
    OGNumeric U = input.getU();

    OGTerminal reconstructedMatrix = Materialisers.toOGTerminal(new MTIMES(L, U));
    if (!Materialisers.toOGTerminal(input.getArg(0)).mathsequals(Materialisers.toOGTerminal(reconstructedMatrix))) {
      throw new MathsException("Arrays not equal. Got: " + reconstructedMatrix.toString() + "\n Expected: " + Materialisers.toOGTerminal(input.getArg(0)).toString());
    }
  }

  /**
   * Check LU will throw on access to illegal arg position.
   */
  @Test(dataProvider = "jointdataContainer", expectedExceptions = MathsExceptionIllegalArgument.class)
  public void outsideArgBound(OGExpr input, OGTerminal expectedL, OGTerminal expectedU) {
    input.getArg(1);
  }

}
