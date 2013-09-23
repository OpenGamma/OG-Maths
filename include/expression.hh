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
    virtual real16 ** toReal16ArrayOfArrays() const = 0;
    virtual complex16 ** toComplex16ArrayOfArrays() const = 0;
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
    void setArgs(ArgContainer* args);
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
    virtual OGNumeric* copy() const;
    virtual const COPY* asCOPY() const;
    virtual void debug_print() const override;
};


class PLUS: public OGBinaryExpr
{
  public:
    PLUS(ArgContainer *args);
    virtual OGNumeric* copy() const;
    virtual const PLUS* asPLUS() const;
    virtual void debug_print() const override;
};


class MINUS: public OGBinaryExpr
{
  public:
    MINUS(ArgContainer *args);
    virtual OGNumeric* copy() const;
    virtual const MINUS* asMINUS() const;
    virtual void debug_print() const override;
};

class SVD: public OGUnaryExpr
{
  public:
    SVD(ArgContainer *args);
    virtual OGNumeric* copy() const;
    virtual const SVD* asSVD() const;
    virtual void debug_print() const override;
};

class SELECTRESULT: public OGBinaryExpr
{
  public:
    SELECTRESULT(ArgContainer *args);
    virtual OGNumeric* copy() const;
    virtual const SELECTRESULT* asSELECTRESULT() const;
    virtual void debug_print() const override;
};

/**
 * Things that extend OGScalar
 */
template <class T> class OGScalar: public OGTerminal
{
  private:
    T _value;
  public:
    OGScalar(T data)
    {
      this->_value=data;
    };

    virtual void accept(Visitor &v) const
    {
      v.visit(this);
    };

    T getValue() const
    {
      return this->_value;
    };

    T ** toArrayOfArrays() const
    {
      T ** tmp = new T * [1];
      tmp[0] = new T[1];
      tmp[0][0] = this->getValue();
      return tmp;
    };
};


class OGRealScalar: public OGScalar<real16>
{
  public:
    OGRealScalar(real16 data): OGScalar<real16>(data) {};
    real16 ** toReal16ArrayOfArrays() const override
    {
      return this->toArrayOfArrays();
    }
    complex16 ** toComplex16ArrayOfArrays() const override
    {
      throw new librdagException();
    }
    virtual OGNumeric* copy() const
    {
      return new OGRealScalar(this->getValue());
    }
    virtual const OGRealScalar* asOGRealScalar() const
    {
      return this;
    }
    virtual void debug_print() const
    {
      printf("OGRealScalar(%f)\n", this->getValue());
    }
};

class OGComplexScalar: public OGScalar<complex16>
{
  public:
    OGComplexScalar(complex16 data): OGScalar<complex16>(data) {};
    real16 ** toReal16ArrayOfArrays() const override
    {
      throw new librdagException();
    }
    complex16 ** toComplex16ArrayOfArrays() const override
    {
      return this->toArrayOfArrays();
    }
    virtual OGNumeric* copy() const
    {
      return new OGComplexScalar(this->getValue());
    }
    virtual const OGComplexScalar* asOGComplexScalar() const
    {
      return this;
    }
    virtual void debug_print() const
    {
      printf("OGComplexScalar(%f+%fi)\n", this->getValue().real(), this->getValue().imag());
    }
};

class OGIntegerScalar: public OGScalar<int>
{
  public:
    OGIntegerScalar(int data): OGScalar<int>(data) {};
    real16 ** toReal16ArrayOfArrays() const override
    {
      throw new librdagException();
    }
    complex16 ** toComplex16ArrayOfArrays() const override
    {
      throw new librdagException();
    }
    virtual OGNumeric* copy() const
    {
      return new OGIntegerScalar(this->getValue());
    }
    virtual const OGIntegerScalar* asOGIntegerScalar() const
    {
      return this;
    }
    virtual void debug_print() const
    {
      printf("OGComplexScalar(%d)\n", this->getValue());
    }
};

template <typename T> class OGArray: public OGTerminal
{
  public:
    virtual ~OGArray() {};
    void accept(Visitor &v)
    {
      v.visit(this);
    };
    T * getData() const
    {
      return _data;
    }
    int getRows() const
    {
      return _rows;
    }
    int getCols() const
    {
      return _cols;
    }
    int getDatalen() const
    {
      return _datalen;
    }
  protected:
    void setData(T * data)
    {
      _data = data;
    }
    void setRows(int rows)
    {
      _rows = rows;
    }
    void setCols(int cols)
    {
      _cols = cols;
    }
    void setDatalen(int datalen)
    {
      _datalen = datalen;
    }
  private:
    T * _data = nullptr;
    int _rows  = 0;
    int _cols  = 0;
    int _datalen = 0;
};

/**
 * Things what extend OGMatrix
 */
template <typename T> class OGMatrix: public OGArray<T>
{
  public:
    OGMatrix(T * data, int rows, int cols)
    {
      this->setData(data);
      this->setRows(rows);
      this->setCols(cols);
      this->setDatalen(rows*cols);
    };
    ~OGMatrix() {};
    void accept(Visitor &v) const
    {
      v.visit(this);
    };
    T ** toArrayOfArrays() const
    {
      int const rows = this->getRows();
      int const cols = this->getCols();
      T * const data = this->getData();
      T ** tmp = new T * [rows];
      for(int i=0; i < rows; i++)
      {
        tmp[i] = new T [cols];
        for(int j = 0; j < cols; j++)
        {
          tmp[i][j] = data[j*rows+i];
        }
      }
      return tmp;
    }
};

class OGRealMatrix: public OGMatrix<real16>
{
  public:
    using OGMatrix::OGMatrix;
    virtual void debug_print() const
    {
      size_t ptr=0;
      printf("\n");
      for(int i = 0 ; i < this->getRows(); i++)
      {
        for(int j = 0 ; j < this->getCols()-1; j++)
        {
          printf("%6.4f, ",this->getData()[ptr++]);
        }
        printf("%6.4f\n",this->getData()[ptr++]);
      }
    }
    real16 ** toReal16ArrayOfArrays() const override
    {
      return this->toArrayOfArrays();
    };
    complex16 ** toComplex16ArrayOfArrays() const override
    {
      throw new librdagException;
    };
    virtual OGNumeric* copy() const
    {
      return new OGRealMatrix(this->getData(), this->getRows(), this->getCols());
    }
    virtual const OGRealMatrix* asOGRealMatrix() const
    {
      return this;
    }
};


class OGComplexMatrix: public OGMatrix<complex16>
{
  public:
    using OGMatrix::OGMatrix;
    virtual void debug_print() const
    {
      size_t ptr=0;
      printf("\n");
      for(int i = 0 ; i < this->getRows(); i++)
      {
        for(int j = 0 ; j < this->getCols()-1; j++)
        {
          printf("%6.4f + %6.4fi, ",this->getData()[ptr].real(),this->getData()[ptr].imag());
          ptr++;
        }
        printf("%6.4f + %6.4fi\n",this->getData()[ptr].real(),this->getData()[ptr].imag());
        ptr++;
      }
    }
    real16 ** toReal16ArrayOfArrays() const override
    {
      printf("throwing exception\n");
      throw new librdagException;
    };
    complex16 ** toComplex16ArrayOfArrays() const override
    {
      printf("returning toArrayOfArrays\n");
      return this->toArrayOfArrays();
    };
    virtual OGNumeric* copy() const
    {
      return new OGComplexMatrix(this->getData(), this->getRows(), this->getCols());
    }
    virtual const OGComplexMatrix* asOGComplexMatrix() const
    {
      return this;
    }
};

class OGLogicalMatrix: public OGRealMatrix
{

};


/**
 * Things what extend OGDiagonalMatrix
 */
template <typename T> class OGDiagonalMatrix: public OGArray<T>
{
  public:
    OGDiagonalMatrix(T * data, int rows, int cols)
    {
      this->setData(data);
      this->setRows(rows);
      this->setCols(cols);
      this->setDatalen(rows>cols?cols:rows);
    };
    ~OGDiagonalMatrix() {};
    virtual void accept(Visitor &v) const
    {
      v.visit(this);
    };
    T ** toArrayOfArrays() const
    {
      int const rows = this->getRows();
      int const cols = this->getCols();
      T * const data = this->getData();
      int const datalen = this->getDatalen();
      T ** tmp = new T * [rows];
      for(int i=0; i < rows; i++)
      {
        tmp[i] = new T [cols];
        for(int j=0; j < cols; j++)
        {
          if(i == j && i < datalen)
          {
            tmp[i][j] = data[i];
          }
          else
          {
            tmp[i][j] = 0.e0;
          }
        }
      }
      return tmp;
    };
  private:
    OGDiagonalMatrix() = delete;
};

class OGRealDiagonalMatrix: public OGDiagonalMatrix<real16>
{
  public:
    using OGDiagonalMatrix::OGDiagonalMatrix;
    virtual void debug_print() const
    {
      size_t ptr=0;
      printf("\n");
      for(int i = 0 ; i < this->getRows(); i++)
      {
        for(int j = 0 ; j < this->getCols()-1; j++)
        {
          if(j==i && i < this->getDatalen())
          {
            printf("%6.4f, ",this->getData()[ptr++]);
          }
          else
          {
            printf("%6.4f, ",0.e0);
          }
        }
        if(i==this->getCols()-1 && i < this->getDatalen())
        {
          printf("%6.4f\n",this->getData()[ptr++]);
        }
        else
        {
          printf("%6.4f\n",0.e0);
        }
      }
    }
    real16 ** toReal16ArrayOfArrays() const override
    {
      printf("returning toArrayOfArrays\n");
      return this->toArrayOfArrays();
    };
    complex16 ** toComplex16ArrayOfArrays() const override
    {
      printf("throwing exception\n");
      throw new librdagException;
    };
    virtual OGNumeric* copy() const
    {
      return new OGRealDiagonalMatrix(this->getData(), this->getRows(), this->getCols());
    }
    virtual const OGRealDiagonalMatrix* asOGRealDiagonalMatrix() const
    {
      return this;
    }
};

class OGComplexDiagonalMatrix: public OGDiagonalMatrix<complex16>
{
  public:
    using OGDiagonalMatrix::OGDiagonalMatrix;
    virtual void debug_print() const
    {
      size_t ptr=0;
      printf("\n");
      for(int i = 0 ; i < this->getRows(); i++)
      {
        for(int j = 0 ; j < this->getCols()-1; j++)
        {
          if(j==i && i < this->getDatalen())
          {
            printf("%6.4f + %6.4fi, ",this->getData()[ptr].real(),this->getData()[ptr].imag());
            ptr++;
          }
          else
          {
            printf("%6.4f + %6.4fi, ",0.e0,0.e0);
          }
        }
        if(i==this->getCols()-1 && i < this->getDatalen())
        {
          printf("%6.4f + %6.4fi\n",this->getData()[ptr].real(),this->getData()[ptr].imag());
          ptr++;
        }
        else
        {
          printf("%6.4f + %6.4fi\n",0.e0,0.e0);
        }
      }
    }
    real16 ** toReal16ArrayOfArrays() const override
    {
      printf("throwing exception\n");
      throw new librdagException;
    };
    complex16 ** toComplex16ArrayOfArrays() const override
    {
      return this->toArrayOfArrays();
    };
    virtual OGNumeric* copy() const
    {
      return new OGComplexDiagonalMatrix(this->getData(), this->getRows(), this->getCols());
    }
    virtual const OGComplexDiagonalMatrix* asOGComplexDiagonalMatrix() const
    {
      return this;
    }
};

/**
 * Things what extend OGSparseMatrix
 */
template <typename T> class OGSparseMatrix: public OGArray<T>
{
  public:
    OGSparseMatrix(int * colPtr, int * rowIdx, T * data, int rows, int cols)
    {
      this->setData(data);
      this->setRows(rows);
      this->setCols(cols);
      this->setDatalen(colPtr[cols]);
      this->setColPtr(colPtr);
      this->setRowIdx(rowIdx);
    };
    ~OGSparseMatrix() {};
    void accept(Visitor &v) const
    {
      v.visit(this);
    };
    /**
     * gets the column pointers
     */
    int * getColPtr() const
    {
      return _colPtr;
    }
    /**
     * gets the row indexes
     */
    int * getRowIdx() const
    {
      return _rowIdx;
    }
    /**
     * FIXME: Not yet implemented.
     */
    virtual real16** toReal16ArrayOfArrays() const
    {
      throw new librdagException();
    }
    virtual complex16** toComplex16ArrayOfArrays() const
    {
      throw new librdagException();
    }
  protected:
    /**
     * sets the column pointers
     */
    void setColPtr(int * colPtr)
    {
      _colPtr = colPtr;
    }
    /**
     * sets the row indexes
     */
    void setRowIdx(int * rowIdx)
    {
      _rowIdx = rowIdx;
    }
  private:
    int * _colPtr = nullptr; // the column pointer index
    int * _rowIdx = nullptr; // the row index
    OGSparseMatrix()=delete;
};

class OGRealSparseMatrix: public OGSparseMatrix<real16>
{
  public:
    using OGSparseMatrix::OGSparseMatrix;
    virtual void debug_print() const
    {
      double nnz = 100.e0 * this->getDatalen() / (this->getRows() * this->getCols());
      printf("\nOGRealSparseMatrix\n");
      printf("[nnz density = %4.2f. rows = %d, columns = %d]\n", nnz, this->getRows(), this->getCols());
      int * colPtr = this->getColPtr();
      for (int ir = 0; ir < this->getCols(); ir++)
      {
        for (int i = colPtr[ir]; i < colPtr[ir + 1]; i++)
        {
          printf("(%d,%d) = %6.4f\n",this->getRowIdx()[i],ir,this->getData()[i]);
        }
      }
    }
    virtual OGNumeric* copy() const
    {
      return new OGRealSparseMatrix(this->getColPtr(), this->getRowIdx(), this->getData(),
                                    this->getRows(), this->getCols());
    }
    virtual const OGRealSparseMatrix* asOGRealSparseMatrix() const
    {
      return this;
    }
};

class OGComplexSparseMatrix: public OGSparseMatrix<complex16>
{
  public:
    using OGSparseMatrix::OGSparseMatrix;
    virtual void debug_print() const
    {
      double nnz = 100.e0 * this->getDatalen() / (double)(this->getRows() * this->getCols());
      printf("\nOGComplexSparseMatrix\n");
      printf("[nnz density = %4.2f. rows = %d, columns = %d]\n", nnz, this->getRows(), this->getCols());
      int * colPtr = this->getColPtr();
      for (int ir = 0; ir < this->getCols(); ir++)
      {
        for (int i = colPtr[ir]; i < colPtr[ir + 1]; i++)
        {
          printf("(%d,%d) = %6.4f + %6.4fi \n",this->getRowIdx()[i],ir,this->getData()[i].real(),this->getData()[i].imag());
        }
      }
    }
    virtual OGNumeric* copy() const
    {
      return new OGComplexSparseMatrix(this->getColPtr(), this->getRowIdx(), this->getData(),
                                       this->getRows(), this->getCols());
    }
    virtual const OGComplexSparseMatrix* asOGComplexSparseMatrix() const
    {
      return this;
    }
};

} // namespace librdag

#endif
