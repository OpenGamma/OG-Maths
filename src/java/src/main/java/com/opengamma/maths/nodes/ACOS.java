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
 * Acos class
 */
public class ACOS extends OGExpr {

  @Override
  public ExprEnum getType() {
    return ExprEnum.ACOS_ENUM;
  }

  public ACOS(OGNumeric arg) {
    super(arg);
  }


}
