/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.nodes;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.lazy.OGExprVariadicReturn;

/**
 * QR class.
 */
public class QR extends OGExprVariadicReturn {

  private static int s_nPossibleResults = 2;

  @Override
  public ExprEnum getType() {
    return ExprEnum.QR_ENUM;
  }

  public QR(OGNumeric arg0) {
    super(arg0, s_nPossibleResults);
  }

  public OGNumeric getQ() {
    return new SELECTRESULT(this, 0);
  }

  public OGNumeric getR() {
    return new SELECTRESULT(this, 1);
  }


}
