/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.maths.datacontainers.matrix;

import static org.testng.Assert.assertTrue;
import static org.testng.AssertJUnit.assertFalse;

import java.util.Arrays;

import org.testng.annotations.Test;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.maths.datacontainers.scalar.OGComplexScalar;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.exceptions.MathsExceptionNullPointer;
import com.opengamma.maths.helpers.DenseMemoryManipulation;
import com.opengamma.maths.helpers.FuzzyEquals;

/**
 * Tests the OGComplexDenseMatrix class 
 */
public class OGComplexDenseMatrixTest {

  double[][] realdata4x3 = new double[][] { { 1.00, 2.00, 3.00 }, { 4.00, 5.00, 6.00 }, { 7.00, 8.00, 9.00 }, { 10.00, 11.00, 12.00 } };
  double[] realdata4x3unwound = new double[] { 1.00, 4.00, 7.00, 10.00, 2.00, 5.00, 8.00, 11.00, 3.00, 6.00, 9.00, 12.00 };

  double[][] imagdata4x3 = new double[][] { { 10.00, 20.00, 30.00 }, { 40.00, 50.00, 60.00 }, { 70.00, 80.00, 90.00 }, { 100.00, 110.00, 120.00 } };
  double[] imagdata4x3unwound = new double[] { 10.00, 40.00, 70.00, 100.00, 20.00, 50.00, 80.00, 110.00, 30.00, 60.00, 90.00, 120.00 };

  double[] interleaved4x3 = new double[] { 1.00, 10.00, 4.00, 40.00, 7.00, 70.00, 10.00, 100.00, 2.00, 20.00, 5.00, 50.00, 8.00, 80.00, 11.00, 110.00, 3.00, 30.00, 6.00, 60.00, 9.00, 90.00, 12.00,
    120.00 };

  OGComplexScalar[][] cmplx4x3 = new OGComplexScalar[][] { { new OGComplexScalar(1, 10), new OGComplexScalar(2, 20), new OGComplexScalar(3, 30) },
    { new OGComplexScalar(4, 40), new OGComplexScalar(5, 50), new OGComplexScalar(6, 60) }, { new OGComplexScalar(7, 70), new OGComplexScalar(8, 80), new OGComplexScalar(9, 90) },
    { new OGComplexScalar(10, 100), new OGComplexScalar(11, 110), new OGComplexScalar(12, 120) } };

  OGComplexDenseMatrix getRow = new OGComplexDenseMatrix(new double[] { 7, 70, 8, 80, 9, 90 }, 1, 3);
  OGComplexDenseMatrix getCol = new OGComplexDenseMatrix(new double[] { 2, 20, 5, 50, 8, 80, 11, 110 }, 4, 1);

  // for equals tests
  OGComplexDenseMatrix defaultVal = new OGComplexDenseMatrix(interleaved4x3,4,3);
  OGComplexDenseMatrix same = new OGComplexDenseMatrix(interleaved4x3,4,3);
  OGComplexDenseMatrix withindiffnumber = new OGComplexDenseMatrix(new double[] { 1.00+ FuzzyEquals.getDefaultTolerance()/2, 10.00, 4.00, 40.00, 7.00, 70.00, 10.00, 100.00, 2.00, 20.00, 5.00, 50.00, 8.00, 80.00, 11.00, 110.00, 3.00, 30.00, 6.00, 60.00, 9.00, 90.00, 12.00,
    120.00 },4,3);
  OGRealDenseMatrix diffclass =  new OGRealDenseMatrix(1);
  OGComplexDenseMatrix diffrows = new OGComplexDenseMatrix(interleaved4x3,3,4);
  OGComplexDenseMatrix diffcols = new OGComplexDenseMatrix(new double[8],4,2);  
  OGComplexDenseMatrix diffnumber = new OGComplexDenseMatrix(new double[] { 1337, 10.00, 4.00, 40.00, 7.00, 70.00, 10.00, 100.00, 2.00, 20.00, 5.00, 50.00, 8.00, 80.00, 11.00, 110.00, 3.00, 30.00, 6.00, 60.00, 9.00, 90.00, 12.00,
    120.00 },4,3);
  OGComplexDenseMatrix diffdomain_cmplx = new OGComplexDenseMatrix(realdata4x3);
  OGRealDenseMatrix diffdomain_real = new OGRealDenseMatrix(realdata4x3);

  // sending in null ptr double[][] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrPtrConstructorNullPtrTest() {
    double[][] tmp = null;
    new OGComplexDenseMatrix(tmp);
  }

  // sending in embedded null ptr double[][] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrPtrConstructorOKThenNullPtrTest() {
    double[][] tmp = new double[2][1];
    tmp[0] = new double[] { 1 };
    tmp[1] = null;
    new OGComplexDenseMatrix(tmp);
  }

  // sending in null ptr double[][] double[][] constructor 
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrPtrDoublePtrPtrConstructorNullPtrFirstTest() {
    double[][] tmp1 = null;
    double[][] tmp2 = new double[2][3];
    new OGComplexDenseMatrix(tmp1, tmp2);
  }

  // sending in null ptr double[][] double[][] constructor 
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrPtrDoublePtrPtrConstructorNullPtrSecondTest() {
    double[][] tmp1 = new double[2][3];
    double[][] tmp2 = null;
    new OGComplexDenseMatrix(tmp1, tmp2);
  }

  // sending in embedded null ptr double[][] double[][] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrPtrConstructorOKThenNullPtrInFirstTest() {
    double[][] tmp1 = new double[2][];
    tmp1[0] = new double[] { 1 };
    tmp1[1] = null;
    double[][] tmp2 = new double[2][1];
    new OGComplexDenseMatrix(tmp1, tmp2);
  }

  // sending in embedded null ptr double[][] double[][] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrPtrConstructorOKThenNullPtrInSecondTest() {
    double[][] tmp2 = new double[2][];
    double[][] tmp1 = new double[2][];
    tmp1[0] = new double[] { 1 };
    tmp1[1] = null;
    new OGComplexDenseMatrix(tmp1, tmp2);
  }

  // sending in null ptr double[] constructor  
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrConstructorNullPtrTest() {
    double[] tmp = null;
    new OGComplexDenseMatrix(tmp, 1, 1);
  }

  // sending in null ptr double[] double [] int int constructor  
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrDoublePtrConstructorNullPtrFirstTest() {
    double[] tmp1 = null;
    double[] tmp2 = new double[1];
    new OGComplexDenseMatrix(tmp1, tmp2, 1, 1);
  }

  // sending in null ptr double[] double [] int int constructor  
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrDoublePtrConstructorNullPtrSecondTest() {
    double[] tmp1 = new double[1];
    double[] tmp2 = null;
    new OGComplexDenseMatrix(tmp1, tmp2, 1, 1);
  }

  // sending in null ptr OGComplexScalar[][] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testOGComplexScalarPtrPtrConstructorNullPtrTest() {
    OGComplexScalar[][] tmp = null;
    new OGComplexDenseMatrix(tmp);
  }

  // sending in embedded null ptr OGComplexScalar[][] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testOGComplexScalarPtrPtrConstructorOKThenNullPtrTest() {
    OGComplexScalar[][] tmp = new OGComplexScalar[2][1];
    tmp[0] = new OGComplexScalar[] { new OGComplexScalar(1) };
    tmp[1] = null;
    new OGComplexDenseMatrix(tmp);
  }

  // sending in null ptr OGComplexScalar constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testOGComplexScalarConstructorNullPtrTest() {
    OGComplexScalar tmp = new OGComplexScalar(0);
    tmp = null;
    new OGComplexDenseMatrix((OGComplexScalar) tmp);
  }

  // sending in ragged[][] double[][] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrPtrConstructorRaggedTest() {
    double[][] tmp = new double[2][];
    tmp[0] = new double[] { 1 };
    tmp[1] = new double[] { 1, 2, 3 };
    new OGComplexDenseMatrix(tmp);
  }

  // sending in ragged[][] first double[][] double[][] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrPtrDoublePtrPtrConstructorRaggedFirstTest() {
    double[][] tmp1 = new double[2][];
    double[][] tmp2 = new double[2][2];
    tmp1[0] = new double[] { 1 };
    tmp1[1] = new double[] { 1, 2, 3 };
    new OGComplexDenseMatrix(tmp1, tmp2);
  }

  // sending in ragged[][] second double[][] double[][] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrPtrDoublePtrPtrConstructorRaggedSecondTest() {
    double[][] tmp1 = new double[2][2];
    double[][] tmp2 = new double[2][];
    tmp2[0] = new double[] { 1 };
    tmp2[1] = new double[] { 1, 2, 3 };
    new OGComplexDenseMatrix(tmp1, tmp2);
  }

  // sending in ragged OGComplexScalar[][] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testOGComplexScalarPtrPtrConstructorRaggedTest() {
    OGComplexScalar[][] tmp = new OGComplexScalar[2][1];
    tmp[0] = new OGComplexScalar[] { new OGComplexScalar(1), new OGComplexScalar(2) };
    tmp[1] = new OGComplexScalar[] { new OGComplexScalar(1) };
    new OGComplexDenseMatrix(tmp);
  }

  // sending in ok double[][] constructor
  @Test
  public void testDoublePtrPtrConstructorInternalDataTest() {
    OGComplexDenseMatrix D = new OGComplexDenseMatrix(realdata4x3);
    assertTrue(D.getClass() == OGComplexDenseMatrix.class);
    assertTrue(Arrays.equals(D.getData(), DenseMemoryManipulation.convertRowMajorDoublePointerToColumnMajorZeroInterleavedSinglePointer(realdata4x3)));
    assertTrue(D.getRows() == 4);
    assertTrue(D.getCols() == 3);
  }

  // sending in ok double[][] double[][] constructor
  @Test
  public void testDoublePtrPtrDoublePtrPtrConstructorInternalDataTest() {
    OGComplexDenseMatrix D = new OGComplexDenseMatrix(realdata4x3, imagdata4x3);
    assertTrue(D.getClass() == OGComplexDenseMatrix.class);
    assertTrue(Arrays.equals(D.getData(), DenseMemoryManipulation.convertTwoRowMajorDoublePointerToColumnMajorInterleavedSinglePointer(realdata4x3, imagdata4x3)));
    assertTrue(D.getRows() == 4);
    assertTrue(D.getCols() == 3);
  }

  //sending in bad rows double[] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorBadRowsDataTest() {
    new OGComplexDenseMatrix(realdata4x3unwound, -1, 3);
  }

  //sending in bad rows double[] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorBadColsDataTest() {
    new OGComplexDenseMatrix(realdata4x3unwound, 3, -1);
  }

  //sending in bad data count double[] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrConstructorDataCountDataTest() {
    new OGComplexDenseMatrix(realdata4x3unwound, 3, 17);
  }

  //sending in ok REAL double[] constructor
  @Test
  public void testDoublePtrConstructorRealDataOKTest() {
    OGComplexDenseMatrix D = new OGComplexDenseMatrix(realdata4x3unwound, 4, 3);
    assertTrue(D.getClass() == OGComplexDenseMatrix.class);
    assertTrue(Arrays.equals(D.getData(), DenseMemoryManipulation.convertSinglePointerToZeroInterleavedSinglePointer(realdata4x3unwound)));
    assertTrue(D.getRows() == 4);
    assertTrue(D.getCols() == 3);
  }

  //sending in ok REAL double[] constructor, interleaved data branch
  @Test
  public void testDoublePtrConstructorInterleavedDataOKTest() {
    OGComplexDenseMatrix D = new OGComplexDenseMatrix(interleaved4x3, 4, 3);
    assertTrue(D.getClass() == OGComplexDenseMatrix.class);
    assertTrue(Arrays.equals(D.getData(), DenseMemoryManipulation.convertTwoSinglePointersToInterleavedSinglePointer(realdata4x3unwound, imagdata4x3unwound)));
    assertTrue(D.getRows() == 4);
    assertTrue(D.getCols() == 3);
  }

  //sending in bad rows double[] double[] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrDoublePtrConstructorBadRowsDataTest() {
    new OGComplexDenseMatrix(realdata4x3unwound, imagdata4x3unwound, -1, 3);
  }

  //sending in bad rows double[] double[] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrDoublePtrConstructorBadColsDataTest() {
    new OGComplexDenseMatrix(realdata4x3unwound, imagdata4x3unwound, 3, -1);
  }

  //sending in bad data count double[] double[] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrDoublePtrConstructorDataCountDataTest() {
    new OGComplexDenseMatrix(realdata4x3unwound, imagdata4x3unwound, 3, 17);
  }

  //sending in bad data lengths double[] double[] constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrDoublePtrConstructorBadDataLengthTest() {
    new OGComplexDenseMatrix(realdata4x3unwound, new double[4], 3, 17);
  }

  //sending in ok double[] double[] constructor
  @Test
  public void testDoublePtrDoublePtrConstructorRealDataOKTest() {
    OGComplexDenseMatrix D = new OGComplexDenseMatrix(realdata4x3unwound, imagdata4x3unwound, 4, 3);
    assertTrue(D.getClass() == OGComplexDenseMatrix.class);
    assertTrue(Arrays.equals(D.getData(), DenseMemoryManipulation.convertTwoSinglePointersToInterleavedSinglePointer(realdata4x3unwound, imagdata4x3unwound)));
    assertTrue(D.getRows() == 4);
    assertTrue(D.getCols() == 3);
  }

  @Test
  // sending in single double for a 1x1 element array
  public void testDoubleConstructorInternalDataTest() {
    OGComplexDenseMatrix D = new OGComplexDenseMatrix((double) 7.);
    assertTrue(D.getClass() == OGComplexDenseMatrix.class);
    assertTrue(Arrays.equals(D.getData(), new double[] { 7., 0 }));
    assertTrue(D.getRows() == 1);
    assertTrue(D.getCols() == 1);
  }

  // sending in null ptr OGComplexScalar constructor
  @Test
  public void testOGComplexScalarConstructorInternalDataTest() {
    OGComplexScalar tmp = new OGComplexScalar(7.0);
    OGComplexDenseMatrix D = new OGComplexDenseMatrix(tmp);
    assertTrue(D.getClass() == OGComplexDenseMatrix.class);
    assertTrue(Arrays.equals(D.getData(), new double[] { 7., 0 }));
    assertTrue(D.getRows() == 1);
    assertTrue(D.getCols() == 1);

  }

  //sending in ok ComlpexType[][] constructor, interleaved data branch
  @Test
  public void testOGComplexScalarPtrPtrConstructorDataOKTest() {
    OGComplexDenseMatrix D = new OGComplexDenseMatrix(cmplx4x3);
    assertTrue(D.getClass() == OGComplexDenseMatrix.class);
    assertTrue(Arrays.equals(D.getData(), DenseMemoryManipulation.convertTwoSinglePointersToInterleavedSinglePointer(realdata4x3unwound, imagdata4x3unwound)));
    assertTrue(D.getRows() == 4);
    assertTrue(D.getCols() == 3);
  }

  // test get rows
  @Test
  public void testGetRowsTest() {
    OGComplexDenseMatrix D = new OGComplexDenseMatrix(interleaved4x3, 4, 3);
    assertTrue(D.getRows() == 4);
  }

  // test get cols
  @Test
  public void testGetColumnsTest() {
    OGComplexDenseMatrix D = new OGComplexDenseMatrix(interleaved4x3, 4, 3);
    assertTrue(D.getCols() == 3);
  }

  @Test
  public void testGetTypeEnum() {
    OGComplexDenseMatrix D = new OGComplexDenseMatrix(interleaved4x3, 4, 3);
    assertTrue(D.getType().equals(ExprEnum.OGComplexMatrix));
  }
  
  @Test
  public void toStringTest() {
    OGComplexDenseMatrix D = new OGComplexDenseMatrix(interleaved4x3, 4, 3);
    D.toString();
  }

  
  @Test
  public void testHashCode() {
    assertTrue(defaultVal.hashCode() == same.hashCode());
    assertFalse(defaultVal.hashCode() == diffnumber.hashCode());
  }

  @Test
  public void testEquals() {
    assertTrue(defaultVal.equals(defaultVal));
    assertTrue(defaultVal.equals(same));
    assertFalse(defaultVal.equals(diffrows));
    assertFalse(defaultVal.equals(diffcols));
    assertFalse(defaultVal.equals(diffclass));
    assertFalse(defaultVal.equals(diffnumber));
  }

  @Test
  public void testFuzzyEquals() {
    assertTrue(defaultVal.fuzzyequals(defaultVal));
    assertTrue(defaultVal.fuzzyequals(same));
    assertTrue(defaultVal.fuzzyequals(withindiffnumber));
    assertFalse(defaultVal.fuzzyequals(diffrows));
    assertFalse(defaultVal.fuzzyequals(diffcols));
    assertFalse(defaultVal.fuzzyequals(diffclass));
    assertFalse(defaultVal.fuzzyequals(diffnumber));
    assertFalse(diffdomain_cmplx.fuzzyequals(diffdomain_real));
  }

  @Test
  public void testMathsEquals() {
    assertTrue(defaultVal.mathsequals(defaultVal));
    assertTrue(defaultVal.mathsequals(same));
    assertTrue(defaultVal.mathsequals(withindiffnumber));
    assertFalse(defaultVal.mathsequals(diffclass));
    assertFalse(defaultVal.mathsequals(diffrows));
    assertFalse(defaultVal.mathsequals(diffcols));
    assertFalse(defaultVal.mathsequals(diffnumber));
    assertTrue(diffdomain_cmplx.mathsequals(diffdomain_real));
  }
}
