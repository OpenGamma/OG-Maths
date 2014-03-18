/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.nodes;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;
import com.opengamma.longdog.datacontainers.OGNumeric;
import com.opengamma.longdog.datacontainers.lazy.OGExpr;
import com.opengamma.longdog.datacontainers.scalar.OGIntegerScalar;

/**
 * Select result class.
 */
public class SELECTRESULT extends OGExpr {

  @Override
  public ExprTypeEnum getType() {
    return ExprTypeEnum.SELECTRESULT_ENUM;
  }

  public SELECTRESULT(OGNumeric arg0, int index) {
    super(arg0, new OGIntegerScalar(index));
  }

}
