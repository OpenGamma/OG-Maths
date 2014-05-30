/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */

package com.opengamma.maths.mem;

import java.util.Arrays;

import org.testng.annotations.Test;

import com.opengamma.maths.nativeloader.NativeLibraries;

@Test
public class AVXAlignedMemoryBlockTest {
  static {
    NativeLibraries.initialize();
  }

  private static int sizeOfInt = Stdlib.SIZEOF_JINT;
  private static int sizeOfLong = Stdlib.SIZEOF_JLONG;
  private static int sizeOfFloat = Stdlib.SIZEOF_JFLOAT;
  private static int sizeOfDouble = Stdlib.SIZEOF_JDOUBLE;

  public void testConstructor() {
    boolean caught = false;
    try {
      new AVXAlignedMemoryBlock(0);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught");
    }
  }

  public void testAllocateAndFree() {
    boolean caught = false;

    // zero amount
    try {
      new AVXAlignedMemoryBlock(0);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught");
    }

    // neg amount
    try {
      new AVXAlignedMemoryBlock(-4);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught");
    }

    // try allocating a block without freeing first
    AVXAlignedMemoryBlock tmp = new AVXAlignedMemoryBlock(128);
    if (tmp.getFreedStatus() != false) {
      throw new RuntimeException("Reference should not be free and yet it is!");
    }
    try {
      tmp.allocate(256);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught");
    }

    // free, now try allocating a new block with an illegal size
    tmp.free();
    if (tmp.getFreedStatus() != true) {
      throw new RuntimeException("Reference should be free and yet it isn't!");
    }
    try {
      tmp.allocate(-1);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught");
    }
    // allocate will have failed so reference should still be free
    if (tmp.getFreedStatus() != true) {
      throw new RuntimeException("Reference should be free and yet it isn't!");
    }

    // try double free-ing as the current reference does not hold an allocation
    try {
      tmp.free();
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught");
    }

    if (tmp.getFreedStatus() != true) {
      throw new RuntimeException("Reference should not be free and yet it is!");
    }

  }

  public void testMemset()
  {
    int blockSizeInBytes = 64;
    int intCount = blockSizeInBytes / sizeOfInt;
    byte valueA = (byte) 0xaa;
    byte valueF = (byte) 0xff;
    AVXAlignedMemoryBlock tmp = new AVXAlignedMemoryBlock(blockSizeInBytes);
    tmp.memset(0, intCount * sizeOfInt, valueA); // memory = 0xA 
    tmp.memset(6 * sizeOfInt, (intCount - 6) * sizeOfInt, valueF);
    int OxA = 0xAAAAAAAA;
    int OxF = 0xFFFFFFFF;
    int[] expected = new int[] { OxA, OxA, OxA, OxA, OxA, OxA, OxF,
      OxF, OxF, OxF, OxF, OxF, OxF, OxF, OxF, OxF };
    int[] read = new int[intCount];
    tmp.readIntArray(read, 0, 0, intCount);
    if (!Arrays.equals(read, expected)) {
      throw new RuntimeException("Arrays do not match");
    }

    // test memset at a bad offset so the invalid access code is tripped
    boolean caught = false;
    try {
      tmp.memset(-1, intCount * sizeOfInt, valueA); // memory = 0xA
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught");
    }

    tmp.free();
  }

  public void testMemsetOverRun() {
    AVXAlignedMemoryBlock tmp = new AVXAlignedMemoryBlock(10);
    boolean caught = false;
    try
    {
      tmp.memset(11, 0, (byte) 0xFF); // memory = 0xA
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught");
    }

    caught = false;
    try
    {
      tmp.memset(6, 5, (byte) 0xFF); // memory = 0xA
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught");
    }
  }

  public void testDoubleArrayReadWrite()
  {
    AVXAlignedMemoryBlock tmp = new AVXAlignedMemoryBlock(256);
    double[] input = new double[] { 1, 2, 3, 4 };
    double[] read, expected;
    tmp.memset(0L, 16 * sizeOfDouble, (byte) 0x00);
    tmp.writeDoubleArray(input, 0, 0, input.length); // [1,2,3,4,0,0,0,0,0,0]
    tmp.writeDoubleArray(input, 0, 4 * sizeOfDouble, input.length - 1); // [1,2,3,4,1,2,3,0,0,0]
    tmp.writeDoubleArray(input, 2, 7 * sizeOfDouble, input.length - 2); // [1,2,3,4,1,2,3,3,4,0]    

    read = new double[10];
    tmp.readDoubleArray(read, 1, 1 * sizeOfDouble, 8);
    expected = new double[] { 0.0, 2.0, 3.0, 4.0, 1.0, 2.0, 3.0, 3.0, 4.0, 0.0 };
    if (!Arrays.equals(read, expected)) {
      throw new RuntimeException("Arrays do not match");
    }

    read = new double[10];
    tmp.readDoubleArray(read, 0, 1 * sizeOfDouble, 6);
    expected = new double[] { 2.0, 3.0, 4.0, 1.0, 2.0, 3.0, 0.0, 0.0, 0.0, 0.0 };
    if (!Arrays.equals(read, expected)) {
      throw new RuntimeException("Arrays do not match");
    }

    boolean caught;

    // Test write OOB on input
    caught = false;
    try {
      tmp.writeDoubleArray(input, 0, 0, input.length + 1);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test write OOB on input");
    }

    // Test write offset too large
    caught = false;
    try {
      tmp.writeDoubleArray(input, 2, 0, input.length - 1);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test write offset too large");
    }

    // Test write buffer overrun
    caught = false;
    try {
      tmp.writeDoubleArray(input, 0, 232, input.length);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test write buffer overrun");
    }

    // Test read too large for array
    caught = false;
    try
    {
      tmp.readDoubleArray(read, 0, 1 * sizeOfDouble, 11);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test read too large for array");
    }

    // Test read offset too large for array
    caught = false;
    try {
      tmp.readDoubleArray(read, 2, 1 * sizeOfDouble, 9);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test read offset too large for array");
    }

    // Test read buffer overrun
    caught = false;
    try {
      tmp.readDoubleArray(read, 0, 232, read.length);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test read buffer overrun");
    }

    tmp.free();
  }

  public void testFloatArrayReadWrite() {
    AVXAlignedMemoryBlock tmp = new AVXAlignedMemoryBlock(256);
    float[] input = new float[] { 1, 2, 3, 4 };
    float[] read, expected;
    tmp.memset(0L, 10 * sizeOfFloat, (byte) 0x00);
    tmp.writeFloatArray(input, 0, 0, input.length); // [1,2,3,4,0,0,0,0,0,0]
    tmp.writeFloatArray(input, 0, 4 * sizeOfFloat, input.length - 1); // [1,2,3,4,1,2,3,0,0,0]
    tmp.writeFloatArray(input, 2, 7 * sizeOfFloat, input.length - 2); // [1,2,3,4,1,2,3,3,4,0]    

    read = new float[10];
    tmp.readFloatArray(read, 1, 1 * sizeOfFloat, 8);
    expected = new float[] { 0, 2, 3, 4, 1, 2, 3, 3, 4, 0 };
    if (!Arrays.equals(read, expected)) {
      throw new RuntimeException("Arrays do not match");
    }

    read = new float[10];
    tmp.readFloatArray(read, 0, 1 * sizeOfFloat, 6);
    expected = new float[] { 2, 3, 4, 1, 2, 3, 0, 0, 0, 0 };
    if (!Arrays.equals(read, expected)) {
      throw new RuntimeException("Arrays do not match");
    }

    boolean caught;

    // Test write OOB on input
    caught = false;
    try {
      tmp.writeFloatArray(input, 0, 0, input.length + 1);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test write OOB on input");
    }

    // Test write offset too large
    caught = false;
    try {
      tmp.writeFloatArray(input, 2, 0, input.length - 1);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test write offset too large");
    }

    // Test write buffer overrun
    caught = false;
    try {
      tmp.writeFloatArray(input, 0, 248, input.length);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test write buffer overrun");
    }

    // Test read too large for array
    caught = false;
    try {
      tmp.readFloatArray(read, 0, 1 * sizeOfDouble, 11);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test read too large for array");
    }

    // Test read offset too large for array
    caught = false;
    try {
      tmp.readFloatArray(read, 2, 1 * sizeOfDouble, 9);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test read offset too large for array");
    }

    // Test read buffer overrun
    caught = false;
    try
    {
      tmp.readFloatArray(read, 0, 248, read.length);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test read buffer overrun");
    }

    tmp.free();
  }

  public void testIntArrayReadWrite() {
    AVXAlignedMemoryBlock tmp = new AVXAlignedMemoryBlock(256);
    int[] input = new int[] { 1, 2, 3, 4 };
    int[] read, expected;
    tmp.memset(0L, 10 * sizeOfInt, (byte) 0x00);
    tmp.writeIntArray(input, 0, 0, input.length); // [1,2,3,4,0,0,0,0,0,0]
    tmp.writeIntArray(input, 0, 4 * sizeOfInt, input.length - 1); // [1,2,3,4,1,2,3,0,0,0]
    tmp.writeIntArray(input, 2, 7 * sizeOfInt, input.length - 2); // [1,2,3,4,1,2,3,3,4,0]    

    read = new int[10];
    tmp.readIntArray(read, 1, 1 * sizeOfInt, 8);
    expected = new int[] { 0, 2, 3, 4, 1, 2, 3, 3, 4, 0 };
    if (!Arrays.equals(read, expected)) {
      throw new RuntimeException("Arrays do not match");
    }

    read = new int[10];
    tmp.readIntArray(read, 0, 1 * sizeOfInt, 6);
    expected = new int[] { 2, 3, 4, 1, 2, 3, 0, 0, 0, 0 };
    if (!Arrays.equals(read, expected)) {
      throw new RuntimeException("Arrays do not match");
    }

    boolean caught;

    // Test write OOB on input
    caught = false;
    try {
      tmp.writeIntArray(input, 0, 0, input.length + 1);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test write OOB on input");
    }

    // Test write offset too large
    caught = false;
    try {
      tmp.writeIntArray(input, 2, 0, input.length - 1);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test write offset too large");
    }

    // Test write buffer overrun
    caught = false;
    try {
      tmp.writeIntArray(input, 0, 248, input.length);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test write buffer overrun");
    }

    // Test read too large for array
    caught = false;
    try {
      tmp.readIntArray(read, 0, 1 * sizeOfDouble, 11);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test read too large for array");
    }

    // Test read offset too large for array
    caught = false;
    try {
      tmp.readIntArray(read, 2, 1 * sizeOfDouble, 9);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test read offset too large for array");
    }

    // Test read buffer overrun
    caught = false;
    try {
      tmp.readIntArray(read, 0, 248, read.length);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test read buffer overrun");
    }

    tmp.free();
  }

  public void testLongArrayReadWrite() {
    AVXAlignedMemoryBlock tmp = new AVXAlignedMemoryBlock(256);
    long[] input = new long[] { 1, 2, 3, 4 };
    long[] read, expected;
    tmp.memset(0L, 10 * sizeOfLong, (byte) 0x00);
    tmp.writeLongArray(input, 0, 0, input.length); // [1,2,3,4,0,0,0,0,0,0]
    tmp.writeLongArray(input, 0, 4 * sizeOfLong, input.length - 1); // [1,2,3,4,1,2,3,0,0,0]
    tmp.writeLongArray(input, 2, 7 * sizeOfLong, input.length - 2); // [1,2,3,4,1,2,3,3,4,0]    

    read = new long[10];
    tmp.readLongArray(read, 1, 1 * sizeOfLong, 8);
    expected = new long[] { 0, 2, 3, 4, 1, 2, 3, 3, 4, 0 };
    if (!Arrays.equals(read, expected)) {
      throw new RuntimeException("Arrays do not match");
    }

    read = new long[10];
    tmp.readLongArray(read, 0, 1 * sizeOfLong, 6);
    expected = new long[] { 2, 3, 4, 1, 2, 3, 0, 0, 0, 0 };
    if (!Arrays.equals(read, expected)) {
      throw new RuntimeException("Arrays do not match");
    }

    boolean caught;

    // Test write OOB on input
    caught = false;
    try {
      tmp.writeLongArray(input, 0, 0, input.length + 1);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test write OOB on input");
    }

    // Test write offset too large
    caught = false;
    try {
      tmp.writeLongArray(input, 2, 0, input.length - 1);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test write offset too large");
    }

    // Test write buffer overrun
    caught = false;
    try {
      tmp.writeLongArray(input, 0, 232, input.length);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test write buffer overrun");
    }

    // Test read too large for array
    caught = false;
    try {
      tmp.readLongArray(read, 0, 1 * sizeOfDouble, 11);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test read too large for array");
    }

    // Test read offset too large for array
    caught = false;
    try {
      tmp.readLongArray(read, 2, 1 * sizeOfDouble, 9);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test read offset too large for array");
    }

    // Test read buffer overrun
    caught = false;
    try {
      tmp.readLongArray(read, 0, 232, read.length);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Exception was not caught: Test read buffer overrun");
    }

    tmp.free();
  }

  public void testGetBaseAddress() {
    AlignedAlignedWriteBuffer buf;
    buf = new AlignedAlignedWriteBuffer(32);
    long addr = buf.getBaseAddress();
    if (addr % 32 != 0) {
      throw new RuntimeException("Base address is not AVX aligned, fail.");
    }
  }

}
