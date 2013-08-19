/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.datacontainers.scalar;

import com.opengamma.longdog.datacontainers.OGArray;

/**
 * Scalar Numeric class
 */
public abstract class OGScalar extends OGArray {

  private double[] _data;

  @Override
  public double[] getData() {
    return _data;
  }
}
