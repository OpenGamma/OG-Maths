/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.nodes;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.lazy.OGExpr;

/**
 * Tanh class
 */
public class TANH extends OGExpr {

  @Override
  public ExprEnum getType() {
    return ExprEnum.TANH_ENUM;
  }

  public TANH(OGNumeric arg) {
    super(arg);
  }


}
