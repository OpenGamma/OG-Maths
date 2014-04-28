/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.nodes;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.lazy.OGExprVariadicReturn;

/**
 * LU class.
 */
public class LU extends OGExprVariadicReturn {

  private static int s_nPossibleResults = 2;

  @Override
  public ExprEnum getType() {
    return ExprEnum.LU_ENUM;
  }

  public LU(OGNumeric arg0) {
    super(arg0, s_nPossibleResults);
  }

  public OGNumeric getL() {
    return new SELECTRESULT(this, 0);
  }

  public OGNumeric getU() {
    return new SELECTRESULT(this, 1);
  }


}
