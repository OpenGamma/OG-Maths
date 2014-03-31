package com.opengamma.maths.datacontainers.matrix;

import static org.testng.Assert.assertTrue;
import static org.testng.AssertJUnit.assertFalse;

import java.util.Arrays;

import org.testng.annotations.Test;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.datacontainers.matrix.OGRealDiagonalMatrix;
import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;
import com.opengamma.maths.exceptions.MathsExceptionNullPointer;
import com.opengamma.maths.helpers.FuzzyEquals;

public class OGRealDiagonalMatrixTest {

  double[] data4x3diagd = new double[] { 1, 2, 3 };
  double[][] data4x3full = new double[][] { { 1.00, 0.00, 0.00 }, { 0.00, 2.00, 0.00 }, { 0.00, 0.00, 3.00 }, { 0.00, 0.00, 0.00 } };

  OGRealDiagonalMatrix defaultVal = new OGRealDiagonalMatrix(data4x3diagd, 4, 3);
  OGRealDiagonalMatrix same = new OGRealDiagonalMatrix(data4x3diagd, 4, 3);
  OGRealDiagonalMatrix withindiffnumber = new OGRealDiagonalMatrix(new double[] { 1 + FuzzyEquals.getDefaultTolerance() / 2, 2, 3 }, 4, 3);
  OGRealDenseMatrix diffclass = new OGRealDenseMatrix(1);
  OGRealDiagonalMatrix diffrows = new OGRealDiagonalMatrix(data4x3diagd, 5, 3);
  OGRealDiagonalMatrix diffcols = new OGRealDiagonalMatrix(data4x3diagd, 4, 4);
  OGRealDiagonalMatrix diffnumber = new OGRealDiagonalMatrix(new double[] { 1337, 2, 3 }, 4, 3);
  OGComplexDiagonalMatrix diffdomain = new OGComplexDiagonalMatrix(new double[] { 1, 2, 3 }, 4, 3);

  // sending in null ptr double[] constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrConstructorNullPtrTest() {
    double[] tmp = null;
    new OGRealDiagonalMatrix(tmp);
  }

  // sending in null ptr double[], int, int constructor
  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void testDoublePtrIntIntConstructorNullPtrTest() {
    double[] tmp = null;
    new OGRealDiagonalMatrix(tmp, 1, 1);
  }

  // sending in ok double[] constructor
  @Test
  public void testDoublePtrConstructorInternalDataTest() {
    OGRealDiagonalMatrix D = new OGRealDiagonalMatrix(data4x3diagd);
    assertTrue(D.getClass() == OGRealDiagonalMatrix.class);
    assertTrue(Arrays.equals(D.getData(), data4x3diagd));
    assertTrue(D.getRows() == 3);
    assertTrue(D.getCols() == 3);
  }

  // sending in bad rows double[], int, int constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrIntIntConstructorBadRowsDataTest() {
    new OGRealDiagonalMatrix(data4x3diagd, -1, 3);
  }

  // sending in bad rows double[], int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoublePtrIntIntConstructorBadColsDataTest() {
    new OGRealDiagonalMatrix(data4x3diagd, 3, -1);
  }

  // sending in ok double[], int, int constructor
  @Test
  public void testDoublePtrIntIntConstructorInternalDataTest() {
    OGRealDiagonalMatrix D = new OGRealDiagonalMatrix(data4x3diagd, 6, 7);
    assertTrue(D.getClass() == OGRealDiagonalMatrix.class);
    assertTrue(Arrays.equals(D.getData(), new double[] { 1, 2, 3, 0, 0, 0 }));
    assertTrue(D.getRows() == 6);
    assertTrue(D.getCols() == 7);
  }

  // sending in bad rows double, int, int constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoubleIntIntConstructorBadRowsDataTest() {
    new OGRealDiagonalMatrix(1, -1, 3);
  }

  // sending in bad rows double, int, int  constructor
  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void testDoubleIntIntConstructorBadColsDataTest() {
    new OGRealDiagonalMatrix(1, 3, -1);
  }

  // sending in ok double, int, int  constructor
  @Test
  public void testDoubleIntIntConstructorInternalDataTest() {
    OGRealDiagonalMatrix D = new OGRealDiagonalMatrix(3, 13, 37);
    assertTrue(D.getClass() == OGRealDiagonalMatrix.class);
    assertTrue(Arrays.equals(D.getData(), new double[] { 3 }));
    assertTrue(D.getRows() == 13);
    assertTrue(D.getCols() == 37);
  }

  // sending in ok double constructor
  @Test
  public void testDoubleConstructorInternalDataTest() {
    OGRealDiagonalMatrix D = new OGRealDiagonalMatrix(3.d);
    assertTrue(D.getClass() == OGRealDiagonalMatrix.class);
    assertTrue(Arrays.equals(D.getData(), new double[] { 3 }));
    assertTrue(D.getRows() == 1);
    assertTrue(D.getCols() == 1);
  }

  // sending in ok int constructor
  @Test
  public void testIntConstructorInternalDataTest() {
    OGRealDiagonalMatrix D = new OGRealDiagonalMatrix((int) 3);
    assertTrue(D.getClass() == OGRealDiagonalMatrix.class);
    assertTrue(Arrays.equals(D.getData(), new double[] { 3 }));
    assertTrue(D.getRows() == 1);
    assertTrue(D.getCols() == 1);
  }

  // test get rows
  @Test
  public void testGetRowsTest() {
    OGRealDiagonalMatrix D = new OGRealDiagonalMatrix(data4x3diagd, 4, 3);
    assertTrue(D.getRows() == 4);
  }

  // test get cols
  @Test
  public void testGetNumColumnsTest() {
    OGRealDiagonalMatrix D = new OGRealDiagonalMatrix(data4x3diagd, 4, 3);
    assertTrue(D.getCols() == 3);
  }

  @Test
  public void testGetTypeEnum() {
    OGRealDiagonalMatrix D = new OGRealDiagonalMatrix(data4x3diagd, 4, 3);
    assertTrue(D.getType().equals(ExprEnum.OGRealDiagonalMatrix));
  }

  @Test
  public void toStringTest() {
    OGRealDiagonalMatrix D = new OGRealDiagonalMatrix(data4x3diagd, 4, 3);
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
    assertFalse(defaultVal.fuzzyequals(diffdomain));
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
    assertTrue(defaultVal.mathsequals(diffdomain));
  }

}
