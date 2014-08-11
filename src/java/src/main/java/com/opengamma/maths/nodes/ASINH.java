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
 * Asinh class
 */
public class ASINH extends OGExpr {

  @Override
  public ExprEnum getType() {
    return ExprEnum.ASINH_ENUM;
  }

  public ASINH(OGNumeric arg) {
    super(arg);
  }


}
