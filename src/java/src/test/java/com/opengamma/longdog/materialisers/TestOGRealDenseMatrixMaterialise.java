/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.materialisers;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.longdog.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.longdog.exceptions.MathsException;
import com.opengamma.longdog.testhelpers.ArraysHelpers;

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

}
