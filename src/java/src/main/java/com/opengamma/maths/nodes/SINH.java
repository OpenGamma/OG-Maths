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
 * Sinh class
 */
public class SINH extends OGExpr {

  @Override
  public ExprEnum getType() {
    return ExprEnum.SINH_ENUM;
  }

  public SINH(OGNumeric arg) {
    super(arg);
  }


}
