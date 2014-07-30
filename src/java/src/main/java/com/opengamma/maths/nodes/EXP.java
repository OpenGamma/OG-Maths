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
 * Exp class
 */
public class EXP extends OGExpr {

  @Override
  public ExprEnum getType() {
    return ExprEnum.EXP_ENUM;
  }

  public EXP(OGNumeric arg) {
    super(arg);
  }


}
