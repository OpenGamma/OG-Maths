/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _TERMINAL_HH
#define _TERMINAL_HH

#include "numeric.hh"
#include "numerictypes.hh"
#include "warningmacros.h"
#include "convertto.hh"

namespace librdag {


namespace detail {

class FuzzyCompareOGTerminalContainer
{
  public:
    FuzzyCompareOGTerminalContainer(const OGTerminal * terminal);
    ~FuzzyCompareOGTerminalContainer();
    const OGTerminal * getTerminal() const;
  private:
    const OGTerminal * _terminal;
};

}

/*
 * Base class for terminal nodes in the AST
 */
class OGTerminal: public OGNumeric
{
  public:
    /**
     * Gets the number of rows
     */
    virtual int getRows() const = 0;
    /**
     * Gets the number of columns
     */
    virtual int getCols() const = 0;
    /**
     * Gets the data length
     */
    virtual int getDatalen() const = 0;
    /**
     * Copies the underlying data to a new real16 array
     */
    virtual real16* toReal16Array() const;
    /**
     * Copies the underlying data to a new complex16 array
     */
    virtual complex16* toComplex16Array() const;
    /**
     * Converts the data to a real16 array of arrays representation
     */
    virtual real16 ** toReal16ArrayOfArrays() const;
    /**
     * Converts the data to a complex16 array of arrays representation
     */
    virtual complex16 ** toComplex16ArrayOfArrays() const;
    /**
     * Returns a real dense matrix representation of this terminal
     */
    virtual OGOwningRealMatrix * asFullOGRealMatrix() const = 0;
    /**
     * Returns a complex dense matrix representation of this terminal
     */
    virtual OGOwningComplexMatrix * asFullOGComplexMatrix() const = 0;
    /**
     *  Returns this
     */
    virtual const OGTerminal* asOGTerminal() const override;
    /**
     * Equals method. Computed bit wise on relavent data fields.
     */
    virtual bool equals(const OGTerminal *)const = 0;
    /**
     * Fuzzy Equals method. Computed with a fuzzy tolerance (to deal with floating point fuzz) on relavent data fields.
     */
    virtual bool fuzzyequals(const OGTerminal *)const = 0;
    /**
     * Checks if two data containers are mathematically equal, regardless of thier underlying data representation.
     */
    virtual bool mathsequals(const OGTerminal *)const;
    virtual bool operator==(const OGTerminal&) const;
    virtual bool operator!=(const OGTerminal&) const;
    virtual bool operator%(const OGTerminal&) const;
    virtual detail::FuzzyCompareOGTerminalContainer& operator~(void) const;
    virtual bool operator==(const detail::FuzzyCompareOGTerminalContainer&) const;
    virtual bool operator!=(const detail::FuzzyCompareOGTerminalContainer&) const;
    OGTerminal();
    virtual ~OGTerminal();
    /**
     * Gets the converter
     */
    const ConvertTo * getConvertTo() const;
  private:
    detail::FuzzyCompareOGTerminalContainer& getFuzzyContainer() const;
    detail::FuzzyCompareOGTerminalContainer * _fuzzyref = nullptr;
    const ConvertTo * _converter = nullptr;
};


/**
 * Things that extend OGScalar
 */

template <typename T>
class OGScalar: public OGTerminal
{
  public:
    OGScalar(T data);
    virtual int getRows() const override;
    virtual int getCols() const override;
    virtual int getDatalen() const override;
    virtual void accept(Visitor &v) const override;
    T getValue() const;
    T ** toArrayOfArrays() const;
    virtual bool equals(const OGTerminal * ) const override;
    virtual bool fuzzyequals(const OGTerminal * ) const override;
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
    virtual OGOwningRealMatrix * asFullOGRealMatrix() const override;
    virtual OGOwningComplexMatrix * asFullOGComplexMatrix() const override;
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
    virtual OGOwningRealMatrix * asFullOGRealMatrix() const override;
    virtual OGOwningComplexMatrix * asFullOGComplexMatrix() const override;
};

class OGIntegerScalar: public OGScalar<int>
{
  public:
    OGIntegerScalar(int data);
    virtual OGNumeric* copy() const override;
    virtual const OGIntegerScalar* asOGIntegerScalar() const override;
    virtual void debug_print() const override;
    virtual ExprType_t getType() const override;
    virtual OGOwningRealMatrix * asFullOGRealMatrix() const override;
    virtual OGOwningComplexMatrix * asFullOGComplexMatrix() const override;
};


template <typename T> class OGArray: public OGTerminal
{
  public:
    T * getData() const; // Returns a pointer to the underlying data
    T * toArray() const; // Returns a pointer to a copy of the underlying data
    virtual int getRows() const override;
    virtual int getCols() const override;
    virtual int getDatalen() const override;
    virtual bool equals(const OGTerminal *)const override;
    virtual bool fuzzyequals(const OGTerminal * ) const override;
  protected:
    void setData(T * data);
    void setRows(int rows);
    void setCols(int cols);
    void setDatalen(int datalen);
    bool fundamentalsEqual(const OGTerminal * ) const;
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
    virtual ExprType_t getType() const override;
    virtual OGOwningRealMatrix * asFullOGRealMatrix() const override;
    virtual OGOwningComplexMatrix * asFullOGComplexMatrix() const override;
};

class OGOwningRealMatrix: public OGRealMatrix
{
  public:
    OGOwningRealMatrix(int rows, int cols);
    virtual ~OGOwningRealMatrix() override;
    using OGRealMatrix::OGRealMatrix;
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
    virtual OGOwningRealMatrix * asFullOGRealMatrix() const override;
    virtual OGOwningComplexMatrix * asFullOGComplexMatrix() const override;
};

class OGOwningComplexMatrix: public OGComplexMatrix
{
  public:
    OGOwningComplexMatrix(int rows, int cols);
    virtual ~OGOwningComplexMatrix() override;
    using OGComplexMatrix::OGComplexMatrix;
};

class OGLogicalMatrix: public OGRealMatrix
{
  public:
    using OGRealMatrix::OGRealMatrix; // TODO: range limit inputs
    virtual const OGLogicalMatrix * asOGLogicalMatrix() const override;
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
    virtual real16* toReal16Array() const override;
    virtual real16** toReal16ArrayOfArrays() const override;
    virtual OGNumeric* copy() const override;
    virtual const OGRealDiagonalMatrix* asOGRealDiagonalMatrix() const override;
    virtual ExprType_t getType() const override;
    virtual OGOwningRealMatrix * asFullOGRealMatrix() const override;
    virtual OGOwningComplexMatrix * asFullOGComplexMatrix() const override;
};

class OGOwningRealDiagonalMatrix: public OGRealDiagonalMatrix
{
  public:
    virtual ~OGOwningRealDiagonalMatrix() override;
    using OGRealDiagonalMatrix::OGRealDiagonalMatrix;
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
    virtual OGOwningRealMatrix * asFullOGRealMatrix() const override;
    virtual OGOwningComplexMatrix * asFullOGComplexMatrix() const override;
};

class OGOwningComplexDiagonalMatrix: public OGComplexDiagonalMatrix
{
  public:
    virtual ~OGOwningComplexDiagonalMatrix() override;
    using OGComplexDiagonalMatrix::OGComplexDiagonalMatrix;
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
    virtual bool equals(const OGTerminal * ) const override; // override OGArray equals to add in calls to check colPtr and rowIdx
    virtual bool fuzzyequals(const OGTerminal * ) const override;
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
    virtual real16* toReal16Array() const override;
    virtual real16** toReal16ArrayOfArrays() const override;
    virtual OGNumeric* copy() const override;
    virtual const OGRealSparseMatrix* asOGRealSparseMatrix() const override;
    virtual ExprType_t getType() const override;
    virtual OGOwningRealMatrix * asFullOGRealMatrix() const override;
    virtual OGOwningComplexMatrix * asFullOGComplexMatrix() const override;
};

class OGOwningRealSparseMatrix: public OGRealSparseMatrix
{
  public:
    virtual ~OGOwningRealSparseMatrix() override;
    using OGRealSparseMatrix::OGRealSparseMatrix;
};

class OGComplexSparseMatrix: public OGSparseMatrix<complex16>
{
  public:
    using OGSparseMatrix::OGSparseMatrix;
    virtual void debug_print() const override;
    virtual complex16* toComplex16Array() const override;
    virtual complex16** toComplex16ArrayOfArrays() const override;
    virtual OGNumeric* copy() const override;
    virtual const OGComplexSparseMatrix* asOGComplexSparseMatrix() const override;
    virtual ExprType_t getType() const override;
    virtual OGOwningRealMatrix * asFullOGRealMatrix() const override;
    virtual OGOwningComplexMatrix * asFullOGComplexMatrix() const override;
};

class OGOwningComplexSparseMatrix: public OGComplexSparseMatrix
{
  public:
    virtual ~OGOwningComplexSparseMatrix() override;
    using OGComplexSparseMatrix::OGComplexSparseMatrix;
};

} // end namespace librdag

#endif // _TERMINAL_HH
