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
import com.opengamma.maths.datacontainers.matrix.OGRealDiagonalMatrix;
import com.opengamma.maths.datacontainers.other.ComplexArrayContainer;
import com.opengamma.maths.exceptions.MathsException;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.helpers.FuzzyEquals;
import com.opengamma.maths.materialisers.Materialisers;
import com.opengamma.maths.nodes.MTIMES;
import com.opengamma.maths.nodes.SVD;

/**
 * A very basic numerical test of the SVD.
 */
public class TestSVDMaterialise {

  static OGRealDenseMatrix real_matrix = new OGRealDenseMatrix(new double[][] { { 1, 2 }, { 3, 4 }, { 5, 6 } });
  static OGComplexDenseMatrix complex_matrix = new OGComplexDenseMatrix(new double[][] { { 1, 2 }, { 3, 4 }, { 5, 6 } }, new double[][] { { 10, 20 }, { 30, 40 }, { 50, 60 } });

  static OGRealDenseMatrix real_hvector = new OGRealDenseMatrix(new double[] { 1, 2, 3 }, 1, 3);

  @DataProvider
  public Object[][] realdataContainer() {
    Object[][] obj = new Object[2][4];

    obj[0][0] = new SVD(real_matrix);
    obj[0][1] = new OGRealDenseMatrix(new double[][] { { -0.2298476964000714, 0.8834610176985253, 0.4082482904638627 }, { -0.5247448187602936, 0.2407824921325463, -0.8164965809277263 },
      { -0.8196419411205156, -0.4018960334334317, 0.4082482904638631 } });
    obj[0][2] = new OGRealDiagonalMatrix(new double[] { 9.5255180915651074, 0.5143005806586441 }, 3, 2);
    obj[0][3] = new OGRealDenseMatrix(new double[][] { { -0.6196294838293402, -0.7848944532670524 }, { -0.7848944532670524, 0.6196294838293402 } });

    obj[1][0] = new SVD(real_hvector); // [MAT-404] picked up by the fuzzer, test to demonstrate fix.
    obj[1][1] = new OGRealDenseMatrix(1);
    obj[1][2] = new OGRealDiagonalMatrix(new double[] { 3.74165738677394, 0, 0 }, 1, 3);
    obj[1][3] = new OGRealDenseMatrix(new double[][] { { 0.2672612419124243, 0.5345224838248488, 0.8017837257372732 }, { -0.5345224838248488, 0.7745419205884383, -0.3381871191173427 },
      { -0.8017837257372732, -0.3381871191173426, 0.4927193213239860 } });

    return obj;
  };

  @DataProvider
  public Object[][] complexdataContainer() {
    Object[][] obj = new Object[1][4];

    obj[0][0] = new SVD(complex_matrix);
    double[][] rp, ip;
    rp = new double[][] { { -0.0228707006002169, -0.0879076568710847, 0.3147401422050641 }, { -0.0522140610036492, -0.0239587534423321, -0.6294802844101258 },
      { -0.0815574214070819, 0.0399901499864153, 0.3147401422050625 } };
    ip = new double[][] { { -0.2287070060021659, -0.8790765687107978, 0.2600102104752862 }, { -0.5221406100364927, -0.2395875344233279, -0.5200204209505755 },
      { -0.8155742140708198, 0.3999014998641418, 0.2600102104752883 } };
    obj[0][1] = new OGComplexDenseMatrix(rp, ip);
    obj[0][2] = new OGRealDiagonalMatrix(new double[] { 95.7302720469661779, 5.1686568674896343 }, 3, 2);
    double[] d = new double[] { -0.6196294838293402, 0, 0.7848944532670524, -0.e0, -0.7848944532670524, 0, -0.6196294838293402, 0 };
    obj[0][3] = new OGComplexDenseMatrix(d, 2, 2);

    return obj;
  };

  @DataProvider
  public Object[][] jointdataContainer() {
    Object[][] obj = new Object[2][];
    obj[0] = realdataContainer()[0];
    obj[1] = complexdataContainer()[0];
    return obj;

  }

  /**
   * Check SVD will materialise to a double[][] correctly from a real space result.
   */
  @Test(dataProvider = "realdataContainer")
  public void materialiseToJDoubleArray(SVD input, OGRealDenseMatrix expectedU, OGRealDiagonalMatrix expectedS, OGRealDenseMatrix expectedVT) {
    double[][] U = Materialisers.toDoubleArrayOfArrays(input.getU());
    double[][] S = Materialisers.toDoubleArrayOfArrays(input.getS());
    double[][] VT = Materialisers.toDoubleArrayOfArrays(input.getVT());

    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toDoubleArrayOfArrays(expectedU), U)) {
      throw new MathsException("U not equal. Got: " + U[0][0] + " Expected: " + expectedU.getData()[0]);
    }

    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toDoubleArrayOfArrays(expectedS), S)) {
      throw new MathsException("S not equal. Got: " + S[0][0] + " Expected: " + expectedS.getData()[0]);
    }

    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toDoubleArrayOfArrays(expectedVT), VT)) {
      throw new MathsException("VT not equal. Got: " + VT[0][0] + " Expected: " + expectedVT.getData()[0]);
    }
  }

  /**
   * Check SVD will materialise to a {@code ComplexArrayContainer} correctly from a complex space result.
   */
  @Test(dataProvider = "complexdataContainer")
  public void materialiseToComplexArrayContainer(SVD input, OGComplexDenseMatrix expectedU, OGRealDiagonalMatrix expectedS, OGComplexDenseMatrix expectedVT) {
    ComplexArrayContainer U = Materialisers.toComplexArrayContainer(input.getU());
    double[][] S = Materialisers.toDoubleArrayOfArrays(input.getS());
    ComplexArrayContainer VT = Materialisers.toComplexArrayContainer(input.getVT());

    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toComplexArrayContainer(expectedU).getReal(), U.getReal())) {
      throw new MathsException("U not equal. Got: " + U.toString() + " Expected: " + expectedU.toString());
    }

    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toComplexArrayContainer(expectedU).getImag(), U.getImag())) {
      throw new MathsException("U not equal. Got: " + U.toString() + " Expected: " + expectedU.toString());
    }

    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toDoubleArrayOfArrays(expectedS), S)) {
      throw new MathsException("S not equal. Got: " + S[0][0] + " Expected: " + expectedS.getData()[0]);
    }

    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toComplexArrayContainer(expectedVT).getReal(), VT.getReal())) {
      throw new MathsException("VT not equal. Got: " + VT.toString() + " Expected: " + expectedVT.toString());
    }

    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toComplexArrayContainer(expectedVT).getImag(), VT.getImag())) {
      throw new MathsException("VT not equal. Got: " + VT.toString() + " Expected: " + expectedVT.toString());
    }
  }

  /**
   * Check SVD will materialise to a OGTerminal correctly.
   */
  @Test(dataProvider = "jointdataContainer")
  public void materialiseToOGTerminal(SVD input, OGTerminal expectedU, OGRealDiagonalMatrix expectedS, OGTerminal expectedVT) {
    OGTerminal U = Materialisers.toOGTerminal(input.getU());
    OGTerminal S = Materialisers.toOGTerminal(input.getS());
    OGTerminal VT = Materialisers.toOGTerminal(input.getVT());

    if (!FuzzyEquals.ArrayFuzzyEquals(U.getData(), expectedU.getData())) {
      throw new MathsException("U not equal. Got: " + U.toString() + " Expected: " + expectedU.getData()[0]);
    }

    if (!FuzzyEquals.ArrayFuzzyEquals(S.getData(), expectedS.getData())) {
      throw new MathsException("S not equal. Got: " + S.toString() + " Expected: " + expectedS.getData()[0]);
    }

    if (!FuzzyEquals.ArrayFuzzyEquals(VT.getData(), expectedVT.getData())) {
      throw new MathsException("VT not equal. Got: " + VT.toString() + " Expected: " + expectedVT.getData()[0]);
    }
  }

  /**
   * Check SVD will reconstruct correctly. i.e. A = U*S*V**T;
   */
  @Test(dataProvider = "jointdataContainer")
  public void reconstructionTest(SVD input, OGTerminal expectedU, OGRealDiagonalMatrix expectedS, OGTerminal expectedV) {

    OGNumeric U = input.getU();
    OGNumeric S = input.getS();
    OGNumeric VT = input.getVT();

    OGTerminal reconstructedMatrix = Materialisers.toOGTerminal(new MTIMES(new MTIMES(U, S), VT));
    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toOGTerminal(input.getArg(0)).getData(), reconstructedMatrix.getData(), 1e-15, 4e-14)) { // numerical fuzz on recontruction
      throw new MathsException("Arrays not equal. Got: " + reconstructedMatrix.toString() + "\n Expected: " + Materialisers.toOGTerminal(input.getArg(0)).toString());
    }
  }

  /**
   * Check SVD will throw on access to illegal arg position.
   */
  @Test(dataProvider = "jointdataContainer", expectedExceptions = MathsExceptionIllegalArgument.class)
  public void outsideArgBound(OGExpr input, OGTerminal expectedU, OGRealDiagonalMatrix expectedS, OGTerminal expectedVT) {
    input.getArg(1);
  }

}
