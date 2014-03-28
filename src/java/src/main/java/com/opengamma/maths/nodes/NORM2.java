/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.nodes;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.lazy.OGExpr;

/**
 * Norm2 class
 */
public class NORM2 extends OGExpr {

  @Override
  public ExprEnum getType() {
    return ExprEnum.NORM2_ENUM;
  }

  public NORM2(OGNumeric arg) {
    super(arg);
  }


}
