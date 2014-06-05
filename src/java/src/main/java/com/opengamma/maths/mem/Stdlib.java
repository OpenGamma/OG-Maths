/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */

package com.opengamma.maths.mem;

import java.lang.reflect.Field;

import sun.misc.Unsafe; //CSIGNORE illegal package but needed

import com.opengamma.maths.nativeloader.NativeLibraries;

/**
 * Stdlib provides methods to allocate, free, read and write to a 64 byte aligned memory block.
 * It does this through the Unsafe class, JNI calls and pointer manipulation. The onus is on the
 * programmer to not blow their feet off. As suggested by the class name "Unsafe", it is unsafe.
 * The JNI calls are used to directly call memory management functions in e.g. stdlib.h, again, 
 * this is not very safe. Proceed at your own risk, dragons be 'ere.
 */
@SuppressWarnings("restriction")
public class Stdlib {

  // 64 bit address size
  private static final int s_addressSize = 8;

  // Load native components
  static {
    NativeLibraries.initialize();
  }

  // attempt to get the Unsafe class
  private static final Unsafe UNSAFE_METHODS;
  static {
    try {
      Field unsafeField = Unsafe.class.getDeclaredField("theUnsafe");
      unsafeField.setAccessible(true);
      UNSAFE_METHODS = (Unsafe) unsafeField.get(null);
    } catch (Exception e) {
      throw new RuntimeException("Unable to hook into unsafe", e);
    }

    int pokedAddressSize = UNSAFE_METHODS.addressSize();
    if (pokedAddressSize != s_addressSize) {
      throw new RuntimeException("Address size of machine is reported as " + pokedAddressSize + " this library will only work with 8 byte wide addresses.");
    }
  }

  // native bindings
  private static native long wrapped_allocateAlignedMemory(long amount); //CSIGNORE

  private static native int wrapped_freeAlignedMemory(long addr); //CSIGNORE

  private static native int wrapped_checkPointerSize(); //CSIGNORE

  private static native long wrapped_incrementAddress(long addr); //CSIGNORE

  // class instance
  private static Stdlib s_instance = new Stdlib();

  // fix for old OpenJDKs not having Unsafe.ARRAY_<TYPE>_INDEX_SCALE and UNSAFE_METHODS.ARRAY_<TYPE>_BASE_OFFSET
  /**
   * Array base offset for an array of bools
   */
  public static final int ARRAY_BOOLEAN_BASE_OFFSET = UNSAFE_METHODS.arrayBaseOffset(boolean[].class);
  /**
   * Array base offset for an array of bytes
   */
  public static final int ARRAY_BYTE_BASE_OFFSET = UNSAFE_METHODS.arrayBaseOffset(byte[].class);
  /**
   * Array base offset for an array of shorts
   */
  public static final int ARRAY_SHORT_BASE_OFFSET = UNSAFE_METHODS.arrayBaseOffset(short[].class);
  /**
   * Array base offset for an array of chars
   */
  public static final int ARRAY_CHAR_BASE_OFFSET = UNSAFE_METHODS.arrayBaseOffset(char[].class);
  /**
   * Array base offset for an array of ints
   */
  public static final int ARRAY_INT_BASE_OFFSET = UNSAFE_METHODS.arrayBaseOffset(int[].class);
  /**
   * Array base offset for an array of longs
   */
  public static final int ARRAY_LONG_BASE_OFFSET = UNSAFE_METHODS.arrayBaseOffset(long[].class);
  /**
   * Array base offset for an array of floats
   */
  public static final int ARRAY_FLOAT_BASE_OFFSET = UNSAFE_METHODS.arrayBaseOffset(float[].class);
  /**
   * Array base offset for an array of doubles
   */
  public static final int ARRAY_DOUBLE_BASE_OFFSET = UNSAFE_METHODS.arrayBaseOffset(double[].class);
  /**
   * Array base offset for an array of objects
   */
  public static final int ARRAY_OBJECT_BASE_OFFSET = UNSAFE_METHODS.arrayBaseOffset(Object[].class);
  /**
   * Array index scale for an array of bools
   */
  public static final int ARRAY_BOOLEAN_INDEX_SCALE = UNSAFE_METHODS.arrayIndexScale(boolean[].class);
  /**
   * Array index scale for an array of bytes
   */  
  public static final int ARRAY_BYTE_INDEX_SCALE = UNSAFE_METHODS.arrayIndexScale(byte[].class);
  /**
   * Array index scale for an array of shorts
   */  
  public static final int ARRAY_SHORT_INDEX_SCALE = UNSAFE_METHODS.arrayIndexScale(short[].class);
  /**
   * Array index scale for an array of chars
   */  
  public static final int ARRAY_CHAR_INDEX_SCALE = UNSAFE_METHODS.arrayIndexScale(char[].class);
  /**
   * Array index scale for an array of ints
   */  
  public static final int ARRAY_INT_INDEX_SCALE = UNSAFE_METHODS.arrayIndexScale(int[].class);
  /**
   * Array index scale for an array of longs
   */  
  public static final int ARRAY_LONG_INDEX_SCALE = UNSAFE_METHODS.arrayIndexScale(long[].class);
  /**
   * Array index scale for an array of floats
   */  
  public static final int ARRAY_FLOAT_INDEX_SCALE = UNSAFE_METHODS.arrayIndexScale(float[].class);
  /**
   * Array index scale for an array of doubles
   */  
  public static final int ARRAY_DOUBLE_INDEX_SCALE = UNSAFE_METHODS.arrayIndexScale(double[].class);
  /**
   * Array index scale for an array of objects
   */  
  public static final int ARRAY_OBJECT_INDEX_SCALE = UNSAFE_METHODS.arrayIndexScale(Object[].class);

  // The size of primitives
  /**
   * The size in bytes of a jint on a 64 bit platform running a 64 bit JVM
   */
  public static int SIZEOF_JINT = 4; //CSIGNORE
  /**
   * The size in bytes of a jlong on a 64 bit platform running a 64 bit JVM
   */  
  public static int SIZEOF_JLONG = 8; //CSIGNORE
  /**
   * The size in bytes of a jfloat on a 64 bit platform running a 64 bit JVM
   */  
  public static int SIZEOF_JFLOAT = 4; //CSIGNORE
  /**
   * The size in bytes of a jdouble on a 64 bit platform running a 64 bit JVM
   */  
  public static int SIZEOF_JDOUBLE = 8; //CSIGNORE

  /**
   * Gets the instance of Unsafe.
   * @return the Unsafe
   */
  public static final Unsafe getUnsafe() {
    return UNSAFE_METHODS;
  }

  /**
   * Gets a 64 byte aligned block of memory of size "amount" from posix_memalign.
   * Whilst large blocks can be requested, it may not be wise as the code has to 
   * search around looking for blocks in address space that won't overflow a long long.
   * 
   * @param amount the quantity of memory required in bytes
   * @return the address of the aligned memory
   */
  public static long allocateAlignedMemory(final long amount) {
    if (amount < 1) {
      throw new RuntimeException("Cannot perform allocation request for quantities less than 1 byte.");
    }
    // if long overflows above condition catches, pretty unlikely!
    return wrapped_allocateAlignedMemory(amount);
  }

  /**
   * free()'s the memory at a given address. There is no safety!
   * @param address the address to free
   */
  public static void freeAlignedMemory(final long address) {
    wrapped_freeAlignedMemory(address);
  }

  /**
   * Does sizeof(uintptr_t)
   * @return the size of a native pointer
   */
  public static int checkNativePointerSize() {
    return wrapped_checkPointerSize();
  }

  /**
   * Increments the address assuming it's a uintptr_t
   * @param addr the address to increment
   * @return the incremented address
   */
  public static long incrementAddress(long addr) {
    return wrapped_incrementAddress(addr);
  }

  /**
   * Write an int to a specified address
   * @param src the int to write 
   * @param destination the destination to which the int shall be written
   */
  public static void writeIntToAddress(final int src, final long destination) {
    UNSAFE_METHODS.putInt(destination, src);
  }

  /**
   * Write a long to a specified address
   * @param src the long to write 
   * @param destination the destination to which the long shall be written
   */
  public static void writeLongToAddress(final long src, final long destination) {
    UNSAFE_METHODS.putLong(destination, src);
  }

  /**
   * Write a float to a specified address
   * @param src the float to write 
   * @param destination the destination to which the float shall be written
   */
  public static void writeFloatToAddress(final float src, final long destination) {
    UNSAFE_METHODS.putFloat(destination, src);
  }

  /**
   * Write a double to a specified address
   * @param src the double to write 
   * @param destination the destination to which the double shall be written
   */
  public static void writeDoubleToAddress(final double src, final long destination) {
    UNSAFE_METHODS.putDouble(destination, src);
  }

  /**
   * Read an int from a specified address
   * @param addr the address from which the int shall be read
   * @return the read int
   */
  public static int readIntFromAddress(final long addr) {
    return UNSAFE_METHODS.getInt(addr);
  }

  /**
   * Read a long from a specified address
   * @param addr the address from which the long shall be read 
   * @return the read long
   */
  public static long readLongFromAddress(final long addr) {
    return UNSAFE_METHODS.getLong(addr);
  }

  /**
   * Read a float from a specified address
   * @param addr the address from which the float shall be read
   * @return the read float
   */
  public static float readFloatFromAddress(final long addr) {
    return UNSAFE_METHODS.getFloat(addr);
  }

  /**
   * Read a double from a specified address
   * @param addr the address from which the double shall be read
   * @return the read double
   */
  public static double readDoubleFromAddress(final long addr) {
    return UNSAFE_METHODS.getDouble(addr);
  }

  /**
   * Write an int array to memory
   * @param src the source array
   * @param offsetIntoSrc the offset into the source array from which to start using data
   * @param count the amount of data from the src array to write
   * @param destination the address of the destination location
   */
  public static void writeIntArrayToAddress(final int[] src, final int offsetIntoSrc, final int count, final long destination) {
    int coff = (offsetIntoSrc + count);
    if (coff > src.length) {
      throw new RuntimeException("Requested offset+count out of bounds of src");
    }
    long size = count * ARRAY_INT_INDEX_SCALE;
    long jmp = ARRAY_INT_BASE_OFFSET + offsetIntoSrc * ARRAY_INT_INDEX_SCALE;
    UNSAFE_METHODS.copyMemory(src, jmp, null, destination, size);
  }

  /**
   * Write a long array to memory
   * @param src the source array
   * @param offsetIntoSrc the offset into the source array from which to start using data
   * @param count the amount of data from the src array to write
   * @param destination the address of the destination location
   */
  public static void writeLongArrayToAddress(final long[] src, final int offsetIntoSrc, final int count, final long destination) {
    int coff = (offsetIntoSrc + count);
    if (coff > src.length) {
      throw new RuntimeException("Requested offset+count out of bounds of src");
    }
    long size = count * ARRAY_LONG_INDEX_SCALE;
    long jmp = ARRAY_LONG_BASE_OFFSET + offsetIntoSrc * ARRAY_LONG_INDEX_SCALE;
    UNSAFE_METHODS.copyMemory(src, jmp, null, destination, size);
  }

  /**
   * Write an float array to memory
   * @param src the source array
   * @param offsetIntoSrc the offset into the source array from which to start using data
   * @param count the amount of data from the src array to write
   * @param destination the address of the destination location
   */
  public static void writeFloatArrayToAddress(final float[] src, final int offsetIntoSrc, final int count, final long destination) {
    int coff = (offsetIntoSrc + count);
    if (coff > src.length) {
      throw new RuntimeException("Requested offset+count out of bounds of src");
    }
    long size = count * ARRAY_FLOAT_INDEX_SCALE;
    long jmp = ARRAY_FLOAT_BASE_OFFSET + offsetIntoSrc * ARRAY_FLOAT_INDEX_SCALE;
    UNSAFE_METHODS.copyMemory(src, jmp, null, destination, size);
  }

  /**
   * Write an double array to memory
   * @param src the source array
   * @param offsetIntoSrc the offset into the source array from which to start using data
   * @param count the amount of data from the src array to write
   * @param destination the address of the destination location
   */
  public static void writeDoubleArrayToAddress(final double[] src, final int offsetIntoSrc, final int count, final long destination) {
    int coff = (offsetIntoSrc + count);
    if (coff > src.length) {
      throw new RuntimeException("Requested offset+count out of bounds of src");
    }
    long size = count * ARRAY_DOUBLE_INDEX_SCALE;
    long jmp = ARRAY_DOUBLE_BASE_OFFSET + offsetIntoSrc * ARRAY_DOUBLE_INDEX_SCALE;
    UNSAFE_METHODS.copyMemory(src, jmp, null, destination, size);
  }

  /**
   * Read memory to an int array
   * @param destination the int array to read memory to
   * @param offsetIntoDest offset into the destination array
   * @param srcAddr the address at which to start reading
   * @param count the amount of data
   */
  public static void readAddressToIntArray(int[] destination, int offsetIntoDest, long srcAddr, final int count) {
    int coff = (offsetIntoDest + count);
    if (coff > destination.length) {
      throw new RuntimeException("Requested offset+count out of bounds of destination");
    }
    long size = count * ARRAY_INT_INDEX_SCALE;
    long jmp = ARRAY_INT_BASE_OFFSET + offsetIntoDest * ARRAY_INT_INDEX_SCALE;
    UNSAFE_METHODS.copyMemory(null, srcAddr, destination, jmp, size);
  }

  /**
   * Read memory to a long array
   * @param destination the double array to read memory to
   * @param offsetIntoDest offset into the destination array
   * @param srcAddr the address at which to start reading
   * @param count the amount of data
   */
  public static void readAddressToLongArray(long[] destination, int offsetIntoDest, long srcAddr, final int count) {
    int coff = (offsetIntoDest + count);
    if (coff > destination.length) {
      throw new RuntimeException("Requested offset+count out of bounds of destination");
    }
    long size = count * ARRAY_LONG_INDEX_SCALE;
    long jmp = ARRAY_LONG_BASE_OFFSET + offsetIntoDest * ARRAY_LONG_INDEX_SCALE;
    UNSAFE_METHODS.copyMemory(null, srcAddr, destination, jmp, size);
  }

  /**
   * Read memory to a float array
   * @param destination the float array to read memory to
   * @param offsetIntoDest offset into the destination array
   * @param srcAddr the address at which to start reading
   * @param count the amount of data
   */
  public static void readAddressToFloatArray(float[] destination, int offsetIntoDest, long srcAddr, final int count) {
    int coff = (offsetIntoDest + count);
    if (coff > destination.length) {
      throw new RuntimeException("Requested offset+count out of bounds of destination");
    }
    long size = count * ARRAY_FLOAT_INDEX_SCALE;
    long jmp = ARRAY_FLOAT_BASE_OFFSET + offsetIntoDest * ARRAY_FLOAT_INDEX_SCALE;
    UNSAFE_METHODS.copyMemory(null, srcAddr, destination, jmp, size);
  }

  /**
   * Read memory to a double array
   * @param destination the double array to read memory to
   * @param offsetIntoDest offset into the destination array
   * @param srcAddr the address at which to start reading
   * @param count the amount of data
   */
  public static void readAddressToDoubleArray(double[] destination, int offsetIntoDest, long srcAddr, final int count) {
    int coff = (offsetIntoDest + count);
    if (coff > destination.length) {
      throw new RuntimeException("Requested offset+count out of bounds of destination");
    }
    long size = count * ARRAY_DOUBLE_INDEX_SCALE;
    long jmp = ARRAY_DOUBLE_BASE_OFFSET + offsetIntoDest * ARRAY_DOUBLE_INDEX_SCALE;
    UNSAFE_METHODS.copyMemory(null, srcAddr, destination, jmp, size);
  }

  /**
   * Set memory to a specified byte value
   * @param addr the base address for the memset 
   * @param offset the offset from the base address at which the memset will take place
   * @param count the number of bytes to set to value
   * @param value the value to which the memory shall be set
   */
  public static void memset(long addr, long offset, long count, byte value) {
    UNSAFE_METHODS.setMemory(addr + offset, count, value);
  }

}
