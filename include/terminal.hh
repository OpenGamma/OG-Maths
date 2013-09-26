/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _TERMINAL_HH
#define _TERMINAL_HH

#include "numeric.hh"
#include "numerictypes.hh"

namespace librdag {
   
/*
 * Base class for terminal nodes in the AST
 */
class OGTerminal: public OGNumeric
{
  public:
    virtual real16* toReal16Array() const;
    virtual complex16* toComplex16Array() const;
    virtual real16 ** toReal16ArrayOfArrays() const;
    virtual complex16 ** toComplex16ArrayOfArrays() const;
    virtual const OGTerminal* asOGTerminal() const override;
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
    T ** toArrayOfArrays() const;
  protected:
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
    virtual ExprType_t getType() const override;    
};

class OGComplexScalar: public OGScalar<complex16>
{
  public:
    OGComplexScalar(complex16 data);
    virtual complex16 ** toComplex16ArrayOfArrays() const override;
    virtual OGNumeric* copy() const override;
    virtual const OGComplexScalar* asOGComplexScalar() const override;
    virtual void debug_print() const override;
    virtual ExprType_t getType() const override;    
};

class OGIntegerScalar: public OGScalar<int>
{
  public:
    OGIntegerScalar(int data);
    virtual OGNumeric* copy() const override;
    virtual const OGIntegerScalar* asOGIntegerScalar() const override;
    virtual void debug_print() const override;
    virtual ExprType_t getType() const override;
};

template <typename T> class OGArray: public OGTerminal
{
  public:
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
  protected:
    OGMatrix(int rows, int cols);
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
    virtual ExprType_t getType() const override;
};

class OGComplexMatrix: public OGMatrix<complex16>
{
  public:
    using OGMatrix::OGMatrix;
    virtual void debug_print() const override;
    virtual complex16 ** toComplex16ArrayOfArrays() const override;
    virtual OGNumeric* copy() const override;
    virtual const OGComplexMatrix* asOGComplexMatrix() const override;
    virtual ExprType_t getType() const override;
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
    T* toArray() const;
    T** toArrayOfArrays() const;
  protected:
    OGDiagonalMatrix(int rows, int cols);
};

extern template class OGDiagonalMatrix<real16>;
extern template class OGDiagonalMatrix<complex16>;

class OGRealDiagonalMatrix: public OGDiagonalMatrix<real16>
{
  public:
    using OGDiagonalMatrix::OGDiagonalMatrix;
    virtual void debug_print() const override;
    virtual real16* toReal16Array() const override;
    virtual real16** toReal16ArrayOfArrays() const override;
    virtual OGNumeric* copy() const override;
    virtual const OGRealDiagonalMatrix* asOGRealDiagonalMatrix() const override;
    virtual ExprType_t getType() const override;    
};

class OGComplexDiagonalMatrix: public OGDiagonalMatrix<complex16>
{
  public:
    using OGDiagonalMatrix::OGDiagonalMatrix;
    virtual void debug_print() const override;
    virtual complex16* toComplex16Array() const override;
    virtual complex16** toComplex16ArrayOfArrays() const override;
    virtual OGNumeric* copy() const override;
    virtual const OGComplexDiagonalMatrix* asOGComplexDiagonalMatrix() const override;
    virtual ExprType_t getType() const override;    
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
    T** toArrayOfArrays() const;   
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
    virtual real16** toReal16ArrayOfArrays() const override;
    virtual OGNumeric* copy() const override;
    virtual const OGRealSparseMatrix* asOGRealSparseMatrix() const override;
    virtual ExprType_t getType() const override;
};

class OGComplexSparseMatrix: public OGSparseMatrix<complex16>
{
  public:
    using OGSparseMatrix::OGSparseMatrix;
    virtual void debug_print() const override;
    virtual complex16** toComplex16ArrayOfArrays() const override;
    virtual OGNumeric* copy() const override;
    virtual const OGComplexSparseMatrix* asOGComplexSparseMatrix() const override;
    virtual ExprType_t getType() const override;
};


} // end namespace librdag

#endif // _TERMINAL_HH
