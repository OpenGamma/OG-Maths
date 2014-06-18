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
#include <limits>

namespace librdag {


namespace detail {

class FuzzyCompareOGTerminalContainer
{
  public:
    FuzzyCompareOGTerminalContainer(const std::shared_ptr<const OGTerminal>& terminal);
    ~FuzzyCompareOGTerminalContainer();
    const std::shared_ptr<const OGTerminal> getTerminal() const;
  private:
    const std::shared_ptr<const OGTerminal> _terminal;
};

} // end namespace detail


/**
 * Enum for describing the access pattern used for the POD that backs the OGTerminal types.
 */
enum DATA_ACCESS
{
  /**
   * VIEWER access is specified to mean that the underlying data is not owned
   * by the object, i.e. it is a view only.
   */
  VIEWER,
  /**
   * OWNER access is specified to mean that the underlying data is owned by the
   * object, the object therefore has responsibility for managing the lifetime of the data.
   */
  OWNER
};

/*
 * Base class for terminal nodes in the AST
 */
class OGTerminal: public OGNumeric
{
  public:
    /**
     * Pointer type.
     */
    typedef std::shared_ptr<const OGTerminal> Ptr;
    /**
     * Gets the number of rows
     */
    virtual size_t getRows() const = 0;
    /**
     * Gets the number of columns
     */
    virtual size_t getCols() const = 0;
    /**
     * Gets the data length
     */
    virtual size_t getDatalen() const = 0;
    /**
     * Copies the underlying data to a new real8 array
     */
    virtual real8* toReal8Array() const;
    /**
     * Copies the underlying data to a new complex16 array
     */
    virtual complex16* toComplex16Array() const;
    /**
     * Converts the data to a real8 array of arrays representation
     */
    virtual real8 ** toReal8ArrayOfArrays() const;
    /**
     * Converts the data to a complex16 array of arrays representation
     */
    virtual complex16 ** toComplex16ArrayOfArrays() const;
    /**
     * Returns a real dense matrix representation of this terminal
     */
    virtual std::shared_ptr<const OGRealDenseMatrix> asFullOGRealDenseMatrix() const = 0;
    /**
     * Returns a complex dense matrix representation of this terminal
     */
    virtual std::shared_ptr<const OGComplexMatrix> asFullOGComplexMatrix() const = 0;
    /**
     *  Returns this
     */
    virtual OGTerminal::Ptr asOGTerminal() const override;
    /**
     * Equals method. Computed bit wise on relavent data fields.
     */
    virtual bool equals(const OGTerminal::Ptr&) const = 0;
    /**
     * Fuzzy Equals method. Computed with a fuzzy tolerance (to deal with floating point fuzz) on relavent data fields.
     * @param term the terminal to compare against.
     * @return true if \a this is considered fuzzy equal to \a term, false else
     */
    virtual bool fuzzyequals(const OGTerminal::Ptr& term) const = 0;
    /**
     * Fuzzy Equals method. Computed with a fuzzy tolerance (to deal with floating point fuzz) on relavent data fields.
     * @param term the terminal to compare against.
     * @param maxabserror the maximum absolute error to use during comparison
     * @param maxrelerror the maximum relative error to use during comparison
     * @return true if \a this is considered fuzzy equal to \a term, false else
     */
    virtual bool fuzzyequals(const OGTerminal::Ptr& term, real8 maxabserror, real8 maxrelerror) const = 0;
    /**
     * Create a copy of this terminal that owns its own data.
     */
    virtual OGTerminal::Ptr createOwningCopy() const = 0;
    /**
     * Create a complex space terminal version of this terminal that owns its own data.
     */
    virtual OGTerminal::Ptr createComplexOwningCopy() const = 0;

    /**
     * Checks if two data containers are mathematically equal, regardless of thier underlying data representation.
     * @param term the terminal to compare against.
     * @return true if \a this is considered mathematically equal to \a term, false else
     */
    virtual bool mathsequals(const OGTerminal::Ptr& term) const;

    /**
     * Checks if two data containers are mathematically equal, regardless of thier underlying data representation.
     * @param term the terminal to compare against.
     * @param maxabserror the maximum absolute error to use during comparison
     * @param maxrelerror the maximum relative error to use during comparison
     * @return true if \a this is considered mathematically equal to \a term, false else
     */
    virtual bool mathsequals(const OGTerminal::Ptr& term, real8 maxabserror, real8 maxrelerror)const;
    
    virtual bool operator==(const OGTerminal::Ptr&) const;
    virtual bool operator!=(const OGTerminal::Ptr&) const;
    virtual bool operator%(const OGTerminal::Ptr&) const;
    virtual detail::FuzzyCompareOGTerminalContainer operator~(void) const;
    virtual bool operator==(const detail::FuzzyCompareOGTerminalContainer&) const;
    virtual bool operator!=(const detail::FuzzyCompareOGTerminalContainer&) const;
    OGTerminal();
    virtual ~OGTerminal();
  protected:
    ConvertTo _converter;
};

/**
 * Things that extend OGScalar
 */

template <typename T>
class OGScalar: public OGTerminal
{
  public:
    /**
     * Pointer type.
     */
    typedef std::shared_ptr<const OGScalar<T>> Ptr;
    static OGScalar<T>::Ptr create(T data);
    virtual size_t getRows() const override;
    virtual size_t getCols() const override;
    virtual size_t getDatalen() const override;
    T getValue() const;
    T ** toArrayOfArrays() const;
    virtual bool equals(const OGTerminal::Ptr&) const override;
    virtual bool fuzzyequals(const OGTerminal::Ptr& term)const override;
    virtual bool fuzzyequals(const OGTerminal::Ptr&, real8 maxabserror, real8 maxrelerror) const override;
    virtual void debug_print() const override;
    /*
     * The following will throw.
     */
    virtual std::shared_ptr<const OGRealDenseMatrix> asFullOGRealDenseMatrix() const override;
    virtual std::shared_ptr<const OGComplexMatrix> asFullOGComplexMatrix() const override;
    virtual OGTerminal::Ptr createOwningCopy() const override;
    virtual OGTerminal::Ptr createComplexOwningCopy() const override;
    virtual OGNumeric::Ptr copy() const override;
  protected:
    OGScalar(T data);
    T _value;
};

extern template class OGScalar<real8>;
extern template class OGScalar<complex16>;
extern template class OGScalar<int4>;

class OGRealScalar: public OGScalar<real8>
{
  public:
    /**
     * Pointer type.
     */
    typedef std::shared_ptr<const OGRealScalar> Ptr;
    static OGRealScalar::Ptr create(real8 data);
    virtual real8 ** toReal8ArrayOfArrays() const override;
    virtual complex16 ** toComplex16ArrayOfArrays() const override;
    virtual OGNumeric::Ptr copy() const override;
    virtual OGRealScalar::Ptr asOGRealScalar() const override;
    virtual void debug_print() const override;
    virtual ExprType_t getType() const override;
    virtual std::shared_ptr<const OGRealDenseMatrix> asFullOGRealDenseMatrix() const override;
    virtual std::shared_ptr<const OGComplexMatrix> asFullOGComplexMatrix() const override;
    virtual OGTerminal::Ptr createOwningCopy() const override;
    virtual OGTerminal::Ptr createComplexOwningCopy() const override;
  protected:
    OGRealScalar(real8 data);
};


class OGComplexScalar: public OGScalar<complex16>
{
  public:
    /**
     * Pointer type.
     */
    typedef std::shared_ptr<const OGComplexScalar> Ptr;
    static OGComplexScalar::Ptr create(complex16 data);
    virtual complex16 ** toComplex16ArrayOfArrays() const override;
    virtual OGNumeric::Ptr copy() const override;
    virtual OGComplexScalar::Ptr asOGComplexScalar() const override;
    virtual void debug_print() const override;
    virtual ExprType_t getType() const override;
    virtual std::shared_ptr<const OGRealDenseMatrix> asFullOGRealDenseMatrix() const override;
    virtual std::shared_ptr<const OGComplexMatrix> asFullOGComplexMatrix() const override;
    virtual OGTerminal::Ptr createOwningCopy() const override;
    virtual OGTerminal::Ptr createComplexOwningCopy() const override;
  protected:
    OGComplexScalar(complex16 data);
};

class OGIntegerScalar: public OGScalar<int4>
{
  public:
    /**
     * Pointer type.
     */
    typedef std::shared_ptr<const OGIntegerScalar> Ptr;
    static OGIntegerScalar::Ptr create(int4 data);
    virtual OGNumeric::Ptr copy() const override;
    virtual OGIntegerScalar::Ptr asOGIntegerScalar() const override;
    virtual void debug_print() const override;
    virtual ExprType_t getType() const override;
    virtual std::shared_ptr<const OGRealDenseMatrix> asFullOGRealDenseMatrix() const override;
    virtual std::shared_ptr<const OGComplexMatrix> asFullOGComplexMatrix() const override;
    virtual OGTerminal::Ptr createOwningCopy() const override;
    virtual OGTerminal::Ptr createComplexOwningCopy() const override;
  protected:
    OGIntegerScalar(int4 data);
};


template <typename T> class OGArray: public OGTerminal
{
  public:
    virtual ~OGArray() override;
    T * getData() const; // Returns a pointer to the underlying data
    T * toArray() const; // Returns a pointer to a copy of the underlying data
    virtual size_t getRows() const override;
    virtual size_t getCols() const override;
    virtual size_t getDatalen() const override;
    virtual DATA_ACCESS getDataAccess() const;
    virtual bool equals(const OGTerminal::Ptr&) const override;
    virtual bool fuzzyequals(const OGTerminal::Ptr& term)const override;
    virtual bool fuzzyequals(const OGTerminal::Ptr&, real8 maxabserror, real8 maxrelerror) const override;
    /*
     * The following will throw.
     */
    virtual std::shared_ptr<const OGRealDenseMatrix> asFullOGRealDenseMatrix() const override;
    virtual std::shared_ptr<const OGComplexMatrix> asFullOGComplexMatrix() const override;
    virtual OGTerminal::Ptr createOwningCopy() const override;
    virtual OGTerminal::Ptr createComplexOwningCopy() const override;
    virtual void debug_print() const override;
    virtual OGNumeric::Ptr copy() const override;
  protected:
    void setData(T * data);
    void setRows(size_t rows);
    void setCols(size_t cols);
    void setDatalen(size_t datalen);
    void setDataAccess(DATA_ACCESS access_spec);
    bool fundamentalsEqual(const OGTerminal::Ptr&) const;
    bool fundamentalsEqual(const OGTerminal*) const;
  private:
    T * _data = nullptr;
    size_t _rows  = 0;
    size_t _cols  = 0;
    size_t _datalen = 0;
    DATA_ACCESS _data_access = VIEWER;
};

/**
 * Things that extend OGMatrix
 */

template <typename T> class OGMatrix: public OGArray<T>
{
  public:
    typedef std::shared_ptr<const OGMatrix<T>> Ptr;
    static OGMatrix<T>::Ptr create(T* data, size_t rows, size_t cols, DATA_ACCESS access_spec=VIEWER);
    virtual void debug_print() const override;
    virtual OGNumeric::Ptr copy() const override;
    virtual OGTerminal::Ptr createOwningCopy() const override;
    virtual std::shared_ptr<const OGComplexMatrix> asFullOGComplexMatrix() const override;
    virtual real8 ** toReal8ArrayOfArrays() const override;
    virtual complex16 ** toComplex16ArrayOfArrays() const override;
    virtual ExprType_t getType() const override;
    T** toArrayOfArrays() const;
  protected:
    OGMatrix(T * data, size_t rows, size_t cols, DATA_ACCESS access_spec=VIEWER);
};

template<> real8 ** OGMatrix<real8>::toReal8ArrayOfArrays() const;
template<> real8 ** OGMatrix<complex16>::toReal8ArrayOfArrays() const;

template<> ExprType_t OGMatrix<real8>::getType() const ;
template<> ExprType_t OGMatrix<complex16>::getType() const ;

extern template class OGMatrix<real8>;
extern template class OGMatrix<complex16>;

class OGRealDenseMatrix: public OGMatrix<real8>
{
  public:
    /**
     * Pointer type.
     */
    typedef std::shared_ptr<const OGRealDenseMatrix> Ptr;
    static OGRealDenseMatrix::Ptr create(real8* data, size_t rows, size_t cols, DATA_ACCESS access_spec=VIEWER);
    virtual OGNumeric::Ptr copy() const override;
    virtual OGRealDenseMatrix::Ptr asOGRealDenseMatrix() const override;
    virtual ExprType_t getType() const override;
    virtual std::shared_ptr<const OGRealDenseMatrix> asFullOGRealDenseMatrix() const override;
    virtual std::shared_ptr<const OGComplexMatrix> asFullOGComplexMatrix() const override;
    virtual OGTerminal::Ptr createOwningCopy() const override;
    virtual OGTerminal::Ptr createComplexOwningCopy() const override;
  protected:
    using OGMatrix::OGMatrix;
};


class OGComplexMatrix: public OGMatrix<complex16>
{
  public:
    /**
     * Pointer type.
     */
    typedef std::shared_ptr<const OGComplexMatrix> Ptr;
    static OGComplexMatrix::Ptr create(complex16* data, size_t rows, size_t cols, DATA_ACCESS access_spec=VIEWER);
    virtual complex16 ** toComplex16ArrayOfArrays() const override;
    virtual OGNumeric::Ptr copy() const override;
    virtual OGComplexMatrix::Ptr asOGComplexMatrix() const override;
    virtual ExprType_t getType() const override;
    virtual std::shared_ptr<const OGRealDenseMatrix> asFullOGRealDenseMatrix() const override;
    virtual std::shared_ptr<const OGComplexMatrix> asFullOGComplexMatrix() const override;
    virtual OGTerminal::Ptr createOwningCopy() const override;
    virtual OGTerminal::Ptr createComplexOwningCopy() const override;
  protected:
    using OGMatrix::OGMatrix;
};


class OGLogicalMatrix: public OGRealDenseMatrix
{
  public:
    /**
     * Pointer type.
     */
    typedef std::shared_ptr<const OGLogicalMatrix> Ptr;
    // TODO: range limit inputs
    static OGLogicalMatrix::Ptr create(real8* data, size_t rows, size_t cols, DATA_ACCESS access_spec=VIEWER);
    virtual OGLogicalMatrix::Ptr asOGLogicalMatrix() const override;
  protected:
    using OGRealDenseMatrix::OGRealDenseMatrix;
};

/**
 * Things that extend OGDiagonalMatrix
 */

template <typename T> class OGDiagonalMatrix: public OGArray<T>
{
  public:
    typedef std::shared_ptr<const OGDiagonalMatrix<T>> Ptr;
    static OGDiagonalMatrix<T>::Ptr create(T* data, size_t rows, size_t cols, DATA_ACCESS access_spec=VIEWER);
    T** toArrayOfArrays() const;
  protected:
    OGDiagonalMatrix(T * data, size_t rows, size_t cols, DATA_ACCESS access_spec=VIEWER);
};

extern template class OGDiagonalMatrix<real8>;
extern template class OGDiagonalMatrix<complex16>;

class OGRealDiagonalMatrix: public OGDiagonalMatrix<real8>
{
  public:
    /**
     * Pointer type.
     */
    typedef std::shared_ptr<const OGRealDiagonalMatrix> Ptr;
    static OGRealDiagonalMatrix::Ptr create(real8* data, size_t rows, size_t cols, DATA_ACCESS access_spec=VIEWER);
    virtual void debug_print() const override;
    virtual real8* toReal8Array() const override;
    virtual real8** toReal8ArrayOfArrays() const override;
    virtual OGNumeric::Ptr copy() const override;
    virtual OGRealDiagonalMatrix::Ptr asOGRealDiagonalMatrix() const override;
    virtual ExprType_t getType() const override;
    virtual std::shared_ptr<const OGRealDenseMatrix> asFullOGRealDenseMatrix() const override;
    virtual std::shared_ptr<const OGComplexMatrix> asFullOGComplexMatrix() const override;
    virtual OGTerminal::Ptr createOwningCopy() const override;
    virtual OGTerminal::Ptr createComplexOwningCopy() const override;
  protected:
    using OGDiagonalMatrix::OGDiagonalMatrix;
};


class OGComplexDiagonalMatrix: public OGDiagonalMatrix<complex16>
{
  public:
    /**
     * Pointer type.
     */
    typedef std::shared_ptr<const OGComplexDiagonalMatrix> Ptr;
    static OGComplexDiagonalMatrix::Ptr create(complex16* data, size_t rows, size_t cols, DATA_ACCESS access_spec=VIEWER);
    virtual void debug_print() const override;
    virtual complex16* toComplex16Array() const override;
    virtual complex16** toComplex16ArrayOfArrays() const override;
    virtual OGNumeric::Ptr copy() const override;
    virtual OGComplexDiagonalMatrix::Ptr asOGComplexDiagonalMatrix() const override;
    virtual ExprType_t getType() const override;
    virtual std::shared_ptr<const OGRealDenseMatrix> asFullOGRealDenseMatrix() const override;
    virtual std::shared_ptr<const OGComplexMatrix> asFullOGComplexMatrix() const override;
    virtual OGTerminal::Ptr createOwningCopy() const override;
    virtual OGTerminal::Ptr createComplexOwningCopy() const override;
  protected:
    using OGDiagonalMatrix::OGDiagonalMatrix;
};


/**
 * Things that extend OGSparseMatrix
 */

template <typename T> class OGSparseMatrix: public OGArray<T>
{
  public:
    typedef std::shared_ptr<const OGSparseMatrix<T>> Ptr;
    static OGSparseMatrix<T>::Ptr create(int4* colPtr, int4* rowIdx, T* data, size_t rows, size_t cols, DATA_ACCESS access_spec=VIEWER);
    virtual ~OGSparseMatrix();
    int4* getColPtr() const;
    int4* getRowIdx() const;
    T** toArrayOfArrays() const;
    virtual bool equals(const OGTerminal::Ptr&) const override; // override OGArray equals to add in calls to check colPtr and rowIdx
    virtual bool fuzzyequals(const OGTerminal::Ptr& term) const override;
    virtual bool fuzzyequals(const OGTerminal::Ptr&, real8 maxabserror, real8 maxrelerror) const override;
  protected:
    void setColPtr(int4 * colPtr);
    void setRowIdx(int4 * rowIdx);
    OGSparseMatrix(int4 * colPtr, int4 * rowIdx, T * data, size_t rows, size_t cols, DATA_ACCESS access_spec=VIEWER);
  private:
    int4* _colPtr = nullptr; // the column pointer index
    int4* _rowIdx = nullptr; // the row index
};

extern template class OGSparseMatrix<real8>;
extern template class OGSparseMatrix<complex16>;

class OGRealSparseMatrix: public OGSparseMatrix<real8>
{
  public:
    /**
     * Pointer type.
     */
    typedef std::shared_ptr<const OGRealSparseMatrix> Ptr;
    static OGRealSparseMatrix::Ptr create(int4* colPtr, int4* rowIdx, real8* data, size_t rows, size_t cols, DATA_ACCESS access_spec=VIEWER);
    virtual void debug_print() const override;
    virtual real8* toReal8Array() const override;
    virtual real8** toReal8ArrayOfArrays() const override;
    virtual OGNumeric::Ptr copy() const override;
    virtual OGRealSparseMatrix::Ptr asOGRealSparseMatrix() const override;
    virtual ExprType_t getType() const override;
    virtual std::shared_ptr<const OGRealDenseMatrix> asFullOGRealDenseMatrix() const override;
    virtual std::shared_ptr<const OGComplexMatrix> asFullOGComplexMatrix() const override;
    virtual OGTerminal::Ptr createOwningCopy() const override;
    virtual OGTerminal::Ptr createComplexOwningCopy() const override;
  protected:
    using OGSparseMatrix::OGSparseMatrix;
};


class OGComplexSparseMatrix: public OGSparseMatrix<complex16>
{
  public:
    /**
     * Pointer type.
     */
    typedef std::shared_ptr<const OGComplexSparseMatrix> Ptr;
    static OGComplexSparseMatrix::Ptr create(int4* colPtr, int4* rowIdx, complex16* data, size_t rows, size_t cols, DATA_ACCESS access_spec=VIEWER);
    virtual void debug_print() const override;
    virtual complex16* toComplex16Array() const override;
    virtual complex16** toComplex16ArrayOfArrays() const override;
    virtual OGNumeric::Ptr copy() const override;
    virtual OGComplexSparseMatrix::Ptr asOGComplexSparseMatrix() const override;
    virtual ExprType_t getType() const override;
    virtual std::shared_ptr<const OGRealDenseMatrix> asFullOGRealDenseMatrix() const override;
    virtual std::shared_ptr<const OGComplexMatrix> asFullOGComplexMatrix() const override;
    virtual OGTerminal::Ptr createOwningCopy() const override;
    virtual OGTerminal::Ptr createComplexOwningCopy() const override;
  protected:
    using OGSparseMatrix::OGSparseMatrix;
};

/**
 * Creates a non-templated OGMatrix object based on the type of data \a T.
 * e.g. creates an OGRealDenseMatrix from real8 type \a data.
 * @param data the data from which an OGMatrix shall be constructed.
 * @param rows the number of rows in the matrix.
 * @param cols the number of columns in the matrix.
 * @return a non-templated OGMatrix object.
 */
template<typename T>
OGNumeric::Ptr makeConcreteDenseMatrix(T * data, size_t rows, size_t cols, DATA_ACCESS access);
// PTS
template<>
OGNumeric::Ptr makeConcreteDenseMatrix(real8 * data, size_t rows, size_t cols, DATA_ACCESS access);
template<>
OGNumeric::Ptr makeConcreteDenseMatrix(complex16 * data, size_t rows, size_t cols, DATA_ACCESS access);

/**
 * Creates a non-templated OGScalar object based on the type of data \a T.
 * e.g. creates an OGRealScalar from a real8 type \a data.
 * @param data the data from which an OGScalar shall be constructed.
 * @return a non-templated OGScalar object.
 */
template<typename T>
OGNumeric::Ptr makeConcreteScalar(T data);
// PTS
template<>
OGNumeric::Ptr  makeConcreteScalar(real8 data);
template<>
OGNumeric::Ptr  makeConcreteScalar(complex16 data);


} // end namespace librdag

#endif // _TERMINAL_HH
