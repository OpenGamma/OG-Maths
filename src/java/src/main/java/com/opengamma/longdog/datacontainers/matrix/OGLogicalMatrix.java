/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.longdog.datacontainers.matrix;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;

/**
 * Logical matrix
 */
public class OGLogicalMatrix extends OGRealMatrix {

  private static ExprTypeEnum s_type = ExprTypeEnum.OGLogicalMatrix;

  public OGLogicalMatrix(double[] data, int rows, int cols) {
    super(data, rows, cols);
  }

  @Override
  public ExprTypeEnum getType() {
    return s_type;
  }

}
