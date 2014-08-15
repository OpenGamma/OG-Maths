/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.demo;

import com.opengamma.maths.DOGMA;
import com.opengamma.maths.datacontainers.matrix.OGArray;
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;

/**
 * Demonstrates the difference between mtimes() and times().
 */
public class MtimesVsTimesDemo implements DemoInterface {

  OGArray A = new OGRealDenseMatrix(new double[][] { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } });
  OGArray B = new OGRealDenseMatrix(new double[][] { { 0.1, 0.2, 0.3 }, { 0.4, 0.5, 0.6 }, { 0.7, 0.8, 0.9 } });

  @Override
  public void runDemo() {
    StringBuffer buf = new StringBuffer();
    buf.append("_Example: " + this.getClass().getSimpleName() + "_\n");
    buf.append("Take two 3x3 matrices, A and B\n");
    buf.append("A:\n");
    buf.append(A.toString());
    buf.append("B:\n");
    buf.append(B.toString());
    buf.append("\n");
    buf.append("The times() function performs element wise multiplication.\n");
    buf.append("(evaluate via: toOGTerminal(times(A,B))).\n");
    buf.append(DOGMA.toOGTerminal(DOGMA.times(A, B)).toString());
    buf.append("\n");
    buf.append("Whereas the mtimes() function performs matrix context multiplication.\n");
    buf.append("(evaluate via: toOGTerminal(mtimes(A,B))).\n");
    buf.append(DOGMA.toOGTerminal(DOGMA.mtimes(A, B)).toString());
    System.out.print(buf.toString());
  }
}
