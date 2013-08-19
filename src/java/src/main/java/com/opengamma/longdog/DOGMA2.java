/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog;

import com.opengamma.longdog.datacontainers.OGNumeric;
import com.opengamma.longdog.materialisers.Materialisers;
import com.opengamma.longdog.nodes.COPY;
import com.opengamma.longdog.nodes.MINUS;
import com.opengamma.longdog.nodes.PLUS;

/**
 * DOGMA v2.
 */
public class DOGMA2 {

  public static OGNumeric copy(OGNumeric arg0) {
    return new COPY(arg0);
  }

  public static OGNumeric plus(OGNumeric arg0, OGNumeric arg1) {
    return new PLUS(arg0, arg1);
  }

  public static OGNumeric minus(OGNumeric arg0, OGNumeric arg1) {
    return new MINUS(arg0, arg1);
  }

  public static double[][] toDoubleArray(OGNumeric arg0) {
    return Materialisers.toJDoubleArray(arg0);
  } 

}
