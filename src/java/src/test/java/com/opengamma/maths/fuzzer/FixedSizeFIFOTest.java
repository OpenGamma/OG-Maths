/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.fuzzer;

import static org.testng.AssertJUnit.assertTrue;

import org.testng.annotations.Test;

import com.opengamma.maths.exceptions.MathsExceptionIllegalArgument;

/**
 * Tests the fixed size FIFO class.
 */
@Test
public class FixedSizeFIFOTest {

  @Test
  public void defaultCtorTest() {
    FixedSizeFIFO<Integer> f = new FixedSizeFIFO<>();
    assertTrue(f.capacity() == FixedSizeFIFO.DEFAULT_CAPACITY);
  }

  @Test(expectedExceptions = MathsExceptionIllegalArgument.class)
  public void sizedCtorBadArgTest() {
    new FixedSizeFIFO<>(-1);
  }

  @Test
  public void sizedCtorTest() {
    int capacity = 53;
    FixedSizeFIFO<Integer> f = new FixedSizeFIFO<>(capacity);
    assertTrue(f.capacity() == capacity);
  }

  @Test
  public void behaviourTest() {
    int capacity = 5;
    FixedSizeFIFO<Integer> f = new FixedSizeFIFO<>(capacity);

    // fill the container
    for (int k = 0; k < capacity; k++) {
      f.add(k);
      assertTrue(f.get(k) == k);
      assertTrue(f.size() == k + 1);
    }

    // add another one, this should bump off f(0), shift everything along, then add to the end
    f.add(capacity);

    // check everything has moved correctly
    for (int k = 0; k < capacity; k++) {
      assertTrue(f.get(k) == k + 1);
      assertTrue(f.size() == f.capacity());
    }
  }

  @Test(expectedExceptions = IndexOutOfBoundsException.class)
  public void throwOnBadAccessTest() {
    int capacity = 5;
    FixedSizeFIFO<Integer> f = new FixedSizeFIFO<>(capacity);
    f.add(10);

    // try get on element 1, will fail as container just has 1 element at location 0!
    f.get(1);
  }
  
  @Test(expectedExceptions = IndexOutOfBoundsException.class)
  public void throwOnAccessAboveCapacityTest() {
    int capacity = 5;
    FixedSizeFIFO<Integer> f = new FixedSizeFIFO<>(capacity);

    // fill the container
    for (int k = 0; k < capacity; k++) {
      f.add(k);
    }

    // try get on element "capacity", will fail as container indexes from 0!
    f.get(capacity);
  }

}
