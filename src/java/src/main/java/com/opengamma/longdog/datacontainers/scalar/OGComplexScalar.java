/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.datacontainers.scalar;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;

/**
 * Complex Scalar Numeric class
 */
public abstract class OGComplexScalar extends OGScalar {
  private static ExprTypeEnum s_type = ExprTypeEnum.OGComplexScalar;

  @Override
  public ExprTypeEnum getType() {
    return s_type;
  }

}
