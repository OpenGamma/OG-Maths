/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.datacontainers;

import java.util.Arrays;

import org.testng.annotations.Test;

import com.opengamma.maths.DOGMA2;
import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGComplexDiagonalMatrix;
import com.opengamma.maths.datacontainers.matrix.OGComplexSparseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGLogicalMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealDiagonalMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealSparseMatrix;
import com.opengamma.maths.datacontainers.other.ComplexArrayContainer;
import com.opengamma.maths.datacontainers.other.OGResult;
import com.opengamma.maths.materialisers.Materialisers;

/**
 * Test space for ideas.
 */
@Test
public class TestLongdog {

  /**
   * The materialiser
   */
  Materialisers materialiser = new Materialisers();

  @Test(enabled = false)
  public void test1() {
    OGNumeric A = new OGRealDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } });
    OGNumeric B = new OGComplexSparseMatrix(new double[][] { { 1, 0, 3 }, { 0, 5, 6 }, { 7, 8, 0 }, { 10, 0, 0 } }, new double[][] { { 0, 2, 3 }, { 3, 0, 6 }, { 0, 8, 0 }, { 10, 0, 12 } });
    OGNumeric C;

    C = DOGMA2.copy(A);
    C = DOGMA2.minus(B, C);
    C = DOGMA2.plus(A, C);
    C = DOGMA2.plus(C, C);

    materialiser.toDoubleArrayOfArrays(C);

    System.out.println("Back in java land");
  }

  @Test(enabled = false)
  public void Test2() {
    OGLogicalMatrix baz = new OGLogicalMatrix(new double[] { 1, 0, 1, 0 }, 2, 2);
    System.out.println(baz.getType());
    OGRealDenseMatrix wibble = new OGRealDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } });
    materialiser.toDoubleArrayOfArrays(wibble);
    OGComplexDenseMatrix wobble = new OGComplexDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 } }, new double[][] { { 10, 20, 30 }, { 40, 50, 60 } });
    materialiser.toDoubleArrayOfArrays(wobble);
    OGRealSparseMatrix wabble = new OGRealSparseMatrix(new double[][] { { 1, 0, 3 }, { 0, 5, 6 }, { 7, 8, 0 }, { 10, 0, 0 } });
    materialiser.toDoubleArrayOfArrays(wabble);
    OGComplexSparseMatrix webble = new OGComplexSparseMatrix(new double[][] { { 1, 0, 3 }, { 0, 5, 6 }, { 7, 8, 0 }, { 10, 0, 0 } }, new double[][] { { 0, 2, 3 }, { 3, 0, 6 }, { 0, 8, 0 },
      { 10, 0, 12 } });
    double[][] result = materialiser.toDoubleArrayOfArrays(webble);
    System.out.println(Arrays.toString(result[0]));
    System.out.println("Back in java land");
  }

  @Test(enabled = false)
  public void test3() {
    OGNumeric A = new OGRealDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } });
    OGNumeric B = new OGComplexSparseMatrix(new double[][] { { 1, 0, 3 }, { 0, 5, 6 }, { 7, 8, 0 }, { 10, 0, 0 } }, new double[][] { { 0, 2, 3 }, { 3, 0, 6 }, { 0, 8, 0 }, { 10, 0, 12 } });
    OGNumeric C, D, E;
    OGResult res = DOGMA2.svd(A);
    D = res.get(1);
    C = DOGMA2.plus(res.get(1), res.get(2));
    E = DOGMA2.minus(D, C);
    materialiser.toDoubleArrayOfArrays(E);
  }

  @Test
  public void test4() {
    OGNumeric A = new OGRealDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } });
    double[][] result = Materialisers.toDoubleArrayOfArrays(A);
    System.out.println("Array of dimension [" + result.length + "],[" + result[0].length + "]");
    for (int i = 0; i < result.length; i++) {
      System.out.println(Arrays.toString(result[i]));
    }
    System.out.println("Back in java land");
  }

  @Test
  public void test5() {
    OGNumeric A = new OGComplexDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 } }, new double[][] { { 10, 20, 30 }, { 40, 50, 60 } });
    ComplexArrayContainer result = Materialisers.toComplexArrayContainer(A);
    System.out.println("Real array of dimension [" + result.getReal().length + "],[" + result.getReal()[0].length + "]");
    for (int i = 0; i < result.getReal().length; i++) {
      System.out.println(Arrays.toString(result.getReal()[i]));
    }
    System.out.println("Imag array of dimension [" + result.getImag().length + "],[" + result.getImag()[0].length + "]");
    for (int i = 0; i < result.getImag().length; i++) {
      System.out.println(Arrays.toString(result.getImag()[i]));
    }
    System.out.println("Back in java land");
  }

  @Test
  public void test6() {
    OGNumeric A = new OGRealDiagonalMatrix(new double[] { 1, 2, 3, 4 });
    double[][] result = Materialisers.toDoubleArrayOfArrays(A);
    System.out.println("Array of dimension [" + result.length + "],[" + result[0].length + "]");
    for (int i = 0; i < result.length; i++) {
      System.out.println(Arrays.toString(result[i]));
    }
    System.out.println("Back in java land");
  }

  @Test
  public void test7() {
    OGNumeric A = new OGRealDiagonalMatrix(new double[] { 1, 2, 3, 4 }, 6, 7);
    double[][] result = Materialisers.toDoubleArrayOfArrays(A);
    System.out.println("Array of dimension [" + result.length + "],[" + result[0].length + "]");
    for (int i = 0; i < result.length; i++) {
      System.out.println(Arrays.toString(result[i]));
    }
    System.out.println("Back in java land");
  }

  @Test
  public void test8() {
    OGNumeric A = new OGComplexDiagonalMatrix(new double[] { 1, 2, 3, 4 }, new double[] { 10, 20, 30, 40 }, 6, 7);
    ComplexArrayContainer result = Materialisers.toComplexArrayContainer(A);
    System.out.println("Real array of dimension [" + result.getReal().length + "],[" + result.getReal()[0].length + "]");
    for (int i = 0; i < result.getReal().length; i++) {
      System.out.println(Arrays.toString(result.getReal()[i]));
    }
    System.out.println("Imag array of dimension [" + result.getImag().length + "],[" + result.getImag()[0].length + "]");
    for (int i = 0; i < result.getImag().length; i++) {
      System.out.println(Arrays.toString(result.getImag()[i]));
    }
    System.out.println("Back in java land");
  }

  @Test(enabled = false)
  public void test9() {
    OGNumeric A = new OGComplexDiagonalMatrix(new double[] { 1, 2, 3, 4 }, new double[] { 10, 20, 30, 40 });
    ComplexArrayContainer result = Materialisers.toComplexArrayContainer(A);
    System.out.println("Real array of dimension [" + result.getReal().length + "],[" + result.getReal()[0].length + "]");
    for (int i = 0; i < result.getReal().length; i++) {
      System.out.println(Arrays.toString(result.getReal()[i]));
    }
    System.out.println("Imag array of dimension [" + result.getImag().length + "],[" + result.getImag()[0].length + "]");
    for (int i = 0; i < result.getImag().length; i++) {
      System.out.println(Arrays.toString(result.getImag()[i]));
    }
    System.out.println("Back in java land");
  }

}
