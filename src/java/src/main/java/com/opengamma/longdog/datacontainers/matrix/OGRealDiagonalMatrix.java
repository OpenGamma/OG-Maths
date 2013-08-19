/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.datacontainers.matrix;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;

/**
 * Real Diagonal matrix type
 */
public class OGRealDiagonalMatrix extends OGDiagonalMatrix {
  private static ExprTypeEnum s_type = ExprTypeEnum.OGRealDiagonalMatrix;

  @Override
  public ExprTypeEnum getType() {
    return s_type;
  }
}
