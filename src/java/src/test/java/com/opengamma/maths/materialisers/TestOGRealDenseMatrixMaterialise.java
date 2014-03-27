/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.materialisers;

import java.util.Arrays;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.maths.exceptions.MathsException;
import com.opengamma.maths.helpers.DenseMemoryManipulation;
import com.opengamma.maths.materialisers.Materialisers;
import com.opengamma.maths.testhelpers.ArraysHelpers;

public class TestOGRealDenseMatrixMaterialise {

  @DataProvider
  public Object[][] dataContainer() {
    Object[][] obj = new Object[4][2];
    
    
    obj[0][0] = new double[][] {{1}};
    obj[0][1] = new double[][] {{1}};

    obj[1][0] = new double[][] {{1,2,3,4}};
    obj[1][1] = new double[][] {{1,2,3,4}};

    obj[2][0] = new double[][] {{1},{2},{3},{4}};
    obj[2][1] = new double[][] {{1},{2},{3},{4}};    

    obj[3][0] = new double[][] {{1,2,3},{4,5,6},{7,8,9},{10,11,12}};
    obj[3][1] = new double[][] {{1,2,3},{4,5,6},{7,8,9},{10,11,12}};
    
    return obj;
  };

  @Test(dataProvider = "dataContainer")
  public void materialiseToJDoubleArray(double[][] input, double[][] expected) {
    OGRealDenseMatrix tmp = new OGRealDenseMatrix(input);
    double[][] answer = Materialisers.toDoubleArrayOfArrays(tmp);
    if (!ArraysHelpers.ArraysEquals(expected, answer)) {
      throw new MathsException("Arrays not equal");
    }
  }

  @Test(dataProvider = "dataContainer")
  public void materialiseToRealDenseMatrix(double[][] input, double[][] expected) {
    OGRealDenseMatrix tmp = new OGRealDenseMatrix(input);
    OGRealDenseMatrix m = (OGRealDenseMatrix) Materialisers.toOGTerminal(tmp);
    if (!Arrays.equals(tmp.getData(), m.getData())) {
      throw new MathsException("Arrays not equal");
    }
  }
}
