/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _EXPRESSION_HH
#define _EXPRESSION_HH

// bindings
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <memory>

#include "numerictypes.hh"
#include "visitor.hh"
#include "exceptions.hh"
#include "containers.hh"
#include "uncopyable.hh"

using namespace std;


/*
 * The namespace for the DAG library
 */
namespace librdag
{

/*
 * Forward declaration of class types for pidgin RTTI in OGNumeric
 */

class COPY;
class PLUS;
class MINUS;
class SVD;
class SELECTRESULT;
class OGTerminal;
class OGRealScalar;
class OGComplexScalar;
class OGIntegerScalar;
class OGRealMatrix;
class OGComplexMatrix;
class OGRealDiagonalMatrix;
class OGComplexDiagonalMatrix;
class OGRealSparseMatrix;
class OGComplexSparseMatrix;

/*
 * Base class for absolutely everything!
 */
class OGNumeric: private Uncopyable
{
  public:
    virtual ~OGNumeric();
    virtual void debug_print() const = 0;
    virtual void accept(Visitor &v) const = 0;
    virtual OGNumeric* copy() const = 0;
    virtual const COPY* asCOPY() const;
    virtual const PLUS* asPLUS() const;
    virtual const MINUS* asMINUS() const;
    virtual const SVD* asSVD() const;
    virtual const SELECTRESULT* asSELECTRESULT() const;
    virtual const OGTerminal* asOGTerminal() const;
    virtual const OGRealScalar* asOGRealScalar() const;
    virtual const OGComplexScalar* asOGComplexScalar() const;
    virtual const OGIntegerScalar* asOGIntegerScalar() const;
    virtual const OGRealMatrix* asOGRealMatrix() const;
    virtual const OGComplexMatrix* asOGComplexMatrix() const;
    virtual const OGRealDiagonalMatrix* asOGRealDiagonalMatrix() const;
    virtual const OGComplexDiagonalMatrix* asOGComplexDiagonalMatrix() const;
    virtual const OGRealSparseMatrix* asOGRealSparseMatrix() const;
    virtual const OGComplexSparseMatrix* asOGComplexSparseMatrix() const;
};

/*
 * Base class for terminal nodes in the AST
 */
class OGTerminal: public OGNumeric
{
  public:
    virtual real16 ** toReal16ArrayOfArrays() const;
    virtual complex16 ** toComplex16ArrayOfArrays() const;
    virtual const OGTerminal* asOGTerminal() const override;
};

/*
 * Container for expression arguments
 */
typedef PtrVector<OGNumeric> ArgContainer;

/**
 *  Expr type
 */
class OGExpr: public OGNumeric
{
  public:
    virtual ~OGExpr();
    const ArgContainer* getArgs() const;
    size_t getNArgs() const;
    virtual void debug_print() const override;
    virtual void accept(Visitor &v) const override;
  protected:
    OGExpr(ArgContainer* args);
  private:
    ArgContainer* _args;
};

/**
 * Things that extend OGExpr
 */

class OGUnaryExpr: public OGExpr
{
  protected:
    OGUnaryExpr(ArgContainer* args);
};

class OGBinaryExpr : public OGExpr
{
  protected:
    OGBinaryExpr(ArgContainer* args);
};

class COPY: public OGUnaryExpr
{
  public:
    COPY(ArgContainer *args);
    virtual OGNumeric* copy() const override;
    virtual const COPY* asCOPY() const override;
    virtual void debug_print() const override;
};


class PLUS: public OGBinaryExpr
{
  public:
    PLUS(ArgContainer *args);
    virtual OGNumeric* copy() const override;
    virtual const PLUS* asPLUS() const override;
    virtual void debug_print() const override;
};


class MINUS: public OGBinaryExpr
{
  public:
    MINUS(ArgContainer *args);
    virtual OGNumeric* copy() const override;
    virtual const MINUS* asMINUS() const override;
    virtual void debug_print() const override;
};

class SVD: public OGUnaryExpr
{
  public:
    SVD(ArgContainer *args);
    virtual OGNumeric* copy() const override;
    virtual const SVD* asSVD() const override;
    virtual void debug_print() const override;
};

class SELECTRESULT: public OGBinaryExpr
{
  public:
    SELECTRESULT(ArgContainer *args);
    virtual OGNumeric* copy() const override;
    virtual const SELECTRESULT* asSELECTRESULT() const override;
    virtual void debug_print() const override;
};

/**
 * Things that extend OGScalar
 */

template <typename T>
class OGScalar: public OGTerminal
{
  public:
    OGScalar(T data);
    virtual void accept(Visitor &v) const override;
    T getValue() const;
  protected:
    T ** toArrayOfArrays() const;
  private:
    T _value;
};

extern template class OGScalar<real16>;
extern template class OGScalar<complex16>;
extern template class OGScalar<int>;

class OGRealScalar: public OGScalar<real16>
{
  public:
    OGRealScalar(real16 data);
    virtual real16 ** toReal16ArrayOfArrays() const override;
    virtual OGNumeric* copy() const override;
    virtual const OGRealScalar* asOGRealScalar() const override;
    virtual void debug_print() const override;
};

class OGComplexScalar: public OGScalar<complex16>
{
  public:
    OGComplexScalar(complex16 data);
    virtual complex16 ** toComplex16ArrayOfArrays() const override;
    virtual OGNumeric* copy() const override;
    virtual const OGComplexScalar* asOGComplexScalar() const override;
    virtual void debug_print() const override;
};

class OGIntegerScalar: public OGScalar<int>
{
  public:
    OGIntegerScalar(int data);
    virtual OGNumeric* copy() const override;
    virtual const OGIntegerScalar* asOGIntegerScalar() const override;
    virtual void debug_print() const override;
};

template <typename T> class OGArray: public OGTerminal
{
  public:
    virtual void accept(Visitor &v) const override;
    T * getData() const;
    int getRows() const;
    int getCols() const;
    int getDatalen() const;
  protected:
    void setData(T * data);
    void setRows(int rows);
    void setCols(int cols);
    void setDatalen(int datalen);
  private:
    T * _data = nullptr;
    int _rows  = 0;
    int _cols  = 0;
    int _datalen = 0;
};

/**
 * Things that extend OGMatrix
 */

template <typename T> class OGMatrix: public OGArray<T>
{
  public:
    OGMatrix(T * data, int rows, int cols);
    virtual void accept(Visitor &v) const override;
    T** toArrayOfArrays() const;
};

extern template class OGMatrix<real16>;
extern template class OGMatrix<complex16>;

class OGRealMatrix: public OGMatrix<real16>
{
  public:
    using OGMatrix::OGMatrix;
    virtual void debug_print() const override;
    virtual real16 ** toReal16ArrayOfArrays() const override;
    virtual OGNumeric* copy() const override;
    virtual const OGRealMatrix* asOGRealMatrix() const override;
};

class OGComplexMatrix: public OGMatrix<complex16>
{
  public:
    using OGMatrix::OGMatrix;
    virtual void debug_print() const override;
    virtual complex16 ** toComplex16ArrayOfArrays() const override;
    virtual OGNumeric* copy() const override;
    virtual const OGComplexMatrix* asOGComplexMatrix() const override;
};

class OGLogicalMatrix: public OGRealMatrix
{

};

/**
 * Things that extend OGDiagonalMatrix
 */

template <typename T> class OGDiagonalMatrix: public OGArray<T>
{
  public:
    OGDiagonalMatrix(T * data, int rows, int cols);
    virtual void accept(Visitor &v) const override;
    T** toArrayOfArrays() const;
};

extern template class OGDiagonalMatrix<real16>;
extern template class OGDiagonalMatrix<complex16>;

class OGRealDiagonalMatrix: public OGDiagonalMatrix<real16>
{
  public:
    using OGDiagonalMatrix::OGDiagonalMatrix;
    virtual void debug_print() const override;
    virtual real16** toReal16ArrayOfArrays() const override;
    virtual OGNumeric* copy() const override;
    virtual const OGRealDiagonalMatrix* asOGRealDiagonalMatrix() const override;
};

class OGComplexDiagonalMatrix: public OGDiagonalMatrix<complex16>
{
  public:
    using OGDiagonalMatrix::OGDiagonalMatrix;
    virtual void debug_print() const override;
    virtual complex16** toComplex16ArrayOfArrays() const override;
    virtual OGNumeric* copy() const override;
    virtual const OGComplexDiagonalMatrix* asOGComplexDiagonalMatrix() const override;
};

/**
 * Things that extend OGSparseMatrix
 */

template <typename T> class OGSparseMatrix: public OGArray<T>
{
  public:
    OGSparseMatrix(int * colPtr, int * rowIdx, T * data, int rows, int cols);
    virtual void accept(Visitor &v) const override;
    int* getColPtr() const;
    int* getRowIdx() const;
  protected:
    void setColPtr(int * colPtr);
    void setRowIdx(int * rowIdx);
  private:
    int* _colPtr = nullptr; // the column pointer index
    int* _rowIdx = nullptr; // the row index
};

extern template class OGSparseMatrix<real16>;
extern template class OGSparseMatrix<complex16>;

class OGRealSparseMatrix: public OGSparseMatrix<real16>
{
  public:
    using OGSparseMatrix::OGSparseMatrix;
    virtual void debug_print() const override;
    virtual OGNumeric* copy() const override;
    virtual const OGRealSparseMatrix* asOGRealSparseMatrix() const override;
};

class OGComplexSparseMatrix: public OGSparseMatrix<complex16>
{
  public:
    using OGSparseMatrix::OGSparseMatrix;
    virtual void debug_print() const override;
    virtual OGNumeric* copy() const override;
    virtual const OGComplexSparseMatrix* asOGComplexSparseMatrix() const override;
};

} // namespace librdag

#endif
