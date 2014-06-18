/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _JDISPATCH_HH
#define _JDISPATCH_HH

#include "numerictypes.hh"
#include "expression.hh"
#include "terminal.hh"
#include "warningmacros.h"
#include "exceptions.hh"
#include "jvmmanager.hh"
#include "modifiermacros.h"
#include "uncopyable.hh"

namespace convert {

using namespace librdag;  

/**
 * Class representing a real8 array of arrays for conversion to Java
 */
class Real8AoA
{
  public:
    Real8AoA(const OGNumeric::Ptr& node);
    Real8AoA& operator=(const Real8AoA&) = delete;
    Real8AoA(const Real8AoA&) = delete;
    Real8AoA& operator=(Real8AoA&& other);
    Real8AoA(Real8AoA&& other);
    ~Real8AoA();

    /**
     * create a jobjectarray with a copy of the values in this Real8AoA
     *
     * @param env the JNI environment pointer
     */
    jobjectArray toJDoubleAoA(JNIEnv* env) const;
    /**
     * Get the underlying data.
     */
    real8** getData() const;
    /**
     * Get the number of rows.
     */
    size_t getRows() const;
    /**
     * Get the number of columns.
     */
    size_t getCols() const;
  private:
    real8** _data;
    size_t _rows;
    size_t _cols;
};

/**
 * Class representing a real8 array of arrays for conversion to Java
 */
class Complex16AoA
{
  public:
    Complex16AoA(const OGNumeric::Ptr& node);
    Complex16AoA& operator=(const Complex16AoA&) = delete;
    Complex16AoA(const Complex16AoA&) = delete;
    Complex16AoA& operator=(Complex16AoA&& other);
    Complex16AoA(Complex16AoA&& other);
    ~Complex16AoA();

    /**
     * create a jobjectarray with a copy of the real values.
     *
     * @param env the JNI environment pointer
     */
    jobjectArray realPartToJDoubleAoA(JNIEnv* env) const;
    /**
     * create a jobjectarray with a copy of the imaginary values.
     *
     * @param env the JNI environment pointer
     */
    jobjectArray imagPartToJDoubleAoA(JNIEnv* env) const;
    /**
     * Get the underlying data.
     */
    complex16** getData() const;
    /**
     * Get the number of rows.
     */
    size_t getRows() const;
    /**
     * Get the number of columns.
     */
    size_t getCols() const;
  private:
    /**
     * Used by {real,imag}PartToJDoubleAoA
     */
    template<real8 (F)(const complex16&)> jobjectArray toJDoubleAoA(JNIEnv* env) const;
    complex16** _data;
    size_t _rows;
    size_t _cols;
};

/**
 * Class for converting a terminal back to a java terminal
 */
class JavaTerminal: private Uncopyable
{
  public:
    /**
     * Create a JavaTerminal from a node
     *
     * @param env The JNI environment pointer
     * @param node The terminal to translate into a java terminal.
     */
    JavaTerminal(JNIEnv* env, const OGNumeric::Ptr& node);

    /**
     * Get the java object represented by this instance.
     *
     * @return the java object.
     */
    jobject getObject();
  private:
    /**
     * Create a Java OGRealScalar object and store it.
     *
     * @param env the JNI environment pointer
     * @parame node an OGRealScalar to convert
     */
    void createRealScalar(JNIEnv* env, const OGNumeric::Ptr& node);
    /**
     * Create a Java OGComplexScalar object and store it.
     *
     * @param env the JNI environment pointer
     * @parame node an OGComplexScalar to convert
     */
    void createComplexScalar(JNIEnv* env, const OGNumeric::Ptr& node);
    /**
     * Create a Java OGRealMatrix object and store it.
     *
     * @param env the JNI environment pointer
     * @parame node an OGRealMatrix to convert
     */
    void createRealMatrix(JNIEnv* env, const OGNumeric::Ptr& node);
    /**
     * Create a Java OGComplexMatrix object and store it.
     *
     * @param env the JNI environment pointer
     * @parame node an OGComplexMatrix to convert
     */
    void createComplexMatrix(JNIEnv* env, const OGNumeric::Ptr& node);
    /**
     * Create a Java OGRealDiagonalMatrix object and store it.
     *
     * @param env the JNI environment pointer
     * @parame node an OGRealDiagonalMatrix to convert
     */
    void createRealDiagonalMatrix(JNIEnv* env, const OGNumeric::Ptr& node);
    /**
     * Create a Java OGComplexDiagonalMatrix object and store it.
     *
     * @param env the JNI environment pointer
     * @parame node an OGComplexDiagonalMatrix to convert
     */
    void createComplexDiagonalMatrix(JNIEnv* env, const OGNumeric::Ptr& node);
    /**
     * Create a Java OGRealSparseMatrix object and store it.
     *
     * @param env the JNI environment pointer
     * @parame node an OGRealSparseMatrix to convert
     */
    void createRealSparseMatrix(JNIEnv* env, const OGNumeric::Ptr& node);
    /**
     * Create a Java OGComplexSparseMatrix object and store it.
     *
     * @param env the JNI environment pointer
     * @parame node an OGComplexSparseMatrix to convert
     */
    void createComplexSparseMatrix(JNIEnv* env, const OGNumeric::Ptr& node);
    jobject _obj;
};

} // namespace convert

#endif // _JDISPATCH_HH

