/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.demo;

import com.opengamma.maths.DOGMA;
import com.opengamma.maths.datacontainers.OGTerminal;
import com.opengamma.maths.datacontainers.matrix.OGArray;
import com.opengamma.maths.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;

/**
 * Demonstrates context aware addition.
 */
public class AdditionDemo implements DemoInterface {

  OGArray A = new OGRealDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } });
  OGArray B = new OGRealDenseMatrix(new double[][] { { 0.1, 0.2, 0.3 }, { 0.4, 0.5, 0.6 }, { 0.7, 0.8, 0.9 } });
  OGArray C = new OGComplexDenseMatrix(DOGMA.toDoubleArrayOfArrays(A), DOGMA.toDoubleArrayOfArrays(B));

  OGTerminal s1 = DOGMA.D(10);
  OGTerminal s2 = DOGMA.D(20);
  OGTerminal s3 = DOGMA.C(3, 7);

  @Override
  public void runDemo() {
    StringBuffer buf = new StringBuffer();
    buf.append("_Example: " + this.getClass().getSimpleName() + "_\n");
    buf.append("Take two scalars matrices, s1 and s2\n");
    buf.append("s1:\n");
    buf.append(s1.toString());
    buf.append("s2:\n");
    buf.append(s2.toString());
    buf.append("\n");
    buf.append("Addition is simple.\n");
    buf.append("(evaluate via: toOGTerminal(plus(s1,s2))).\n");
    buf.append("Result:\n");
    buf.append(DOGMA.toOGTerminal(DOGMA.plus(s1, s2)).toString());
    buf.append("\n");
    buf.append("We can also add up multiple copies in the same call.\n");
    buf.append("(evaluate via: toOGTerminal(plus(s1,s2,s2,s1,s2,s1))).\n");
    buf.append("Result:\n");
    buf.append(DOGMA.toOGTerminal(DOGMA.plus(s1, s2, s2, s1, s2, s1)).toString());
    buf.append("\n");
    buf.append("We can also perform addition in a vectorised sense.\n");
    buf.append("Take matrix A:\n");
    buf.append(A.toString());
    buf.append("and add scalar s1 to each element. ");
    buf.append("(evaluate via: toOGTerminal(plus(s1,A))).\n");
    buf.append("Result:\n");
    buf.append(DOGMA.toOGTerminal(DOGMA.plus(s1, A)).toString());
    buf.append("\nThis also works in reverse order of arguments. ");
    buf.append("(evaluate via: toOGTerminal(plus(A,s1))).\n");
    buf.append("Result:\n");
    buf.append(DOGMA.toOGTerminal(DOGMA.plus(A, s1)).toString());
    buf.append("\nAnd with multiple arguments. ");
    buf.append("(evaluate via: toOGTerminal(plus(s1,A,s1,A))).\n");
    buf.append("Result:\n");
    buf.append(DOGMA.toOGTerminal(DOGMA.plus(s1, A, s1, A)).toString());
    buf.append("\n\n It's also easy to add matrices. ");
    buf.append("Take matrix B:\n");
    buf.append(B.toString());
    buf.append("and add it to A (evaluate via: toOGTerminal(plus(A,B))).\n");
    buf.append("Result:\n");
    buf.append(DOGMA.toOGTerminal(DOGMA.plus(A, B)).toString());
    buf.append("\n\nDOGMA is also domain aware. Let's add a complex scalar, s3, to real matrix, A.\n");
    buf.append("s3:\n");
    buf.append(s3.toString());
    buf.append("\n\nCompute s3+A (evaluate via: toOGTerminal(plus(s3,A))).\n");
    buf.append("Result:\n");
    buf.append(DOGMA.toOGTerminal(DOGMA.plus(s3, A)).toString());
    buf.append("[note the result has changed domain].\n");
    buf.append("\nAlmost all operations work like this, as vector context operations that work cross domain.\n");

    System.out.print(buf.toString());
  }
}
