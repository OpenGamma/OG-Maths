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
 * Sin class
 */
public class SIN extends OGExpr {

  @Override
  public ExprEnum getType() {
    return ExprEnum.SIN_ENUM;
  }

  public SIN(OGNumeric arg) {
    super(arg);
  }


}
