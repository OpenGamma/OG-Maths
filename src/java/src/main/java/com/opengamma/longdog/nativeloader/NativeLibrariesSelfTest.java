/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.nativeloader;

import com.opengamma.longdog.datacontainers.scalar.OGRealScalar;
import com.opengamma.longdog.materialisers.Materialisers;

public class NativeLibrariesSelfTest {

  public static void main(String[] args) {
    OGRealScalar tmp = new OGRealScalar(1.0);
    double[][] answer = Materialisers.toDoubleArrayOfArrays(tmp);
    System.out.println("Native library linkage appears to be working correctly.");
  }

}
