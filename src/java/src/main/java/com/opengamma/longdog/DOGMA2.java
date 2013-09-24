/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog;

import com.opengamma.longdog.datacontainers.OGNumeric;
import com.opengamma.longdog.datacontainers.other.OGResult;
import com.opengamma.longdog.materialisers.Materialisers;
import com.opengamma.longdog.nativeloader.NativeLibraries;
import com.opengamma.longdog.nodes.COPY;
import com.opengamma.longdog.nodes.NEGATE;
import com.opengamma.longdog.nodes.PLUS;
import com.opengamma.longdog.nodes.SVD;

/**
 * DOGMA v2.
 */
public class DOGMA2 {

  static {
    NativeLibraries.initialize();
  }

  public static OGNumeric copy(OGNumeric arg0) {
    return new COPY(arg0);
  }

  public static OGNumeric plus(OGNumeric arg0, OGNumeric arg1) {
    return new PLUS(arg0, arg1);
  }

  public static OGNumeric minus(OGNumeric arg0, OGNumeric arg1) {
    return new PLUS(arg0, new NEGATE(arg1));
  }

  public static OGResult svd(OGNumeric arg0) {
    return new OGResult(new SVD(arg0));
  }

  public static double[][] toDoubleArray(OGNumeric arg0) {
    return Materialisers.toDoubleArrayOfArrays(arg0);
  }

}
