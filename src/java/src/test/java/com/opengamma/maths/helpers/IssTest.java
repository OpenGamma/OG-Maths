/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
package com.opengamma.maths.helpers;

import static org.testng.AssertJUnit.assertTrue;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.maths.datacontainers.OGTerminal;
import com.opengamma.maths.datacontainers.matrix.OGComplexDenseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGComplexDiagonalMatrix;
import com.opengamma.maths.datacontainers.matrix.OGComplexSparseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGLogicalMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealDenseMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealDiagonalMatrix;
import com.opengamma.maths.datacontainers.matrix.OGRealSparseMatrix;
import com.opengamma.maths.datacontainers.scalar.OGComplexScalar;
import com.opengamma.maths.datacontainers.scalar.OGIntegerScalar;
import com.opengamma.maths.datacontainers.scalar.OGRealScalar;

/**
 * Checks the Iss class.
 */
public class IssTest {

  /**
   * Helper class.
   */
  private class IssTestHelper {
    /**
     * The terminal to test.
     */
    private OGTerminal _term;

    /**
     * The expected state.
     */
    private boolean _expected;

    /**
     * Construct a new test instance with a terminal and its
     * expected state via isFOO().
     * @param term the terminal to check.
     * @param expected the expected state.
     */
    IssTestHelper(final OGTerminal term, final boolean expected) {
      _term = term;
      _expected = expected;
    }

    /**
     * Gets the terminal.
     * @return the terminal.
     */
    public final OGTerminal getTerm() {
      return _term;
    }

    /**
     * Gets the expected state.
     * @return the expected state.
     */
    public final boolean getExpected() {
      return _expected;
    }
  }

  private static OGTerminal _terminals[];
  static {
    _terminals = new OGTerminal[10];
    int ptr = 0;
    _terminals[ptr++] = new OGRealScalar(1);
    _terminals[ptr++] = new OGIntegerScalar(1);
    _terminals[ptr++] = new OGComplexScalar(new double[] { 1, 2 });
    _terminals[ptr++] = new OGRealDenseMatrix(1);
    _terminals[ptr++] = new OGLogicalMatrix(new double[][] { { 1 } });
    _terminals[ptr++] = new OGComplexDenseMatrix(new double[][] { { 1 } });
    _terminals[ptr++] = new OGRealDiagonalMatrix(1);
    _terminals[ptr++] = new OGComplexDiagonalMatrix(1);
    _terminals[ptr++] = new OGRealSparseMatrix(new double[][] { { 1 } });
    _terminals[ptr++] = new OGComplexSparseMatrix(new double[][] { { 1 } });
  }

  @DataProvider
  public Object[][] realTruthTable() {
    Object[][] obj = new Object[10][1];
    int ptr = 0;
    obj[ptr][0] = new IssTestHelper(_terminals[ptr], true);
    ptr++;
    obj[ptr][0] = new IssTestHelper(_terminals[ptr], true);
    ptr++;
    obj[ptr][0] = new IssTestHelper(_terminals[ptr], false);
    ptr++;
    obj[ptr][0] = new IssTestHelper(_terminals[ptr], true);
    ptr++;
    obj[ptr][0] = new IssTestHelper(_terminals[ptr], true);
    ptr++;
    obj[ptr][0] = new IssTestHelper(_terminals[ptr], false);
    ptr++;
    obj[ptr][0] = new IssTestHelper(_terminals[ptr], true);
    ptr++;
    obj[ptr][0] = new IssTestHelper(_terminals[ptr], false);
    ptr++;
    obj[ptr][0] = new IssTestHelper(_terminals[ptr], true);
    ptr++;
    obj[ptr][0] = new IssTestHelper(_terminals[ptr], false);
    return obj;
  }

  @Test(dataProvider = "realTruthTable")
  public void checkIsReal(final IssTestHelper test) {
    assertTrue(Iss.isReal(test.getTerm()) == test.getExpected());
  }

  @Test(dataProvider = "realTruthTable")
  public void checkIsComplex(final IssTestHelper test) {
    assertTrue(Iss.isComplex(test.getTerm()) == !test.getExpected());
  }

}
