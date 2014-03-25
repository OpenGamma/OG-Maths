/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.testnodes;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.longdog.datacontainers.OGTerminal;
import com.opengamma.longdog.datacontainers.lazy.OGExpr;
import com.opengamma.longdog.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.longdog.datacontainers.matrix.OGRealDiagonalMatrix;
import com.opengamma.longdog.datacontainers.scalar.OGRealScalar;
import com.opengamma.longdog.exceptions.MathsException;
import com.opengamma.longdog.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.longdog.helpers.FuzzyEquals;
import com.opengamma.longdog.materialisers.Materialisers;
import com.opengamma.longdog.nodes.MTIMES;
import com.opengamma.longdog.nodes.SVD;

/**
 * A very basic numerical test of the SVD.
 */
public class TestSVDMaterialise {

  static OGRealDenseMatrix matrix = new OGRealDenseMatrix(new double[][] { { 1, 2 }, { 3, 4 }, { 5, 6 } });

  @DataProvider
  public Object[][] dataContainer() {
    Object[][] obj = new Object[1][4];

    obj[0][0] = new SVD(matrix);
    obj[0][1] = new OGRealDenseMatrix(new double[][] { { -0.2298476964000714, 0.8834610176985253, 0.4082482904638627 }, { -0.5247448187602936, 0.2407824921325463, -0.8164965809277263 },
      { -0.8196419411205156, -0.4018960334334317, 0.4082482904638631 } });
    obj[0][2] = new OGRealDiagonalMatrix(new double[] { 9.5255180915651074, 0.5143005806586441 }, 3, 2);
    obj[0][3] = new OGRealDenseMatrix(new double[][] { { -0.6196294838293402, -0.7848944532670524 }, { -0.7848944532670524, 0.6196294838293402 } });

    return obj;
  };

  @Test(dataProvider = "dataContainer")
  public void materialiseToJDoubleArray(SVD input, OGRealDenseMatrix expectedU, OGRealDiagonalMatrix expectedS, OGRealDenseMatrix expectedV) {
    double[][] U = Materialisers.toDoubleArrayOfArrays(input.getU());
    double[][] S = Materialisers.toDoubleArrayOfArrays(input.getS());
    double[][] V = Materialisers.toDoubleArrayOfArrays(input.getVT());

    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toDoubleArrayOfArrays(expectedU), U)) {
      throw new MathsException("U not equal. Got: " + U[0][0] + " Expected: " + expectedU.getData()[0]);
    }

    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toDoubleArrayOfArrays(expectedS), S)) {
      throw new MathsException("S not equal. Got: " + S[0][0] + " Expected: " + expectedS.getData()[0]);
    }

    if (!FuzzyEquals.ArrayFuzzyEquals(Materialisers.toDoubleArrayOfArrays(expectedV), V)) {
      throw new MathsException("V not equal. Got: " + V[0][0] + " Expected: " + expectedV.getData()[0]);
    }
  }

  @Test(dataProvider = "dataContainer")
  public void materialiseToOGTerminal(SVD input, OGRealDenseMatrix expectedU, OGRealDiagonalMatrix expectedS, OGRealDenseMatrix expectedV) {
    OGTerminal U = Materialisers.toOGTerminal(input.getU());
    OGTerminal S = Materialisers.toOGTerminal(input.getS());
    OGTerminal V = Materialisers.toOGTerminal(input.getVT());

    if (!FuzzyEquals.ArrayFuzzyEquals(U.getData(), expectedU.getData())) {
      throw new MathsException("U not equal. Got: " + U.toString() + " Expected: " + expectedU.getData()[0]);
    }

    if (!FuzzyEquals.ArrayFuzzyEquals(S.getData(), expectedS.getData())) {
      throw new MathsException("S not equal. Got: " + S.toString() + " Expected: " + expectedS.getData()[0]);
    }

    if (!FuzzyEquals.ArrayFuzzyEquals(V.getData(), expectedV.getData())) {
      throw new MathsException("Arrays not equal. Got: " + V.toString() + " Expected: " + expectedV.getData()[0]);
    }
  }

  @Test(dataProvider = "dataContainer")
  public void reconstructionTest(SVD input, OGRealDenseMatrix expectedU, OGRealDiagonalMatrix expectedS, OGRealDenseMatrix expectedV) {

    // TODO: FIXME: MAT-353 Need SELECTRESULT to be understood correctly by librdag. 
    OGTerminal U = Materialisers.toOGTerminal(input.getU());
    OGTerminal S = new OGRealDenseMatrix(Materialisers.toDoubleArrayOfArrays(input.getS()));
    OGTerminal VT = Materialisers.toOGTerminal(input.getVT());

   OGTerminal reconstructedMatrix =  Materialisers.toOGTerminal(new MTIMES(new MTIMES(U, S), VT));
    if (!FuzzyEquals.ArrayFuzzyEquals(matrix.getData(), reconstructedMatrix.getData())) {
      throw new MathsException("Arrays not equal. Got: " + matrix.toString() + "\n Expected: " + reconstructedMatrix.toString());
    }
  }

  @Test(dataProvider = "dataContainer", expectedExceptions = MathsExceptionIllegalArgument.class)
  public void outsideArgBound(OGExpr input, OGRealDenseMatrix expectedU, OGRealDiagonalMatrix expectedS, OGRealDenseMatrix expectedV) {
    input.getArg(1);
  }

}
