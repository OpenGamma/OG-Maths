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
 * Matrix Inverse class
 */
public class INV extends OGExpr {

  @Override
  public ExprEnum getType() {
    return ExprEnum.INV_ENUM;
  }

  public INV(OGNumeric arg) {
    super(arg);
  }


}
