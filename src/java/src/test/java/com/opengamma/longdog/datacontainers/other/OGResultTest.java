/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.datacontainers.other;

import static org.testng.Assert.assertTrue;

import org.testng.annotations.Test;

import com.opengamma.longdog.datacontainers.ExprTypeEnum;
import com.opengamma.longdog.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.longdog.datacontainers.scalar.OGIntegerScalar;
import com.opengamma.longdog.exceptions.MathsException;
import com.opengamma.longdog.exceptions.MathsExceptionNullPointer;
import com.opengamma.longdog.nodes.SELECTRESULT;
import com.opengamma.longdog.nodes.SVD;

/**
 * Tests the OGResult class
 */
@Test
public class OGResultTest {

  @Test(expectedExceptions = MathsExceptionNullPointer.class)
  public void constrNullInTest() {
    new OGResult(null);
  }

  @Test(expectedExceptions = MathsException.class)
  public void getOOB1Test() {
    OGResult tmp = new OGResult(new SVD(new OGRealDenseMatrix(1)));
    tmp.get(-1);
  }

  @Test(expectedExceptions = MathsException.class)
  public void getOOB2Test() {
    OGResult tmp = new OGResult(new SVD(new OGRealDenseMatrix(1)));
    tmp.get(3);
  }

  @Test
  public void getOkTest() {
    SVD svdobj = new SVD(new OGRealDenseMatrix(1));
    OGResult tmp = new OGResult(svdobj);
    assertTrue(tmp.get(0) instanceof SELECTRESULT);
    assertTrue(tmp.get(0).getType().equals(ExprTypeEnum.SELECTRESULT_ENUM));
    assertTrue(((OGIntegerScalar) ((SELECTRESULT) tmp.get(0)).getArg(1)).getValue() == 0);
    assertTrue(tmp.get(1) instanceof SELECTRESULT);
    assertTrue(tmp.get(1).getType().equals(ExprTypeEnum.SELECTRESULT_ENUM));
    assertTrue(((OGIntegerScalar) ((SELECTRESULT) tmp.get(1)).getArg(1)).getValue() == 1);
    assertTrue(tmp.get(2) instanceof SELECTRESULT);
    assertTrue(tmp.get(2).getType().equals(ExprTypeEnum.SELECTRESULT_ENUM));
    assertTrue(((OGIntegerScalar) ((SELECTRESULT) tmp.get(2)).getArg(1)).getValue() == 2);
  }

}
