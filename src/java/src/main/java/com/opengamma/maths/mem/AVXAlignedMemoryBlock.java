/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */

package com.opengamma.maths.mem;

/**
 * Holds memory block aligned to 64 bytes and provides methods to read and write from said block.
 * This class is a touch dangerous as it relies on native memory access provided by Stdlib.java,
 * however, thankfully, there be less dragons here.
 */
public class AVXAlignedMemoryBlock {

  private long _blockStartAddress;
  private long _blockEndAddress;
  private boolean _freed = true;

  // checks that any access is occurring at a valid location in the block
  // offsetIntoBlock is a byte offset into the block at which the access is requested
  // count is the number of accesses of a given type requested
  // sizeof is the number that would be returned from C's sizeof()
  private void checkAccessIsValid(long offsetIntoBlock, int count, int sizeof) {
    if (offsetIntoBlock < 0) {
      throw new RuntimeException("Cannot have negative offset into block.");
    }
    if (_blockStartAddress + offsetIntoBlock + (long) (count) * (long) sizeof > _blockEndAddress)
    {
      throw new RuntimeException("Access is invalid, count of offset plus data size out of block bounds\n"
          + "Block start address: " + _blockStartAddress + "\n"
          + "Block end address: " + _blockEndAddress + "\n"
          + "Offset: " + offsetIntoBlock + "\n"
          + "Count: " + count + "\n"
          + "Data size: " + sizeof + "\n"
          + "Total data width: " + sizeof * count + "\n");
    }
  }

  /**
   * Create a new memory block of size bytes
   * @param bytes the size in bytes of the memory block to allocate
   */
  public AVXAlignedMemoryBlock(long bytes) {
    if (bytes < 1) {
      throw new RuntimeException("Cannot allocate a negative amount of memory");
    }
    allocate(bytes);
  }

  /**
   * Allocate memory in an off jheap location
   * @param bytes the amount of memory to allocate in bytes
   */
  public void allocate(long bytes) {
    if (bytes < 1) {
      throw new RuntimeException("Cannot allocate a negative amount of memory");
    }
    if (!_freed) {
      throw new RuntimeException("Cannot allocate new memory block without freeing the current reference.");
    }
    _blockStartAddress = Stdlib.allocateAlignedMemory(bytes);
    _blockEndAddress = _blockStartAddress + bytes;
    _freed = false;
  }

  /**
   * Free the off jheap memory associated with the memory block.
   */
  public void free() {
    if (!_freed) {
      Stdlib.freeAlignedMemory(_blockStartAddress);
      _freed = true;
    } else {
      throw new RuntimeException("Cannot free() already free'd allocation.");
    }
  }

  /**
   * Reports whether the backing memory allocation has been free'd
   * @return true if the allocation has been free'd, false otherwise
   */
  public boolean getFreedStatus() {
    return _freed;
  }

  /**
   * Get the block base address i.e. the address of the start of the block
   * @return the base address of the block
   */
  public long getBaseAddress() {
    return _blockStartAddress;
  }

  /**
   * Set memory to a specified byte value
   * @param offset the offset from the base address (in bytes) at which the memset will take place
   * @param count the number of bytes to set to value
   * @param value the value to which the memory shall be set
   */
  public void memset(long offset, int count, byte value) {
    checkAccessIsValid(offset, count, 1);
    Stdlib.memset(_blockStartAddress, offset, count, value);
  }

  /**
   * Write an int at a given byte offset
   * @param val the value to write
   * @param offsetIntoBlock the byte offset into the block from which to write the value
   */
  public void writeInt(int val, long offsetIntoBlock) {
    checkAccessIsValid(offsetIntoBlock, 1, Stdlib.SIZEOF_JINT);
    Stdlib.writeIntToAddress(val, _blockStartAddress + offsetIntoBlock);
  }

  /**
   * Write a long at a given byte offset
   * @param val the value to write
   * @param offsetIntoBlock the byte offset into the block from which to write the value
   */
  public void writeLong(long val, long offsetIntoBlock) {
    checkAccessIsValid(offsetIntoBlock, 1, Stdlib.SIZEOF_JLONG);
    Stdlib.writeLongToAddress(val, _blockStartAddress + offsetIntoBlock);
  }

  /**
   * Write a float at a given byte offset
   * @param val the value to write
   * @param offsetIntoBlock the byte offset into the block from which to write the value
   */
  public void writeFloat(float val, long offsetIntoBlock) {
    checkAccessIsValid(offsetIntoBlock, 1, Stdlib.SIZEOF_JFLOAT);
    Stdlib.writeFloatToAddress(val, _blockStartAddress + offsetIntoBlock);
  }

  /**
   * Write a double at a given byte offset
   * @param val the value to write
   * @param offsetIntoBlock the byte offset into the block from which to write the value
   */
  public void writeDouble(double val, long offsetIntoBlock) {
    checkAccessIsValid(offsetIntoBlock, 1, Stdlib.SIZEOF_JDOUBLE);
    Stdlib.writeDoubleToAddress(val, _blockStartAddress + offsetIntoBlock);
  }

  /**
   * Read an int from a given byte offset
   * @param offsetIntoBlock the byte offset into the block from which to read the value
   * @return the read int
   */
  public int readInt(long offsetIntoBlock) {
    checkAccessIsValid(offsetIntoBlock, 1, Stdlib.SIZEOF_JINT);
    return Stdlib.readIntFromAddress(_blockStartAddress + offsetIntoBlock);
  }

  /**
   * Read a long from a given byte offset
   * @param offsetIntoBlock the byte offset into the block from which to read the value
   * @return the read long
   */
  public long readLong(long offsetIntoBlock) {
    checkAccessIsValid(offsetIntoBlock, 1, Stdlib.SIZEOF_JLONG);
    return Stdlib.readLongFromAddress(_blockStartAddress + offsetIntoBlock);
  }

  /**
   * Read a float from a given byte offset
   * @param offsetIntoBlock the byte offset into the block from which to read the value
   * @return the read float
   */
  public float readFloat(long offsetIntoBlock) {
    checkAccessIsValid(offsetIntoBlock, 1, Stdlib.SIZEOF_JFLOAT);
    return Stdlib.readFloatFromAddress(_blockStartAddress + offsetIntoBlock);
  }

  /**
   * Read a double from a given byte offset
   * @param offsetIntoBlock the byte offset into the block from which to read the value
   * @return the read double
   */
  public double readDouble(long offsetIntoBlock) {
    checkAccessIsValid(offsetIntoBlock, 1, Stdlib.SIZEOF_JDOUBLE);
    return Stdlib.readDoubleFromAddress(_blockStartAddress + offsetIntoBlock);
  }

  /**
   * Write an int array to an arbitrary memory offset.
   * @param array the array containing the source data
   * @param offsetIntoSrc the offset into the source from which the start of the source data should be taken
   * @param offsetIntoBlock the offset at which the write should take place from the start of the memory block (in BYTES!)
   * @param count the number of elements of the source data to write to the block
   */
  public void writeIntArray(int[] array, int offsetIntoSrc, long offsetIntoBlock, int count) {
    checkAccessIsValid(offsetIntoBlock, count, Stdlib.SIZEOF_JINT);
    Stdlib.writeIntArrayToAddress(array, offsetIntoSrc, count, _blockStartAddress + offsetIntoBlock);
  }

  /**
   * Write a long array to an arbitrary memory offset.
   * @param array the array containing the source data
   * @param offsetIntoSrc the offset into the source from which the start of the source data should be taken
   * @param offsetIntoBlock the offset at which the write should take place from the start of the memory block (in BYTES!)
   * @param count the number of elements of the source data to write to the block
   */
  public void writeLongArray(long[] array, int offsetIntoSrc, long offsetIntoBlock, int count) {
    checkAccessIsValid(offsetIntoBlock, count, Stdlib.SIZEOF_JLONG);
    Stdlib.writeLongArrayToAddress(array, offsetIntoSrc, count, _blockStartAddress + offsetIntoBlock);
  }

  /**
   * Write a float array to an arbitrary memory offset.
   * @param array the array containing the source data
   * @param offsetIntoSrc the offset into the source from which the start of the source data should be taken
   * @param offsetIntoBlock the offset at which the write should take place from the start of the memory block (in BYTES!)
   * @param count the number of elements of the source data to write to the block
   */
  public void writeFloatArray(float[] array, int offsetIntoSrc, long offsetIntoBlock, int count) {
    checkAccessIsValid(offsetIntoBlock, count, Stdlib.SIZEOF_JFLOAT);
    Stdlib.writeFloatArrayToAddress(array, offsetIntoSrc, count, _blockStartAddress + offsetIntoBlock);
  }

  /**
   * Write a double array to an arbitrary memory offset.
   * @param array the array containing the source data
   * @param offsetIntoSrc the offset into the source from which the start of the source data should be taken
   * @param offsetIntoBlock the offset at which the write should take place from the start of the memory block (in BYTES!)
   * @param count the number of elements of the source data to write to the block
   */
  public void writeDoubleArray(double[] array, int offsetIntoSrc, long offsetIntoBlock, int count) {
    checkAccessIsValid(offsetIntoBlock, count, Stdlib.SIZEOF_JDOUBLE);
    Stdlib.writeDoubleArrayToAddress(array, offsetIntoSrc, count, _blockStartAddress + offsetIntoBlock);
  }

  /**
   * Read an int array from an arbitrary memory offset.
   * @param destination the array in which to place the read data
   * @param offsetIntoDest the offset into the destination at which the read data should start to be placed 
   * @param byteOffset the offset at which the read should take place from the start of the memory block (in BYTES!)
   * @param count the number of elements of the source data to read to the array
   */
  public void readIntArray(int[] destination, int offsetIntoDest, long byteOffset, final int count) {
    checkAccessIsValid(byteOffset, count, Stdlib.SIZEOF_JINT);
    Stdlib.readAddressToIntArray(destination, offsetIntoDest, _blockStartAddress + byteOffset, count);
  }

  /**
   * Read a long array from an arbitrary memory offset.
   * @param destination the array in which to place the read data
   * @param offsetIntoDest the offset into the destination at which the read data should start to be placed 
   * @param byteOffset the offset at which the read should take place from the start of the memory block (in BYTES!)
   * @param count the number of elements of the source data to read to the array
   */
  public void readLongArray(long[] destination, int offsetIntoDest, long byteOffset, final int count) {
    checkAccessIsValid(byteOffset, count, Stdlib.SIZEOF_JLONG);
    Stdlib.readAddressToLongArray(destination, offsetIntoDest, _blockStartAddress + byteOffset, count);
  }

  /**
   * Read a float array from an arbitrary memory offset.
   * @param destination the array in which to place the read data
   * @param offsetIntoDest the offset into the destination at which the read data should start to be placed 
   * @param byteOffset the offset at which the read should take place from the start of the memory block (in BYTES!)
   * @param count the number of elements of the source data to read to the array
   */
  public void readFloatArray(float[] destination, int offsetIntoDest, long byteOffset, final int count) {
    checkAccessIsValid(byteOffset, count, Stdlib.SIZEOF_JFLOAT);
    Stdlib.readAddressToFloatArray(destination, offsetIntoDest, _blockStartAddress + byteOffset, count);
  }

  /**
   * Read a double array from an arbitrary memory offset.
   * @param destination the array in which to place the read data
   * @param offsetIntoDest the offset into the destination at which the read data should start to be placed 
   * @param byteOffset the offset at which the read should take place from the start of the memory block (in BYTES!)
   * @param count the number of elements of the source data to read to the array
   */
  public void readDoubleArray(double[] destination, int offsetIntoDest, long byteOffset, final int count) {
    checkAccessIsValid(byteOffset, count, Stdlib.SIZEOF_JDOUBLE);
    Stdlib.readAddressToDoubleArray(destination, offsetIntoDest, _blockStartAddress + byteOffset, count);
  }

}
