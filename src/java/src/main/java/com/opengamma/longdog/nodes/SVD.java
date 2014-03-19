/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.nodes;

import com.opengamma.longdog.datacontainers.ExprEnum;
import com.opengamma.longdog.datacontainers.OGNumeric;
import com.opengamma.longdog.datacontainers.lazy.OGExprVariadicReturn;

/**
 * SVD class.
 */
public class SVD extends OGExprVariadicReturn {

  private static int s_nPossibleResults = 3;

  @Override
  public ExprEnum getType() {
    return ExprEnum.SVD_ENUM;
  }

  public SVD(OGNumeric arg0) {
    super(arg0, s_nPossibleResults);
  }

  public OGNumeric getU() {
    return new SELECTRESULT(this, 0);
  }

  public OGNumeric getS() {
    return new SELECTRESULT(this, 1);
  }

  public OGNumeric getVT() {
    return new SELECTRESULT(this, 2);
  }

}
