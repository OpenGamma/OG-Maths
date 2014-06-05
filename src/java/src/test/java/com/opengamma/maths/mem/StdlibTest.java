/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.maths.mem;

import org.testng.annotations.Test;

import sun.misc.Unsafe;

import com.opengamma.maths.mem.Stdlib;
import com.opengamma.maths.nativeloader.NativeLibraries;

/**
 * Test methods for stdlib
 */
@SuppressWarnings("restriction")
@Test
public class StdlibTest {

  static {
    NativeLibraries.initialize();
  }

  public void allocateAndFreeCheck() {
    long addr = 0;
    try {
      addr = Stdlib.allocateAlignedMemory(100);
    } catch (Exception e) {
      throw new RuntimeException("Cannot allocate memory.");
    }
    Stdlib.freeAlignedMemory(addr);

    boolean caught = false;
    // bad alloc
    try {
      addr = Stdlib.allocateAlignedMemory(0);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception not caught");
    }

  }

  public void nativePointerSizeCheck() {
    int size = Stdlib.checkNativePointerSize();
    if (size != 8) {
      throw new RuntimeException("sizeof(uintptr_t) should be 8.");
    }
  }

  public void unsafeGetterCheck() {
    Unsafe u = Stdlib.getUnsafe();
    if (u.addressSize() != Stdlib.checkNativePointerSize()) {
      throw new RuntimeException("addressSize reported should be 8, the same as sizeof(uintptr_t).");
    }
  }

  public void incrementAddressCheck() {
    long addr = 0;
    try {
      addr = Stdlib.allocateAlignedMemory(100);
    } catch (Exception e) {
      throw new RuntimeException("Cannot allocate memory.");
    }
    long addrplusplus = Stdlib.incrementAddress(addr);
    if (addr + Stdlib.checkNativePointerSize() != addrplusplus) {
      Stdlib.freeAlignedMemory(addr);
      throw new RuntimeException("Doing pointer arithmetic from java is failing.");
    }
    Stdlib.freeAlignedMemory(addr);
  }

}
