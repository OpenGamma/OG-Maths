/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.datacontainers.scalar;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;

/**
 * Real Scalar Numeric class
 */
public class OGRealScalar extends OGScalar {
  private static ExprTypeEnum s_type = ExprTypeEnum.OGRealScalar;

  @Override
  public ExprTypeEnum getType() {
    return s_type;
  }

}
