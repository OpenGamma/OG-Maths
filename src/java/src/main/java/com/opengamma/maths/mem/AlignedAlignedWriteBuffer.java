/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 * 
 * Please see distribution for license.
 */
package com.opengamma.maths.mem;

/**
 * Use as a buffer to perform aligned read and writes. 
 * Writes are guaranteed to start at an address which is a multiple of "alignment".
 * The base address is AVX aligned to 64 bytes.
 */
public class AlignedAlignedWriteBuffer {

  /**
   * The maximum size this buffer can be in bytes
   */
  public static final long MAX_SIZE = 1073741824; // 1Gb
  private AVXAlignedMemoryBlock _block;
  private long _baseAddress;
  private long _buffersize;
  private long _currentOffset;
  private boolean _freed = true;
  private int _stride;
  private static final int s_defaultstride = 64; // required memory stride, 64 bytes = 512 bits for AVX

  /**
   * Create a buffer of at least "size" (size will be rounded to the nearest 64 byte boundary)
   * @param size the minimum buffer size
   */
  public AlignedAlignedWriteBuffer(long size) {
    this(size, s_defaultstride);
  }

  /**
   * Create a buffer of at least "size" (size will be rounded to the nearest \a alignment boundary)
   * @param size the minimum buffer size
   * @param alignment the address multiple to which the data should be aligned (must be a power of 2)
   */
  public AlignedAlignedWriteBuffer(long size, int alignment) {
    if (!isPow2(alignment) || alignment < 4) {
      throw new RuntimeException("Alignment provided is not a power of 2 greater than or equal to 4.");
    }

    if (size < 0) {
      throw new RuntimeException("Negative buffer size makes no sense.");
    }

    if (alignment > size) {
      // We cannot have size less than alignment, so meet ours and the caller's requirements
      // by setting the size to equal the aligment.
      size = alignment;
    }

    if ((size % alignment) != 0) {
      // Round up size to next alignment boundary
      size = alignment * ((size / alignment) + 1);
    }
    
    if (size > AlignedAlignedWriteBuffer.MAX_SIZE) {
      throw new RuntimeException("Max buffer size of " + AlignedAlignedWriteBuffer.MAX_SIZE + " is exceeded with " +
          size);
    }

    _buffersize = size;
    _stride = alignment;
    _block = new AVXAlignedMemoryBlock(_buffersize);
    _baseAddress = _block.getBaseAddress();
    _freed = false;
  }

  /**
   * Gets the current address of the write buffer tape head.
   * @return the current address
   */
  public final long getCurrentAddress() {
    return _baseAddress + _currentOffset;
  }

  /**
   * Gets the default alignment.
   * @return the default alignment
   */
  public final int getDefaultAlignment() {
    return s_defaultstride;
  }

  /**
   * Gets the alignment (stride size) of the buffer.
   * @return the alignment (stride size)
   */
  public final int getAlignment() {
    return _stride;
  }

  /**
   * Gets the base address of the buffer, i.e. the position from which the offsets are measured 
   * @return the base address
   */
  public final long getBaseAddress() {
    return _block.getBaseAddress();
  }

  /**
   * Resets the the buffer offset to zero. i.e. tape rewind.
   */
  public final void reset() {
    _currentOffset = 0;
  }

  /**
   * Sets the block to 0x0.
   */
  public final void clean() {
    Stdlib.memset(_baseAddress, 0L, _buffersize, (byte) 0x0);
  }

  /**
   * Skip bytes much memory. If the skip does not land on an alignment boundary, the number
   * of bytes to skip will be rounded up to the next alignment boundary.
   * @param nbytes the minimum amount to skip.
   */
  public void skip(long nbytes) {
    if ((nbytes % _stride) != 0) {
      nbytes = _stride * ((nbytes / _stride) + 1);
    }
    
    if (_currentOffset + nbytes > _buffersize) {
      throw new RuntimeException("Memory skip exceeds size of buffer");
    }

    _currentOffset += nbytes;
  }

  /**
   * Free's the block.
   */
  public final void free() {
    if (!_freed) {
      _block.free();
      _freed = true;
    } else {
      throw new RuntimeException("Trying to free already freed memory.");
    }
  }

  /**
   * Returns the size of the buffer
   * @return the size in bytes of the buffer
   */
  public final long size() {
    return _buffersize;
  }

  /**
   * Returns the position of the current offset for writing to the buffer. 
   * @return the current offset
   */
  public final long getCurrentOffset() {
    return _currentOffset;
  }

  /**
   * Returns the number of bytes remaining in the buffer.
   * @return the number of bytes remaining in the buffer
   */
  public final long bytesRemaining() {
    return _buffersize - _currentOffset;
  }

  /**
   * Reads an int from a specified position, be aware that the writes were aligned!
   * @param position the location from which to read assuming the data is an array of ints 
   * @return the int at location {@code position}
   */
  public final int readAlignedInt(long position) {
    if (position < 0) {
      throw new InvalidAccessException("Position must be positive");
    }
    long byteOffset = position * Stdlib.SIZEOF_JINT;
    if (byteOffset + Stdlib.SIZEOF_JINT > _buffersize) {
      throw new InvalidAccessException("Read exceeds buffer size.");
    }
    if (byteOffset + Stdlib.SIZEOF_JINT > _currentOffset) {
      throw new InvalidAccessException("Read exceeds current position.");
    }
    return _block.readInt(byteOffset);
  }

  /**
   * Reads a float from a specified position, be aware that the writes were aligned!
   * @param position the location from which to read assuming the data is an array of floats 
   * @return the float at location {@code position}
   */
  public final float readAlignedFloat(long position) {
    if (position < 0) {
      throw new InvalidAccessException("Position must be positive");
    }
    long byteOffset = position * Stdlib.SIZEOF_JFLOAT;
    if (byteOffset + Stdlib.SIZEOF_JFLOAT > _buffersize) {
      throw new InvalidAccessException("Read exceeds buffer size.");
    }
    if (byteOffset + Stdlib.SIZEOF_JFLOAT > _currentOffset) {
      throw new InvalidAccessException("Read exceeds current position.");
    }
    return _block.readFloat(byteOffset);
  }

  /**
   * Reads a long from a specified position, be aware that the writes were aligned!
   * @param position the location from which to read assuming the data is an array of longs 
   * @return the long at location {@code position}
   */
  public final long readAlignedLong(long position) {
    if (position < 0) {
      throw new InvalidAccessException("Position must be positive");
    }
    long byteOffset = position * Stdlib.SIZEOF_JLONG;
    if (byteOffset + Stdlib.SIZEOF_JLONG > _buffersize) {
      throw new InvalidAccessException("Read exceeds buffer size.");
    }
    if (byteOffset + Stdlib.SIZEOF_JLONG > _currentOffset) {
      throw new InvalidAccessException("Read exceeds current position.");
    }
    return _block.readLong(byteOffset);
  }

  /**
   * Reads a double from a specified position, be aware that the writes were aligned!
   * @param position the location from which to read assuming the data is an array of doubles 
   * @return the double at location {@code position}
   */
  public final double readAlignedDouble(long position) {
    if (position < 0) {
      throw new InvalidAccessException("Position must be positive");
    }
    long byteOffset = position * Stdlib.SIZEOF_JDOUBLE;
    if (byteOffset + Stdlib.SIZEOF_JDOUBLE > _buffersize) {
      throw new InvalidAccessException("Read exceeds buffer size.");
    }
    if (byteOffset + Stdlib.SIZEOF_JDOUBLE > _currentOffset) {
      throw new InvalidAccessException("Read exceeds current position.");
    }
    return _block.readDouble(byteOffset);
  }

  /**
   * Writes an in to the next available address corresponding to the specified alignment. 
   * @param val the value to write
   */
  public final void writeAlignedInt(int val) {
    int writelen = Stdlib.SIZEOF_JINT;
    if (_currentOffset + Stdlib.SIZEOF_JINT > _buffersize) {
      throw new InvalidAccessException("Write exceeds buffer size.");
    }
    _block.writeInt(val, _currentOffset);
    _currentOffset += advanceOffset(writelen);
  }

  /**
   * Writes a long to the next available address corresponding to the specified alignment. 
   * @param val the value to write
   */
  public final void writeAlignedLong(long val) {
    int writelen = Stdlib.SIZEOF_JLONG;
    if (_currentOffset + Stdlib.SIZEOF_JLONG > _buffersize) {
      throw new InvalidAccessException("Write exceeds buffer size.");
    }
    _block.writeLong(val, _currentOffset);
    _currentOffset += advanceOffset(writelen);
  }

  /**
   * Writes a float to the next available address corresponding to the specified alignment. 
   * @param val the value to write
   */
  public final void writeAlignedFloat(float val) {
    int writelen = Stdlib.SIZEOF_JFLOAT;
    if (_currentOffset + Stdlib.SIZEOF_JFLOAT > _buffersize) {
      throw new InvalidAccessException("Write exceeds buffer size.");
    }
    _block.writeFloat(val, _currentOffset);
    _currentOffset += advanceOffset(writelen);
  }

  /**
   * Writes a double to the next available address corresponding to the specified alignment. 
   * @param val the value to write
   */
  public final void writeAlignedDouble(double val) {
    int writelen = Stdlib.SIZEOF_JDOUBLE;
    if (_currentOffset + Stdlib.SIZEOF_JDOUBLE > _buffersize) {
      throw new InvalidAccessException("Write exceeds buffer size.");
    }
    _block.writeDouble(val, _currentOffset);
    _currentOffset += advanceOffset(writelen);
  }

  /**
   * Performs _stride byte aligned write of an array of floats.
   * @param array the array to write
   */
  public final void writeAlignedFloatArray(float[] array) {
    int len = array.length;
    long writelen = len * Stdlib.SIZEOF_JFLOAT;
    if (_currentOffset + writelen > _buffersize) {
      throw new InvalidAccessException("Write exceeds buffer size.");
    }
    _block.writeFloatArray(array, 0, _currentOffset, len);
    _currentOffset += advanceOffset(writelen);
  }

  /**
   * Performs _stride byte aligned write of an array of doubles.
   * @param array the array to write
   */
  public final void writeAlignedDoubleArray(double[] array) {
    int len = array.length;
    long writelen = len * Stdlib.SIZEOF_JDOUBLE;
    if (_currentOffset + writelen > _buffersize) {
      throw new InvalidAccessException("Write exceeds buffer size.");
    }
    _block.writeDoubleArray(array, 0, _currentOffset, len);
    _currentOffset += advanceOffset(writelen);
  }

  /**
   * Performs _stride byte aligned write of an array of ints.
   * @param array the array to write
   */
  public final void writeAlignedIntArray(int[] array) {
    int len = array.length;
    long writelen = len * Stdlib.SIZEOF_JINT;
    if (_currentOffset + writelen > _buffersize) {
      throw new InvalidAccessException("Write exceeds buffer size.");
    }
    _block.writeIntArray(array, 0, _currentOffset, len);
    _currentOffset += advanceOffset(writelen);
  }

  /**
   * Performs _stride byte aligned write of an array of longs.
   * @param array the array to write
   */
  public final void writeAlignedLongArray(long[] array) {
    int len = array.length;
    long writelen = len * Stdlib.SIZEOF_JLONG;
    if (_currentOffset + writelen > _buffersize) {
      throw new InvalidAccessException("Write exceeds buffer size.");
    }
    _block.writeLongArray(array, 0, _currentOffset, len);
    _currentOffset += advanceOffset(writelen);
  }

  /**
   * Performs read of an array of doubles in the base units of the array type.
   * For example. If position is given as 5 then the read will occur at offset 5 * sizeof(double).
   * @param array the array in which to place the read data
   * @param position the position from which the read should take place
   */
  public final void readAlignedDoubleArray(double[] array, long position) {
    if (position < 0) {
      throw new InvalidAccessException("Position must be positive");
    }
    int len = array.length;
    long readWidth = len * Stdlib.SIZEOF_JDOUBLE;
    long byteOffset = position * Stdlib.SIZEOF_JDOUBLE;
    if (byteOffset + readWidth > _buffersize) {
      throw new InvalidAccessException("Read exceeds buffer size.");
    }
    if (byteOffset + readWidth > _currentOffset) {
      throw new InvalidAccessException("Read exceeds current position.");
    }
    _block.readDoubleArray(array, 0, byteOffset, len);
  }

  /**
   * Performs read of an array of floats in the base units of the array type.
   * For example. If position is given as 5 then the read will occur at offset 5 * sizeof(float).
   * @param array the array in which to place the read data
   * @param position the position from which the read should take place
   */
  public final void readAlignedFloatArray(float[] array, long position) {
    if (position < 0) {
      throw new InvalidAccessException("Position must be positive");
    }
    int len = array.length;
    long readWidth = len * Stdlib.SIZEOF_JFLOAT;
    long byteOffset = position * Stdlib.SIZEOF_JFLOAT;
    if (byteOffset + readWidth > _buffersize) {
      throw new InvalidAccessException("Read exceeds buffer size.");
    }
    if (byteOffset + readWidth > _currentOffset) {
      throw new InvalidAccessException("Read exceeds current position.");
    }
    _block.readFloatArray(array, 0, byteOffset, len);
  }

  /**
   * Performs read of an array of ints in the base units of the array type.
   * For example. If position is given as 5 then the read will occur at offset 5 * sizeof(int).
   * @param array the array in which to place the read data
   * @param position the position from which the read should take place
   */
  public final void readAlignedIntArray(int[] array, long position) {
    if (position < 0) {
      throw new InvalidAccessException("Position must be positive");
    }
    int len = array.length;
    long readWidth = len * Stdlib.SIZEOF_JINT;
    long byteOffset = position * Stdlib.SIZEOF_JINT;
    if (byteOffset + readWidth > _buffersize) {
      throw new InvalidAccessException("Read exceeds buffer size.");
    }
    if (byteOffset + readWidth > _currentOffset) {
      throw new InvalidAccessException("Read exceeds current position.");
    }
    _block.readIntArray(array, 0, byteOffset, len);
  }

  /**
   * Performs read of an array of longs in the base units of the array type.
   * For example. If position is given as 5 then the read will occur at offset 5 * sizeof(long).
   * @param array the array in which to place the read data
   * @param position the position from which the read should take place
   */
  public final void readAlignedLongArray(long[] array, long position) {
    if (position < 0) {
      throw new InvalidAccessException("Position must be positive");
    }
    int len = array.length;
    long readWidth = len * Stdlib.SIZEOF_JLONG;
    long byteOffset = position * Stdlib.SIZEOF_JLONG;
    if (byteOffset + readWidth > _buffersize) {
      throw new InvalidAccessException("Read exceeds buffer size.");
    }
    if (byteOffset + readWidth > _currentOffset) {
      throw new InvalidAccessException("Read exceeds current position.");
    }
    _block.readLongArray(array, 0, byteOffset, len);
  }

  // Helper methods

  // is a signed int a power of 2, we do not care for negative values as that's blocked by input. To add back in  "&& val > 0"
  private boolean isPow2(long val) {
    return ((val >> 1 & -val) == 0) ? true : false;
  }

  // how much should the offset be advanced given the length to be written? Round up to the next
  // alignment boundary
  private long advanceOffset(long len) {
    if ((len % _stride) != 0) {
      len = _stride * ((len / _stride) + 1);
    }
    return len;
  }

}
