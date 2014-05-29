/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

package com.opengamma.maths.fuzzer;

import java.util.LinkedList;
import java.util.List;

import com.opengamma.maths.helpers.Catchers;

/**
 * A fixed capacity FIFO container.
 * Behaviour is very basic. 
 * 1) Adding a new item to the container will append to the container.
 * 2) If the container is at capacity when the add() request occurs, 
 * the oldest element is dropped and the new added. 
 */
public class FixedSizeFIFO<T> {

  /**
   * The default capacity used for this container.
   */
  public static int DEFAULT_CAPACITY = 100;

  /**
   * The capacity of this container
   */
  private int _capacity;
  
  /**
   * The list that acts as backing storage for this container.
   */
  private List<T> _backing;

  /**
   * Construct a new {@link FixedSizeFIFO} with default capacity of {@code DEFAULT_CAPACITY}.
   */
  public FixedSizeFIFO() {
    this(DEFAULT_CAPACITY);
  }

  /**
   * Construct a new {@link FixedSizeFIFO} with a declared capacity.
   * @param capacity the fixed capacity of this container.
   */
  public FixedSizeFIFO(int capacity) {
    Catchers.catchValueShouldBeGreaterThanOrEqualToXFromArgList(capacity, 1, 1);
    _capacity = capacity;
    _backing = new LinkedList<T>();
  };

  /**
   * Add an element to the container, if the container is at capacity then the oldest
   * element in the container is dropped and the new added.
   * @param element
   */
  public void add(T element) {
    if (_backing.size() == _capacity) {
      _backing.remove(0);
    }
    _backing.add(element);
  }

  /**
   * Returns the number of elements in the container.
   * @return the number of elements in the container.
   */
  public int size() {
    return _backing.size();
  }

  /**
   * Retrieves the element at the specified location.
   * @param location the location of the element to return.
   * @return the element at {@code location}.
   */
  public T get(int location) {
    return _backing.get(location);
  }

  /**
   * Returns the capacity of the container.
   * @return the capacity of the container.
   */
  public int capacity() {
    return _capacity;
  }

}
