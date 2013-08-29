/**
 * Copyright (C) 2012 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.longdog.datacontainers.matrix;

import static org.testng.Assert.assertTrue;

import java.util.Arrays;

import org.testng.annotations.Test;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;
import com.opengamma.longdog.helpers.DenseMemoryManipulation;
import com.opengamma.longdog.helpers.MathsExceptionIllegalArgument;
import com.opengamma.longdog.helpers.MathsExceptionNullPointer;

/**
 * Tests the OGRealMatrix Class
 */
public class OGRealMatrixTest {

  double[][] data4x3 = new double[][] { { 1.00, 2.00, 3.00 }, { 4.00, 5.00, 6.00 }, { 7.00, 8.00, 9.00 }, { 10.00, 11.00, 12.00 } };
  double[] data4x3unwound = new double[] { 1.00, 4.00, 7.00, 10.00, 2.00, 5.00, 8.00, 11.00, 3.00, 6.00, 9.00, 12.00 };
  OGRealMatrix getRow = new OGRealMatrix(new double[] { 7, 8, 9 }, 1, 3);
  OGRealMatrix getCol = new OGRealMatrix(new double[] { 2, 5, 8, 11 }, 4, 1);

  // sending in null ptr double[][] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrPtrConstructorNullPtrTest() {
    double[][] tmp = null;
    new OGRealMatrix(tmp);
  }

  // sending in embedded null ptr double[][] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrPtrConstructorOKThenNullPtrTest() {
    double[][] tmp = new double[2][1];
    tmp[0] = new double[] { 1 };
    tmp[1] = null;
    new OGRealMatrix(tmp);
  }

  // sending in null ptr double[] constructor  
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrConstructorNullPtrTest() {
    double[] tmp = null;
    new OGRealMatrix(tmp, 1, 1);
  }

  // sending in ragged[][] double[][] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrPtrConstructorRaggedTest() {
    double[][] tmp = new double[2][];
    tmp[0] = new double[] { 1 };
    tmp[1] = new double[] { 1, 2, 3 };
    new OGRealMatrix(tmp);
  }

  // sending in ok double[][] constructor
  @Test
  public void testDoublePtrPtrConstructorInternalDataTest() {
    OGRealMatrix D = new OGRealMatrix(data4x3);
    assertTrue(D.getClass() == OGRealMatrix.class);
    assertTrue(Arrays.equals(D.getData(), DenseMemoryManipulation.convertRowMajorDoublePointerToColumnMajorSinglePointer(data4x3)));
    assertTrue(D.getRows() == 4);
    assertTrue(D.getCols() == 3);
  }

  //sending in bad rows double[] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorBadRowsDataTest() {
    new OGRealMatrix(data4x3unwound, -1, 3);
  }

  //sending in bad rows double[] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorBadColsDataTest() {
    new OGRealMatrix(data4x3unwound, 3, -1);
  }

  //sending in bad data count double[] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorDataCountDataTest() {
    new OGRealMatrix(data4x3unwound, 3, 17);
  }

  // sending in ok double[] constructor
  @Test
  public void testDoublePtrConstructorInternalDataTest() {
    OGRealMatrix D = new OGRealMatrix(data4x3unwound, 4, 3);
    assertTrue(D.getClass() == OGRealMatrix.class);
    assertTrue(Arrays.equals(D.getData(), DenseMemoryManipulation.convertRowMajorDoublePointerToColumnMajorSinglePointer(data4x3)));
    assertTrue(D.getRows() == 4);
    assertTrue(D.getCols() == 3);
  }

  // sending in single double for a 1x1 element array
  @Test
  public void testDoubleConstructorInternalDataTest() {
    OGRealMatrix D = new OGRealMatrix(7.);
    assertTrue(D.getClass() == OGRealMatrix.class);
    assertTrue(Arrays.equals(D.getData(), new double[] { 7. }));
    assertTrue(D.getRows() == 1);
    assertTrue(D.getCols() == 1);
  }

  // sending in single double for a 1xn element array
  @Test
  public void testDoubleRowPtrConstructorInternalDataTest() {
    double[] data = new double[] { 1, 2, 3, 4 };
    OGRealMatrix D = new OGRealMatrix(data);
    assertTrue(D.getClass() == OGRealMatrix.class);
    assertTrue(Arrays.equals(D.getData(), data));
    assertTrue(D.getRows() == 1);
    assertTrue(D.getCols() == 4);
  }

  // test get rows
  @Test
  public void testGetRowsTest() {
    OGRealMatrix D = new OGRealMatrix(data4x3unwound, 4, 3);
    assertTrue(D.getRows() == 4);
  }

  // test get cols
  @Test
  public void testGetColumnsTest() {
    OGRealMatrix D = new OGRealMatrix(data4x3unwound, 4, 3);
    assertTrue(D.getCols() == 3);
  }

  @Test
  public void testGetTypeEnum() {
    OGRealMatrix D = new OGRealMatrix(data4x3unwound, 4, 3);
    assertTrue(D.getType().equals(ExprTypeEnum.OGRealMatrix));
  }

}
