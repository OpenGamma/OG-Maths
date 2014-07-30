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
 * Atan class
 */
public class ATAN extends OGExpr {

  @Override
  public ExprEnum getType() {
    return ExprEnum.ATAN_ENUM;
  }

  public ATAN(OGNumeric arg) {
    super(arg);
  }


}
