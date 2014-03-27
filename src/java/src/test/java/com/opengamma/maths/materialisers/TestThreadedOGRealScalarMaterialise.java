/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.materialisers;

import java.util.Random;
import java.util.concurrent.Future;
import java.util.concurrent.ScheduledThreadPoolExecutor;

import org.testng.annotations.Test;

import com.opengamma.maths.datacontainers.scalar.OGRealScalar;
import com.opengamma.maths.exceptions.MathsException;
import com.opengamma.maths.materialisers.Materialisers;
import com.opengamma.maths.testhelpers.ArraysHelpers;

public class TestThreadedOGRealScalarMaterialise {

  private final int NTASKS = 128;

  private static class Zergling implements Runnable {

    private static final int NITERATIONS = 512;

    @Override
    public void run() {
      double v = new Random().nextDouble();
      double[][] vaa = new double[][] {{v}};
      OGRealScalar input = new OGRealScalar(v);
      for (int i = 0; i < NITERATIONS; i++) {
        double[][] answer = Materialisers.toDoubleArrayOfArrays(input);
        if (!ArraysHelpers.ArraysEquals(answer, vaa)) {
          throw new MathsException("Output not equal to input, got: " + answer[0][0] + ", expected: " + v);
        }
      }
    }
  }

  @Test
  public void materialiseToJDoubleArray() throws Exception {
    ScheduledThreadPoolExecutor stpe = new ScheduledThreadPoolExecutor(NTASKS);
    Future<?> futures[] = new Future[NTASKS];
    for (int i = 0; i < NTASKS; i++) {
      futures[i] = stpe.submit(new Zergling());
    }
    for (int i = 0; i < NTASKS; i++) {
      futures[i].get();
    }
  }
}
