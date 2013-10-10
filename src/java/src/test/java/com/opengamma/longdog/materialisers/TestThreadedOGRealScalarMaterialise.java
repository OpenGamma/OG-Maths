/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.longdog.materialisers;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import com.opengamma.longdog.datacontainers.scalar.OGRealScalar;
import com.opengamma.longdog.exceptions.MathsException;
import com.opengamma.longdog.testhelpers.ArraysHelpers;

import java.lang.Runnable;
import java.util.concurrent.Future;
import java.util.concurrent.ScheduledThreadPoolExecutor;
import java.util.Random;

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
