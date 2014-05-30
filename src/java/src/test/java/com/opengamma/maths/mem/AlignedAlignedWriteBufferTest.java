/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */

package com.opengamma.maths.mem;

import static org.testng.AssertJUnit.assertTrue;

import java.util.Arrays;

import org.testng.annotations.Test;

import com.opengamma.maths.nativeloader.NativeLibraries;

@Test
public class AlignedAlignedWriteBufferTest {
  static {
    NativeLibraries.initialize();
  }

  public void testAllocate() {
    AlignedAlignedWriteBuffer buf;

    buf = new AlignedAlignedWriteBuffer(63, 32);
    if (buf.size() != 64) {
      throw new RuntimeException("Allocation width is wrong. Expected 64 got " +
          buf.size());
    }
    buf.free();

    buf = new AlignedAlignedWriteBuffer(64, 32);
    if (buf.size() != 64) {
      throw new RuntimeException("Allocation width is wrong. Expected 64 got " +
          buf.size());
    }
    buf.free();

    buf = new AlignedAlignedWriteBuffer(65, 32);
    if (buf.size() != 96) {
      throw new RuntimeException("Allocation width is wrong. Expected 96 got " +
          buf.size());
    }
    buf.free();

    buf = new AlignedAlignedWriteBuffer(127, 32);
    if (buf.size() != 128) {
      throw new RuntimeException(
          "Allocation width is wrong. Expected 128 got " + buf.size());
    }
    buf.free();

    buf = new AlignedAlignedWriteBuffer(128, 32);
    if (buf.size() != 128) {
      throw new RuntimeException(
          "Allocation width is wrong. Expected 128 got " + buf.size());
    }
    buf.free();

    buf = new AlignedAlignedWriteBuffer(129, 32);
    if (buf.size() != 160) {
      throw new RuntimeException(
          "Allocation width is wrong. Expected 160 got " + buf.size());
    }
    buf.free();

    buf = new AlignedAlignedWriteBuffer(199, 32);
    if (buf.size() != 224) {
      throw new RuntimeException(
          "Allocation width is wrong. Expected 224 got " + buf.size());
    }
    buf.free();

    buf = new AlignedAlignedWriteBuffer(64, 16);
    if (buf.size() != 64) {
      throw new RuntimeException("Allocation width is wrong. Expected 64 got " +
          buf.size());
    }
    buf.free();

    // check catch on giant malloc
    boolean caught = false;
    try {
      buf = new AlignedAlignedWriteBuffer(2 * AlignedAlignedWriteBuffer.MAX_SIZE, 32);
    } catch (RuntimeException ex) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Giant malloc was accepted when it should have thrown.");
    }
  }

  private boolean checkBasePointerAlignment(long pointer, int align) {
    if (pointer % (long) align == 0L) {
      return true;
    } else {
      return false;
    }
  }

  public void testAlign() {
    AlignedAlignedWriteBuffer buf;
    buf = new AlignedAlignedWriteBuffer(63);
    int align = buf.getDefaultAlignment();
    if (!checkBasePointerAlignment(buf.getBaseAddress(), align)) {
      throw new RuntimeException(
          "Base address is not a multiple of align. Base address is " +
              buf.getBaseAddress() + " alignment is " +
              buf.getDefaultAlignment());
    }
    buf.free();
  }

  public void testReadWriteDoubleArrayMethod() {
    AlignedAlignedWriteBuffer buf;
    int bufWidth = 224;
    buf = new AlignedAlignedWriteBuffer(199, 32);
    if (buf.bytesRemaining() != buf.size()) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in bytesRemaining().");
    }
    if (buf.getCurrentOffset() != 0) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in getCurrentOffset().");
    }
    double[] tmp = new double[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
      14, 15, 16 };
    buf.writeAlignedDoubleArray(tmp);
    int bytesRemaining, currentOffset;
    currentOffset = 16 * Stdlib.SIZEOF_JDOUBLE;
    bytesRemaining = bufWidth - currentOffset;
    if (buf.bytesRemaining() != bytesRemaining) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in bytesRemaining().");
    }
    if (buf.getCurrentOffset() != currentOffset) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in getCurrentOffset().");
    }
    double[] ret, expected;
    ret = new double[9];
    expected = new double[] { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    buf.readAlignedDoubleArray(ret, 0);
    if (!Arrays.equals(expected, ret)) {
      throw new RuntimeException("Arrays do not match");
    }
    ret = new double[5];
    expected = new double[] { 9, 10, 11, 12, 13 };
    buf.readAlignedDoubleArray(ret, 8);
    if (!Arrays.equals(expected, ret)) {
      throw new RuntimeException("Arrays do not match");
    }

    boolean caught = false;
    // check invalid access
    try {
      buf.readAlignedDoubleArray(new double[1], 17); // currently written 16, so 17 is OOB
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

    // check invalid access, negative position
    try {
      buf.readAlignedDoubleArray(new double[1], -1);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

    caught = false;
    try { // 1.75 * tmp fits in this buffer
      buf.writeAlignedDoubleArray(tmp);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid write");
    }

    caught = false;
    buf.writeAlignedDoubleArray(new double[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 });
    if (buf.bytesRemaining() != 0) {
      throw new RuntimeException("Buffer should be full and it's reported as not being!");
    }

    try {
      buf.readAlignedDoubleArray(expected, buf.size() - expected.length + 1); // attempt read 1 more than the buffer has
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

    buf.free();
  }

  public void testReadWriteFloatArrayMethod() {
    AlignedAlignedWriteBuffer buf;
    int bufWidth = 224;
    buf = new AlignedAlignedWriteBuffer(199, 32);
    if (buf.bytesRemaining() != buf.size()) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in bytesRemaining().");
    }
    if (buf.getCurrentOffset() != 0) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in getCurrentOffset().");
    }
    float[] tmp = new float[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
      14, 15, 16 };
    buf.writeAlignedFloatArray(tmp);
    int bytesRemaining, currentOffset;
    currentOffset = 16 * Stdlib.SIZEOF_JFLOAT;
    bytesRemaining = bufWidth - currentOffset;
    if (buf.bytesRemaining() != bytesRemaining) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in bytesRemaining().");
    }
    if (buf.getCurrentOffset() != currentOffset) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in getCurrentOffset().");
    }
    float[] ret, expected;
    ret = new float[9];
    expected = new float[] { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    buf.readAlignedFloatArray(ret, 0);
    if (!Arrays.equals(expected, ret)) {
      throw new RuntimeException("Arrays do not match");
    }
    ret = new float[5];
    expected = new float[] { 9, 10, 11, 12, 13 };
    buf.readAlignedFloatArray(ret, 8);
    if (!Arrays.equals(expected, ret)) {
      throw new RuntimeException("Arrays do not match");
    }

    boolean caught = false;
    // check invalid access
    try {
      buf.readAlignedFloatArray(new float[1], 17); // currently written 16 , so 17 is OOB
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

    // check invalid access, negative position
    try {
      buf.readAlignedFloatArray(new float[1], -1);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

    caught = false;
    try { // 3.5 * tmp fits in this buffer
      buf.writeAlignedFloatArray(tmp);
      buf.writeAlignedFloatArray(tmp);
      buf.writeAlignedFloatArray(tmp);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid write");
    }

    caught = false;
    buf.writeAlignedFloatArray(new float[] { 1, 2, 3, 4 });
    if (buf.bytesRemaining() != 0) {
      throw new RuntimeException("Buffer should be full and it's reported as not being!");
    }

    try {
      buf.readAlignedFloatArray(expected, buf.size() - expected.length + 1); // attempt read 1 more than the buffer has
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

    buf.free();
  }

  public void testReadWriteIntArrayMethod() {
    AlignedAlignedWriteBuffer buf;
    int bufWidth = 224;
    buf = new AlignedAlignedWriteBuffer(199, 32);
    if (buf.bytesRemaining() != buf.size()) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in bytesRemaining().");
    }
    if (buf.getCurrentOffset() != 0) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in getCurrentOffset().");
    }
    int[] tmp = new int[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
      14, 15, 16 };
    buf.writeAlignedIntArray(tmp);
    int bytesRemaining, currentOffset;
    currentOffset = 16 * Stdlib.SIZEOF_JINT;
    bytesRemaining = bufWidth - currentOffset;
    if (buf.bytesRemaining() != bytesRemaining) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in bytesRemaining().");
    }
    if (buf.getCurrentOffset() != currentOffset) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in getCurrentOffset().");
    }
    int[] ret, expected;
    ret = new int[9];
    expected = new int[] { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    buf.readAlignedIntArray(ret, 0);
    if (!Arrays.equals(expected, ret)) {
      throw new RuntimeException("Arrays do not match");
    }
    ret = new int[5];
    expected = new int[] { 9, 10, 11, 12, 13 };
    buf.readAlignedIntArray(ret, 8);
    if (!Arrays.equals(expected, ret)) {
      throw new RuntimeException("Arrays do not match");
    }

    boolean caught = false;
    // check invalid access
    try {
      buf.readAlignedIntArray(new int[1], 17); // currently written 16 , so 17 is OOB
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

    // check invalid access, negative position
    try {
      buf.readAlignedIntArray(new int[1], -1);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

    caught = false;
    try { // 3.5 * tmp fits in this buffer
      buf.writeAlignedIntArray(tmp);
      buf.writeAlignedIntArray(tmp);
      buf.writeAlignedIntArray(tmp);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid write");
    }

    caught = false;
    buf.writeAlignedIntArray(new int[] { 1, 2, 3, 4 });
    if (buf.bytesRemaining() != 0) {
      throw new RuntimeException("Buffer should be full and it's reported as not being!");
    }

    try {
      buf.readAlignedIntArray(expected, buf.size() - expected.length + 1); // attempt read 1 more than the buffer has
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

    buf.free();
  }

  public void testReadWriteLongArrayMethod() {
    AlignedAlignedWriteBuffer buf;
    long bufWidth = 224;
    buf = new AlignedAlignedWriteBuffer(199, 32);
    if (buf.bytesRemaining() != buf.size()) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in bytesRemaining().");
    }
    if (buf.getCurrentOffset() != 0) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in getCurrentOffset().");
    }
    long[] tmp = new long[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
      14, 15, 16 };
    buf.writeAlignedLongArray(tmp);
    long bytesRemaining, currentOffset;
    currentOffset = 16 * Stdlib.SIZEOF_JLONG;
    bytesRemaining = bufWidth - currentOffset;
    if (buf.bytesRemaining() != bytesRemaining) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in bytesRemaining().");
    }
    if (buf.getCurrentOffset() != currentOffset) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in getCurrentOffset().");
    }
    long[] ret, expected;
    ret = new long[9];
    expected = new long[] { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    buf.readAlignedLongArray(ret, 0);
    if (!Arrays.equals(expected, ret)) {
      throw new RuntimeException("Arrays do not match");
    }
    ret = new long[5];
    expected = new long[] { 9, 10, 11, 12, 13 };
    buf.readAlignedLongArray(ret, 8);
    if (!Arrays.equals(expected, ret)) {
      throw new RuntimeException("Arrays do not match");
    }

    boolean caught = false;
    // check invalid access
    try {
      buf.readAlignedLongArray(new long[1], 17); // currently written 16, so 17 is OOB
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

    // check invalid access, negative position
    try {
      buf.readAlignedLongArray(new long[1], -1);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

    caught = false;
    try { // 1.75 * tmp fits in this buffer
      buf.writeAlignedLongArray(tmp);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid write");
    }

    caught = false;
    buf.writeAlignedLongArray(new long[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 });
    if (buf.bytesRemaining() != 0) {
      throw new RuntimeException("Buffer should be full and it's reported as not being!");
    }

    try {
      buf.readAlignedLongArray(expected, buf.size() - expected.length + 1); // attempt read 1 more than the buffer has
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

    buf.free();
  }

  public void testReset() {
    AlignedAlignedWriteBuffer buf;
    int bufWidth = 224;
    buf = new AlignedAlignedWriteBuffer(199, 32);
    double[] tmp = new double[] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
      14, 15, 16 };
    buf.writeAlignedDoubleArray(tmp);
    int bytesRemaining, currentOffset;
    currentOffset = 16 * Stdlib.SIZEOF_JDOUBLE;
    bytesRemaining = bufWidth - currentOffset;
    if (buf.bytesRemaining() != bytesRemaining) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in bytesRemaining().");
    }
    if (buf.getCurrentOffset() != currentOffset) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in getCurrentOffset().");
    }
    buf.reset();
    if (buf.bytesRemaining() != bufWidth) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in bytesRemaining().");
    }
    if (buf.getCurrentOffset() != 0) {
      throw new RuntimeException(
          "Buffer is incorrectly reporting in getCurrentOffset().");
    }
    buf.free();
  }

  public void testTwoVarConstructor() {
    boolean caught = false;
    try {
      new AlignedAlignedWriteBuffer(200, 17);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Illegal buffer alignment was not caught.");
    }
    // no need to free, should have failed on construct above
  }

  public void testBadContructor() {
    boolean caught = false;
    try {
      new AlignedAlignedWriteBuffer(-1);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException(
          "Did not catch bad byte value on construct. Size=-1");
    }
    caught = false;
    try {
      new AlignedAlignedWriteBuffer(2, 2);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException(
          "Did not catch bad byte value on construct. Size=2");
    }
    caught = false;
    try {
      new AlignedAlignedWriteBuffer(12, 19);
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException(
          "Did not catch bad byte value on construct. Size=19");
    }
  }

  public void testToMaxSizeConstructor() {
    AlignedAlignedWriteBuffer buf = new AlignedAlignedWriteBuffer(
        AlignedAlignedWriteBuffer.MAX_SIZE);
    buf.free();
  }

  public void getCurrentAddress() {
    AlignedAlignedWriteBuffer buf = new AlignedAlignedWriteBuffer(
        AlignedAlignedWriteBuffer.MAX_SIZE, Stdlib.SIZEOF_JINT);
    long base, o1, o2;
    base = buf.getCurrentAddress();
    buf.writeAlignedDouble(1.);
    o1 = buf.getCurrentAddress();
    if (o1 != base + Stdlib.SIZEOF_JDOUBLE) {
      throw new RuntimeException("Address in buffer not advancing correctly");
    }
    buf.writeAlignedIntArray(new int[] { 4, 5, 6 });
    o2 = buf.getCurrentAddress();
    if (o2 != base + Stdlib.SIZEOF_JDOUBLE + 3 * Stdlib.SIZEOF_JINT) {
      throw new RuntimeException("Address in buffer not advancing correctly");
    }
    buf.free();

    // try a different alignment
    buf = new AlignedAlignedWriteBuffer(AlignedAlignedWriteBuffer.MAX_SIZE,
        Stdlib.SIZEOF_JLONG);
    base = buf.getCurrentAddress();
    buf.writeAlignedDouble(1.);
    o1 = buf.getCurrentAddress();
    if (o1 != base + Stdlib.SIZEOF_JLONG) {
      throw new RuntimeException("Address in buffer not advancing correctly");
    }
    buf.writeAlignedIntArray(new int[] { 4, 5, 6 });
    o2 = buf.getCurrentAddress();
    if (o2 != base + Stdlib.SIZEOF_JLONG + 2 * Stdlib.SIZEOF_JLONG) {
      throw new RuntimeException("Address in buffer not advancing correctly");
    }
    buf.free();
  }

  public void singleIntsReadWrite() {
    AlignedAlignedWriteBuffer buf;
    buf = new AlignedAlignedWriteBuffer(32 * 4, 32);
    buf.writeAlignedInt(7);
    buf.writeAlignedInt(8);
    buf.writeAlignedInt(9);
    int[] expected = new int[] { 7, 8, 9 };
    for (int j = 0; j < 3; j++) {
      if (buf.readAlignedInt(j * 8) != expected[j]) { // 8x as 8 ints fit in an AVX register, and writes were done aligned to their base
        throw new RuntimeException();
      }
    }
    boolean caught = false;

    // check invalid access
    try {
      buf.readAlignedInt(25); // currently written 3 at 32 byte strides, which is equiv of 24 ints
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

    // check invalid access, negative position
    try {
      buf.readAlignedInt(-1);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

    // buffer full
    buf.writeAlignedInt(10);

    if (buf.bytesRemaining() != 0) {
      throw new RuntimeException(
          "Buffer should be full and it's reported as not being!");
    }

    try {
      buf.writeAlignedInt(10);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid write");
    }

    try {
      buf.readAlignedInt(32);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }
  }

  public void singleLongsReadWrite() {
    AlignedAlignedWriteBuffer buf;
    buf = new AlignedAlignedWriteBuffer(32 * 4, 32);
    buf.writeAlignedLong(7);
    buf.writeAlignedLong(8);
    buf.writeAlignedLong(9);
    long[] expected = new long[] { 7, 8, 9 };
    for (int j = 0; j < 3; j++) {
      if (buf.readAlignedLong(j * 4) != expected[j]) { // 4x as 4 long fit in an AVX register, and writes were done aligned to their base
        throw new RuntimeException();
      }
    }
    boolean caught = false;

    // check invalid access
    try {
      buf.readAlignedLong(13); // currently written 3 at 32 byte strides, which is equiv of 12 longs
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid write");
    }

    // check invalid access, negative position
    try {
      buf.readAlignedLong(-1);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

    // buffer full
    buf.writeAlignedLong(10);

    try {
      buf.writeAlignedLong(10);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid write");
    }

    if (buf.bytesRemaining() != 0) {
      throw new RuntimeException(
          "Buffer should be full and it's reported as not being!");
    }

    try {
      buf.readAlignedLong(16);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid write");
    }

  }

  public void singleFloatsReadWrite() {
    AlignedAlignedWriteBuffer buf;
    buf = new AlignedAlignedWriteBuffer(32 * 4, 32); // can hold 32 floats if aligned to 4
    buf.writeAlignedFloat(7);
    buf.writeAlignedFloat(8);
    buf.writeAlignedFloat(9);
    float[] expected = new float[] { 7F, 8F, 9F };
    for (int j = 0; j < 3; j++) {
      if (Math.abs(buf.readAlignedFloat(j * 8) - expected[j]) > Math.ulp(1.0F)) { // 8x as 8 float fit in an AVX register, and writes were done aligned to their base
        throw new RuntimeException();
      }
    }
    boolean caught = false;

    // check invalid access
    try {
      buf.readAlignedFloat(25); // currently written 3 at 32 byte strides, which is equiv of 24 floats
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

    // check invalid access, negative position
    try {
      buf.readAlignedFloat(-1);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

    // buffer full
    buf.writeAlignedFloat(10);

    if (buf.bytesRemaining() != 0) {
      throw new RuntimeException(
          "Buffer should be full and it's reported as not being!");
    }

    try {
      buf.writeAlignedFloat(10);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid write");
    }

    try {
      buf.readAlignedFloat(32);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

  }

  public void singleDoublesReadWrite() {
    AlignedAlignedWriteBuffer buf;
    buf = new AlignedAlignedWriteBuffer(32 * 4, 32); // can hold 16 doubles if aligned to 4
    buf.writeAlignedDouble(7);
    buf.writeAlignedDouble(8);
    buf.writeAlignedDouble(9);
    double[] expected = new double[] { 7., 8., 9. };

    for (int j = 0; j < 3; j++) {
      if (Math.abs(buf.readAlignedDouble(j * 4) - expected[j]) > Math.ulp(1.0)) { // 4x as 4 double fit in an AVX register, and writes were done aligned to their base
        throw new RuntimeException();
      }
    }
    boolean caught = false;

    // check invalid access
    try {
      buf.readAlignedDouble(13); // currently written 3 at 32 byte strides, which is equiv of 12 doubles
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

    // check invalid access, negative position
    try {
      buf.readAlignedDouble(-1);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }

    // buffer full
    buf.writeAlignedDouble(10);

    if (buf.bytesRemaining() != 0) {
      throw new RuntimeException(
          "Buffer should be full and it's reported as not being!");
    }

    try {
      buf.writeAlignedDouble(10);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid write");
    }

    try {
      buf.readAlignedDouble(16);
    } catch (InvalidAccessException e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException("Invalid read");
    }
  }

  public void getAlignment() {
    AlignedAlignedWriteBuffer buf = new AlignedAlignedWriteBuffer(
        AlignedAlignedWriteBuffer.MAX_SIZE);
    if (buf.getAlignment() != buf.getDefaultAlignment()) {
      throw new RuntimeException("Alignment isn't correct");
    }
    buf = new AlignedAlignedWriteBuffer(AlignedAlignedWriteBuffer.MAX_SIZE, 16);
    if (buf.getAlignment() != 16) {
      throw new RuntimeException("Alignment isn't correct");
    }
  }

  public void freeCheck() {
    AlignedAlignedWriteBuffer buf = new AlignedAlignedWriteBuffer(
        AlignedAlignedWriteBuffer.MAX_SIZE);
    buf.free();
    boolean caught = false;
    try {
      buf.free();
    } catch (Exception e) {
      caught = true;
    }
    if (!caught) {
      throw new RuntimeException(
          "Did not throw exception whilst trying to double free");
    }
  }

  public void skipCheck() {
    AlignedAlignedWriteBuffer buf = new AlignedAlignedWriteBuffer(AlignedAlignedWriteBuffer.MAX_SIZE, 32);
    // jump the entire buffer
    if (buf.getCurrentOffset() != 0) {
      throw new RuntimeException("offset is incorrect");
    }
    if (buf.bytesRemaining() != AlignedAlignedWriteBuffer.MAX_SIZE) {
      throw new RuntimeException("bytes remaining is incorrect");
    }

    // rewind to continue test
    buf.reset();

    // jump pow2 bytes
    buf.skip(64);
    if (buf.getCurrentOffset() != 64) {
      throw new RuntimeException("skip max size failed");
    }

    // jump random bytes and check that we land at the next aligned place
    long random_bytes = 37;
    long alignment = buf.getAlignment();
    long aligned_skip = alignment * ((random_bytes / alignment) + 1);
    long expected_new_position = buf.getCurrentOffset() + aligned_skip;
    buf.skip(random_bytes);
    if (buf.getCurrentOffset() != expected_new_position) {
      throw new RuntimeException("skip unaligned amount failed to align on boundary");
    }

    // check we throw on skip huge
    boolean caught = false;
    try {
      buf.skip(2 * AlignedAlignedWriteBuffer.MAX_SIZE);
    } catch (RuntimeException ex) {
      caught = true;
    }

    if (!caught) {
      throw new RuntimeException(
          "Did not throw exception whilst trying to skip too far");
    }

  }

  public void cleanCheck() {
    AlignedAlignedWriteBuffer buf = new AlignedAlignedWriteBuffer(32, 32);
    int[] expectedRange = { 0, 1, 2, 3, 4, 5, 6, 7 };
    buf.writeAlignedIntArray(expectedRange);
    int[] computed = new int[8];
    assertTrue(buf.bytesRemaining() == 0);
    buf.readAlignedIntArray(computed, 0);
    assertTrue(Arrays.equals(computed, expectedRange));
    buf.clean();
    int[] zeros = new int[8];
    buf.readAlignedIntArray(computed, 0);
    assertTrue(Arrays.equals(computed, zeros));
    buf.free();
  }

}
