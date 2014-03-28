/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.nodes;

import com.opengamma.maths.datacontainers.ExprEnum;
import com.opengamma.maths.datacontainers.OGNumeric;
import com.opengamma.maths.datacontainers.lazy.OGExpr;
import com.opengamma.maths.datacontainers.scalar.OGIntegerScalar;

/**
 * Select result class.
 */
public class SELECTRESULT extends OGExpr {

  @Override
  public ExprEnum getType() {
    return ExprEnum.SELECTRESULT_ENUM;
  }

  public SELECTRESULT(OGNumeric arg0, int index) {
    super(arg0, new OGIntegerScalar(index));
  }

}
