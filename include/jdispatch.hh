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
    int getRows() const;
    /**
     * Get the number of columns.
     */
    int getCols() const;
  private:
    real8** _data;
    int _rows;
    int _cols;
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
    int getRows() const;
    /**
     * Get the number of columns.
     */
    int getCols() const;
  private:
    /**
     * Used by {real,imag}PartToJDoubleAoA
     */
    template<real8 (F)(const complex16&)> jobjectArray toJDoubleAoA(JNIEnv* env) const;
    complex16** _data;
    int _rows;
    int _cols;
};
  
class DLLEXPORT_C DispatchToOGTerminal: public librdag::Visitor
{
  public:
    DispatchToOGTerminal(JNIEnv* env);
    virtual ~DispatchToOGTerminal();
    virtual void visit(librdag::OGExpr const *thing);
    virtual void visit(librdag::OGScalar<real8> const *thing);
    virtual void visit(librdag::OGScalar<complex16> const *thing);
    virtual void visit(librdag::OGScalar<int> const *thing);
    virtual void visit(librdag::OGMatrix<real8> const *thing);
    virtual void visit(librdag::OGMatrix<complex16> const *thing);
    virtual void visit(librdag::OGDiagonalMatrix<real8> const *thing);
    virtual void visit(librdag::OGDiagonalMatrix<complex16> const *thing);
    virtual void visit(librdag::OGSparseMatrix<real8> const *thing);
    virtual void visit(librdag::OGSparseMatrix<complex16> const *thing);
    jobject getObject();
  private:
    void setObject(jobject obj);
    jobject _obj = nullptr;
    JNIEnv* _env;
};



} // namespace convert

#endif // _JDISPATCH_HH

