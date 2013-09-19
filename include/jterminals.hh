/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _JTERMINALS_HH
#define _JTERMINALS_HH

#include "expression.hh"
#include "jni.h"

namespace convert {

using namespace librdag;

/*
 * An OGRealScalar backed by data pinned from a Java based OGRealScalar.
 * Note that _dataref is assigned on construction of the OGRealScalar base class,
 * this is to keep JNI calls to a minimum by holding reference to the data pointer needed
 * to free via the ReleasePrimitiveArrayCritical() function.
 */
class JOGRealScalar: public OGRealScalar
{
  public:
    using OGRealScalar::OGRealScalar;
    JOGRealScalar(jobject * obj);
    virtual ~JOGRealScalar() override;
    virtual void debug_print() const override;
  private:
    jobject * _backingObject = nullptr;
    real16 * _dataRef = nullptr;
};

/*
 * An OGComplexScalar backed by data pinned from a Java based OGRealScalar.
 * Note that _dataref is assigned on construction of the OGComplexScalar base class,
 * this is to keep JNI calls to a minimum by holding reference to the data pointer needed
 * to free via the ReleasePrimitiveArrayCritical() function.
 */
class JOGComplexScalar: public OGComplexScalar
{
  public:
    using OGComplexScalar::OGComplexScalar;
    JOGComplexScalar(jobject * obj);
    virtual ~JOGComplexScalar() override;
    virtual void debug_print() const override;
  private:
    jobject * _backingObject = nullptr;
    complex16 * _dataRef = nullptr;
};

/*
 * An OGRealMatrix backed by data pinned from a Java based OGRealMatrix
 */
class JOGRealMatrix: public OGRealMatrix
{
  public:
    using OGRealMatrix::OGRealMatrix;
    JOGRealMatrix(jobject * obj);
    virtual ~JOGRealMatrix() override;
    virtual void debug_print() const override;
  private:
    jobject * _backingObject = nullptr;
};

/*
 * An OGComplexMatrix backed by data pinned from a Java based OGComplexMatrix
 */
class JOGComplexMatrix: public OGComplexMatrix
{
  public:
    JOGComplexMatrix(jobject * obj);
    virtual ~JOGComplexMatrix() override;
    virtual void debug_print() const override;
  private:
    jobject * _backingObject = nullptr;
};

/*
 * An OGRealSparseMatrix backed by data pinned from a Java based OGRealSparseMatrix
 */
class JOGRealSparseMatrix: public OGRealSparseMatrix
{
  public:
    JOGRealSparseMatrix(jobject * obj);
    virtual ~JOGRealSparseMatrix() override;
    virtual void debug_print() const override;
    virtual real16** toReal16ArrayOfArrays() const override;
    virtual complex16** toComplex16ArrayOfArrays() const override;
  private:
    jobject * _backingObject = nullptr;
};

/*
 * An OGComplexSparseMatrix backed by data pinned from a Java based OGComplexSparseMatrix
 */
class JOGComplexSparseMatrix: public OGComplexSparseMatrix
{
  public:
    JOGComplexSparseMatrix(jobject * obj);
    virtual ~JOGComplexSparseMatrix() override;
    virtual void debug_print() const override;
    virtual real16** toReal16ArrayOfArrays() const override;
    virtual complex16** toComplex16ArrayOfArrays() const override;
  private:
    jobject * _backingObject = nullptr;
};

/*
 * An OGRealDiagonalMatrix backed by data pinned from a Java based OGRealDiagonalMatrix
 */
class JOGRealDiagonalMatrix: public OGRealDiagonalMatrix
{
  public:
    JOGRealDiagonalMatrix(jobject * obj);
    virtual ~JOGRealDiagonalMatrix() override;
    virtual void debug_print() const override;
  private:
    jobject * _backingObject = nullptr;
};

/*
 * An OGComplexDiagonalMatrix backed by data pinned from a Java based OGComplexDiagonalMatrix
 */
class JOGComplexDiagonalMatrix: public OGComplexDiagonalMatrix
{
  public:
    JOGComplexDiagonalMatrix(jobject * obj);
    virtual ~JOGComplexDiagonalMatrix() override;
    virtual void debug_print() const override;
  private:
    jobject * _backingObject = nullptr;
};

} // namespace convert
#endif // _JTERMINALS_HH
