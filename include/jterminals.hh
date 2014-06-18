/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _JTERMINALS_HH
#define _JTERMINALS_HH

#include "terminal.hh"
#include "jvmmanager.hh"

namespace convert {

using namespace librdag;

/**
 * helper function to get size_ts from int getFOO() in java
 */
DLLEXPORT_C size_t getSizeTFromVoidJMethod(jmethodID id, jobject obj);


/*
 * An OGRealScalar backed by data pinned from a Java based OGRealScalar.
 * Note that _dataref is assigned on construction of the OGRealScalar base class,
 * this is to keep JNI calls to a minimum by holding reference to the data pointer needed
 * to free via the ReleasePrimitiveArrayCritical() function.
 */
class DLLEXPORT_C JOGRealScalar: public OGRealScalar
{
  public:
    typedef std::shared_ptr<const JOGRealScalar> Ptr;
    static JOGRealScalar::Ptr create(jobject obj);
    virtual ~JOGRealScalar() override;
    virtual void debug_print() const override;
  private:
    JOGRealScalar(jobject obj);
    jobject _backingObject = nullptr;
    real8 * _dataRef = nullptr;
};

/*
 * An OGComplexScalar backed by data pinned from a Java based OGRealScalar.
 * Note that _dataref is assigned on construction of the OGComplexScalar base class,
 * this is to keep JNI calls to a minimum by holding reference to the data pointer needed
 * to free via the ReleasePrimitiveArrayCritical() function.
 */
class DLLEXPORT_C JOGComplexScalar: public OGComplexScalar
{
  public:
    typedef std::shared_ptr<const JOGComplexScalar> Ptr;
    static JOGComplexScalar::Ptr create(jobject obj);
    virtual ~JOGComplexScalar() override;
    virtual void debug_print() const override;
  private:
    JOGComplexScalar(jobject obj);
    jobject _backingObject = nullptr;
    complex16 * _dataRef = nullptr;
};

/*
 * An OGIntegerScalar backed by data pinned from a Java based OGIntegerScalar.
 * Note that _dataref is assigned on construction of the OGIntegerScalar base class,
 * this is to keep JNI calls to a minimum by holding reference to the data pointer needed
 * to free via the ReleasePrimitiveArrayCritical() function.
 */
class DLLEXPORT_C JOGIntegerScalar: public OGIntegerScalar
{
  public:
    typedef std::shared_ptr<const JOGIntegerScalar> Ptr;
    static JOGIntegerScalar::Ptr create(jobject obj);
    virtual ~JOGIntegerScalar() override;
    virtual void debug_print() const override;
  private:
    JOGIntegerScalar(jobject obj);
    jobject _backingObject = nullptr;
    jint* _dataRef = nullptr;
};

/*
 * An OGRealDenseMatrix backed by data pinned from a Java based OGRealDenseMatrix
 */
class DLLEXPORT_C JOGRealDenseMatrix: public OGRealDenseMatrix
{
  public:
    typedef std::shared_ptr<const JOGRealDenseMatrix> Ptr;
    static JOGRealDenseMatrix::Ptr create(jobject obj);
    virtual ~JOGRealDenseMatrix() override;
    virtual void debug_print() const override;
  private:
    JOGRealDenseMatrix(jobject obj);
    jobject _backingObject = nullptr;
};

/*
 * An OGComplexMatrix backed by data pinned from a Java based OGComplexMatrix
 */
class DLLEXPORT_C JOGComplexMatrix: public OGComplexMatrix
{
  public:
    typedef std::shared_ptr<const JOGComplexMatrix> Ptr;
    static JOGComplexMatrix::Ptr create(jobject obj);
    virtual ~JOGComplexMatrix() override;
    virtual void debug_print() const override;
  private:
    JOGComplexMatrix(jobject obj);
    jobject _backingObject = nullptr;
};

/*
 * An OGLogicalMatrix backed by data pinned from a Java based OGLogicalMatrix
 */
class DLLEXPORT_C JOGLogicalMatrix: public OGLogicalMatrix
{
  public:
    typedef std::shared_ptr<const JOGLogicalMatrix> Ptr;
    static JOGLogicalMatrix::Ptr create(jobject obj);
    virtual ~JOGLogicalMatrix() override;
    virtual void debug_print() const override;
  private:
    JOGLogicalMatrix(jobject obj);
    jobject _backingObject = nullptr;
};

/*
 * An OGRealSparseMatrix backed by data pinned from a Java based OGRealSparseMatrix
 */
class DLLEXPORT_C JOGRealSparseMatrix: public OGRealSparseMatrix
{
  public:
    typedef std::shared_ptr<const JOGRealSparseMatrix> Ptr;
    static JOGRealSparseMatrix::Ptr create(jobject obj);
    virtual ~JOGRealSparseMatrix() override;
    virtual void debug_print() const override;
    virtual real8** toReal8ArrayOfArrays() const override;
    virtual complex16** toComplex16ArrayOfArrays() const override;
  private:
    JOGRealSparseMatrix(jobject obj);
    jobject _backingObject = nullptr;
};

/*
 * An OGComplexSparseMatrix backed by data pinned from a Java based OGComplexSparseMatrix
 */
class DLLEXPORT_C JOGComplexSparseMatrix: public OGComplexSparseMatrix
{
  public:
    typedef std::shared_ptr<const JOGComplexSparseMatrix> Ptr;
    static JOGComplexSparseMatrix::Ptr create(jobject obj);
    virtual ~JOGComplexSparseMatrix() override;
    virtual void debug_print() const override;
    virtual real8** toReal8ArrayOfArrays() const override;
    virtual complex16** toComplex16ArrayOfArrays() const override;
  private:
    JOGComplexSparseMatrix(jobject obj);
    jobject _backingObject = nullptr;
};

/*
 * An OGRealDiagonalMatrix backed by data pinned from a Java based OGRealDiagonalMatrix
 */
class DLLEXPORT_C JOGRealDiagonalMatrix: public OGRealDiagonalMatrix
{
  public:
    typedef std::shared_ptr<const JOGRealDiagonalMatrix> Ptr;
    static JOGRealDiagonalMatrix::Ptr create(jobject obj);
    virtual ~JOGRealDiagonalMatrix() override;
    virtual void debug_print() const override;
  private:
    JOGRealDiagonalMatrix(jobject obj);
    jobject _backingObject = nullptr;
};

/*
 * An OGComplexDiagonalMatrix backed by data pinned from a Java based OGComplexDiagonalMatrix
 */
class DLLEXPORT_C JOGComplexDiagonalMatrix: public OGComplexDiagonalMatrix
{
  public:
    typedef std::shared_ptr<const JOGComplexDiagonalMatrix> Ptr;
    static JOGComplexDiagonalMatrix::Ptr create(jobject obj);
    virtual ~JOGComplexDiagonalMatrix() override;
    virtual void debug_print() const override;
  private:
    JOGComplexDiagonalMatrix(jobject obj);
    jobject _backingObject = nullptr;
};

} // namespace convert
#endif // _JTERMINALS_HH
