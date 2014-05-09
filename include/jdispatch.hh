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

class Real16AoA
{
  public:
    Real16AoA(const pOGNumeric& node);
    Real16AoA& operator=(const Real16AoA&) = delete;
    Real16AoA(const Real16AoA&) = delete;
    Real16AoA& operator=(Real16AoA&& other);
    Real16AoA(Real16AoA&& other);
    ~Real16AoA();

    jobjectArray toJDoubleAoA(JNIEnv* env) const;
    real16** getData() const;
    int getRows() const;
    int getCols() const;
  private:
    real16** _data;
    int _rows;
    int _cols;
};

class Complex16AoA
{
  public:
    Complex16AoA(const pOGNumeric& node);
    Complex16AoA& operator=(const Complex16AoA&) = delete;
    Complex16AoA(const Complex16AoA&) = delete;
    Complex16AoA& operator=(Complex16AoA&& other);
    Complex16AoA(Complex16AoA&& other);
    ~Complex16AoA();

    jobjectArray realPartToJDoubleAoA(JNIEnv* env) const;
    jobjectArray imagPartToJDoubleAoA(JNIEnv* env) const;
    complex16** getData() const;
    int getRows() const;
    int getCols() const;
  private:
    complex16** _data;
    int _rows;
    int _cols;
};


//jobjectArray convertReal16AoA2JDoubleArrOfArr(JNIEnv * env, Real16AoA arr);
//jobjectArray extractRealPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(JNIEnv * env, complex16 ** inputData, int rows, int cols);
//jobjectArray extractImagPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(JNIEnv * env, complex16 ** inputData, int rows, int cols);

/**
class DLLEXPORT_C DispatchToReal16ArrayOfArrays: public librdag::Visitor
{
  public:
    virtual ~DispatchToReal16ArrayOfArrays();
    virtual void visit(librdag::OGExpr const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGScalar<real16> const *thing);
    virtual void visit(librdag::OGScalar<complex16> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGScalar<int> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGMatrix<real16> const *thing);
    virtual void visit(librdag::OGMatrix<complex16> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGDiagonalMatrix<real16> const *thing);
    virtual void visit(librdag::OGDiagonalMatrix<complex16> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGSparseMatrix<real16> const *thing);
    virtual void visit(librdag::OGSparseMatrix<complex16> const SUPPRESS_UNUSED *thing);
    void setData(real16 ** data);
    void setRows(int rows);
    void setCols(int cols);
    real16 ** getData();
    int getRows();
    int getCols();
  private:
    real16 ** _data = nullptr;
    int rows;
    int cols;
    
};
**/

/*
class DLLEXPORT_C DispatchToComplex16ArrayOfArrays: public librdag::Visitor
{
  public:
    virtual ~DispatchToComplex16ArrayOfArrays();
    virtual void visit(librdag::OGExpr const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGScalar<real16> const *thing);
    virtual void visit(librdag::OGScalar<complex16> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGScalar<int> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGMatrix<real16> const *thing);
    virtual void visit(librdag::OGMatrix<complex16> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGDiagonalMatrix<real16> const *thing);
    virtual void visit(librdag::OGDiagonalMatrix<complex16> const SUPPRESS_UNUSED *thing);
    virtual void visit(librdag::OGSparseMatrix<real16> const *thing);
    virtual void visit(librdag::OGSparseMatrix<complex16> const SUPPRESS_UNUSED *thing);
    void setData(complex16 ** data);
    void setRows(int rows);
    void setCols(int cols);
    complex16 ** getData();
    int getRows();
    int getCols();
  private:
    complex16 ** _data = nullptr;
    int rows;
    int cols;    
};*/
  
class DLLEXPORT_C DispatchToOGTerminal: public librdag::Visitor
{
  public:
    DispatchToOGTerminal(JNIEnv* env);
    virtual ~DispatchToOGTerminal();
    virtual void visit(librdag::OGExpr const *thing);
    virtual void visit(librdag::OGScalar<real16> const *thing);
    virtual void visit(librdag::OGScalar<complex16> const *thing);
    virtual void visit(librdag::OGScalar<int> const *thing);
    virtual void visit(librdag::OGMatrix<real16> const *thing);
    virtual void visit(librdag::OGMatrix<complex16> const *thing);
    virtual void visit(librdag::OGDiagonalMatrix<real16> const *thing);
    virtual void visit(librdag::OGDiagonalMatrix<complex16> const *thing);
    virtual void visit(librdag::OGSparseMatrix<real16> const *thing);
    virtual void visit(librdag::OGSparseMatrix<complex16> const *thing);
    jobject getObject();
  private:
    void setObject(jobject obj);
    jobject _obj = nullptr;
    JNIEnv* _env;
};



} // namespace convert

#endif // _JDISPATCH_HH

