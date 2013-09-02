package com.opengamma.longdog.datacontainers;

import org.testng.annotations.Test;

import com.opengamma.longdog.DOGMA2;
import com.opengamma.longdog.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.longdog.datacontainers.matrix.OGComplexSparseMatrix;
import com.opengamma.longdog.datacontainers.matrix.OGLogicalMatrix;
import com.opengamma.longdog.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.longdog.datacontainers.matrix.OGRealSparseMatrix;
import com.opengamma.longdog.datacontainers.other.OGResult;
import com.opengamma.longdog.materialisers.Materialisers;

@Test
public class TestLongdog {

  Materialisers materialiser = new Materialisers();

  @Test
  public void test1() {
    OGNumeric A = new OGRealDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } });
    OGNumeric B = new OGComplexSparseMatrix(new double[][] { { 1, 0, 3 }, { 0, 5, 6 }, { 7, 8, 0 }, { 10, 0, 0 } }, new double[][] { { 0, 2, 3 }, { 3, 0, 6 }, { 0, 8, 0 }, { 10, 0, 12 } });
    OGNumeric C;

    C = DOGMA2.copy(A);
    C = DOGMA2.minus(B, C);
    C = DOGMA2.plus(A, C);
    C = DOGMA2.plus(C, C);

    materialiser.toJDoubleArray(C);

    System.out.println("Back in java land");
  }

  @Test
  public void Test2() {
    OGLogicalMatrix baz = new OGLogicalMatrix(new double[] { 1, 0, 1, 0 }, 2, 2);
    System.out.println(baz.getType());
    OGRealDenseMatrix wibble = new OGRealDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } });
    materialiser.toJDoubleArray(wibble);
    OGComplexDenseMatrix wobble = new OGComplexDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 } }, new double[][] { { 10, 20, 30 }, { 40, 50, 60 } });
    materialiser.toJDoubleArray(wobble);
    OGRealSparseMatrix wabble = new OGRealSparseMatrix(new double[][] { { 1, 0, 3 }, { 0, 5, 6 }, { 7, 8, 0 }, { 10, 0, 0 } });
    materialiser.toJDoubleArray(wabble);
    OGComplexSparseMatrix webble = new OGComplexSparseMatrix(new double[][] { { 1, 0, 3 }, { 0, 5, 6 }, { 7, 8, 0 }, { 10, 0, 0 } }, new double[][] { { 0, 2, 3 }, { 3, 0, 6 }, { 0, 8, 0 },
      { 10, 0, 12 } });
    materialiser.toJDoubleArray(webble);
    System.out.println("Back in java land");
  }

  @Test
  public void test3() {
    OGNumeric A = new OGRealDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 10, 11, 12 } });
    OGNumeric B = new OGComplexSparseMatrix(new double[][] { { 1, 0, 3 }, { 0, 5, 6 }, { 7, 8, 0 }, { 10, 0, 0 } }, new double[][] { { 0, 2, 3 }, { 3, 0, 6 }, { 0, 8, 0 }, { 10, 0, 12 } });
    OGNumeric C, D, E;
    OGResult res = DOGMA2.svd(A);
    D = res.get(1);
    C = DOGMA2.plus(res.get(1), res.get(2));
    E = DOGMA2.minus(D, C);
    materialiser.toJDoubleArray(E);
  }

}
