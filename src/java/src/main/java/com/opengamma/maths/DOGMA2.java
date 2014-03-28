/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths;

import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.other.OGResult;
import com.opengamma.maths.materialisers.Materialisers;
import com.opengamma.maths.nativeloader.NativeLibraries;
import com.opengamma.maths.nodes.COPY;
import com.opengamma.maths.nodes.NEGATE;
import com.opengamma.maths.nodes.PLUS;
import com.opengamma.maths.nodes.SVD;

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
