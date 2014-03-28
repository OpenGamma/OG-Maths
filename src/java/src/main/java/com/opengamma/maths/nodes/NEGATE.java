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
 * Negate class
 */
public class NEGATE extends OGExpr {

  @Override
  public ExprEnum getType() {
    return ExprEnum.NEGATE_ENUM;
  }

  public NEGATE(OGNumeric arg) {
    super(arg);
  }


}
