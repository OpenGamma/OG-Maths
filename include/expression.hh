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

class OGRealScalar;
class OGComplexScalar;
class OGIntegerScalar;

/*
 * Base class for absolutely everything!
 */
class OGNumeric
{
  public:
    virtual ~OGNumeric();
    virtual void debug_print();
    virtual void accept(Visitor &v)=0;
    virtual const OGRealScalar* asOGRealScalar() const;
    virtual const OGComplexScalar* asOGComplexScalar() const;
    virtual const OGIntegerScalar* asOGIntegerScalar() const;
  protected:
    OGNumeric();
};

/* 
 * Base class for terminal nodes in the AST
 */
class OGTerminal: public OGNumeric
{
  public:
    virtual real16 ** toReal16ArrayOfArrays() = 0;
    virtual complex16 ** toComplex16ArrayOfArrays() = 0;
};

/*
 * Container for expression arguments
 */
typedef PtrVector<OGNumeric> ArgContainer;

/**
 *  Expr type
 */
class OGExpr: public OGNumeric, private Uncopyable
{
  public:
    virtual ~OGExpr();
    ArgContainer* getArgs();
    size_t getNArgs();
    virtual void debug_print();
    void accept(Visitor &v);
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
    void debug_print();
};


class PLUS: public OGBinaryExpr
{
  public:
    PLUS(ArgContainer *args);
    void debug_print();
};


class MINUS: public OGBinaryExpr
{
  public:
    MINUS(ArgContainer *args);
    void debug_print();
};

class SVD: public OGUnaryExpr
{
  public:
    SVD(ArgContainer *args);
    void debug_print();
};

class SELECTRESULT: public OGBinaryExpr
{
  public:
    SELECTRESULT(ArgContainer *args);
    void debug_print();
};

/**
 * Things that extend OGScalar
 */
template <class T> class OGScalar: public OGTerminal
{
  private:
    T _value;
  public:
    OGScalar(const OGScalar * const copy)
    {
    	this->_value = copy->_value;
    }

    OGScalar(const OGScalar& copy)
    {
      this->_value= copy._value;
    }
    
    OGScalar(T data)
    {
      this->_value=data;
    };
    
    void accept(Visitor &v)
    {
      v.visit(this);
    };
    
    T getValue()
    {
      return this->_value;
    };
    
    T ** toArrayOfArrays()
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
    OGRealScalar(const OGRealScalar * const copy): OGScalar<real16>(copy){};
    OGRealScalar(const OGRealScalar& copy): OGScalar<real16>(copy){};
    OGRealScalar(real16 data): OGScalar<real16>(data){};
    real16 ** toReal16ArrayOfArrays() override
    {
      return this->toArrayOfArrays();
    }
    complex16 ** toComplex16ArrayOfArrays() override
    {
      throw new librdagException();
    }
    virtual const OGRealScalar* asOGRealScalar() const
    {
      return this;
    }
};

class OGComplexScalar: public OGScalar<complex16>
{
  public:
    OGComplexScalar(const OGComplexScalar * const copy): OGScalar<complex16>(copy){};
    OGComplexScalar(const OGComplexScalar& copy): OGScalar<complex16>(copy){};
    OGComplexScalar(complex16 data): OGScalar<complex16>(data){};    
    real16 ** toReal16ArrayOfArrays() override
    {
      throw new librdagException();
    }
    complex16 ** toComplex16ArrayOfArrays() override
    {
      return this->toArrayOfArrays();
    }
    virtual const OGComplexScalar* asOGComplexScalar() const
    {
      return this;
    }
};

class OGIntegerScalar: public OGScalar<int>
{
  public:
    OGIntegerScalar(const OGIntegerScalar * const copy): OGScalar<int>(copy){};
    OGIntegerScalar(const OGIntegerScalar& copy): OGScalar<int>(copy){};
    OGIntegerScalar(int data): OGScalar<int>(data){};
    real16 ** toReal16ArrayOfArrays() override
    {
      throw new librdagException();
    }
    complex16 ** toComplex16ArrayOfArrays() override
    {
      throw new librdagException();
    }
    virtual const OGIntegerScalar* asOGIntegerScalar() const
    {
      return this;
    }
};

template <typename T> class OGArray: public OGTerminal
{
  public:
    virtual ~OGArray()
    {
      delete _data;
    };
    void accept(Visitor &v)
    {
      v.visit(this);
    };
    T * getData()
    {
      return _data;
    }
    void setData(T * data)
    {
      _data = data;
    }
    int getRows()
    {
      return _rows;
    }
    void setRows(int rows)
    {
      _rows = rows;
    }
    int getCols()
    {
      return _cols;
    }
    void setCols(int cols)
    {
      _cols = cols;
    }
    int getDatalen()
    {
      return _datalen;
    }
    void setDatalen(int datalen)
    {
      _datalen = datalen;
    }
  private:
    T * _data = NULL;
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
    OGMatrix(const OGMatrix& copy)
    {
      this->setRows(copy->getRows());
      this->setCols(copy->getCols());
      this->setDatalen(copy->getDatalen());
      T * tmpdata = new T [this->getDatalen()];
      memcpy(tmpdata, copy._data, sizeof(T)*this->getDatalen());
      this->setData(tmpdata);
    }
    OGMatrix(T * data, int rows, int cols)
    {
      this->setData(data);
      this->setRows(rows);
      this->setCols(cols);
      this->setDatalen(rows*cols);
    };
    ~OGMatrix()
    {
      if(this->getData())
      {
        delete(this->getData());
      }
    }
    void accept(Visitor &v)
    {
      v.visit(this);
    };
    T ** toArrayOfArrays()
    {
      int const rows = this->getRows();
      int const cols = this->getCols();
      T * const data = this->getData();
      T ** tmp = new T * [rows];
      for(int i=0; i < rows; i++)
      {
        tmp[i] = new T [cols];
        for(int j = 0; j < cols; j++) {
          tmp[i][j] = data[j*rows+i];
        }
      }
      return tmp;
    }
  private:
    OGMatrix() = delete;
};

class OGRealMatrix: public OGMatrix<real16>
{
  public: 
    using OGMatrix::OGMatrix;
    void debug_print()
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
    real16 ** toReal16ArrayOfArrays() override
    {
      return this->toArrayOfArrays();
    };
    complex16 ** toComplex16ArrayOfArrays() override
    {
          throw new librdagException;
    };
};


class OGComplexMatrix: public OGMatrix<complex16>
{
  public:
    using OGMatrix::OGMatrix;
    void debug_print()
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
    real16 ** toReal16ArrayOfArrays() override
    {
      printf("throwing exception\n");
      throw new librdagException;
    };
    complex16 ** toComplex16ArrayOfArrays() override
    {
      printf("returning toArrayOfArrays\n");
      return this->toArrayOfArrays();
    };    
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
    OGDiagonalMatrix(const OGDiagonalMatrix& copy)
    {
      this->setRows(copy->getRows());
      this->setCols(copy->getCols());
      this->setDatalen(copy->getDatalen());
      T * tmpdata=new T [this->getDatalen()];
      memcpy(tmpdata, copy._data, sizeof(T)*this->getDatalen());
      this->setData(tmpdata);
    }
    OGDiagonalMatrix(T * data, int rows, int cols)
    {
      this->setData(data);
      this->setRows(rows);
      this->setCols(cols);
      this->setDatalen(rows>cols?cols:rows);
    };
    ~OGDiagonalMatrix()
    {
      delete(this->getData());
    }
    void accept(Visitor &v)
    {
      v.visit(this);
    };
    T ** toArrayOfArrays()
    {
      int const rows = this->getRows();
      int const cols = this->getCols();
      T * const data = this->getData();
      int const datalen = this->getDatalen();
      T ** tmp = new T * [rows];
      for(int i=0; i < rows; i++)
      {
        tmp[i] = new T [cols];
        for(int j=0; j < cols; j++) {
          if(i == j && i < datalen)
          {
            tmp[i][j] = data[i];
          }else{
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
    void debug_print()
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
        } else {
          printf("%6.4f\n",0.e0);
        }
      }
    }
    real16 ** toReal16ArrayOfArrays() override
    {
      printf("returning toArrayOfArrays\n");
      return this->toArrayOfArrays();
    };
    complex16 ** toComplex16ArrayOfArrays() override
    {
      printf("throwing exception\n");
      throw new librdagException;
    };     
};

class OGComplexDiagonalMatrix: public OGDiagonalMatrix<complex16>
{
  public:
    using OGDiagonalMatrix::OGDiagonalMatrix;
    void debug_print()
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
        } else {
          printf("%6.4f + %6.4fi\n",0.e0,0.e0);
        }
      }
    }
    real16 ** toReal16ArrayOfArrays() override
    {
      printf("throwing exception\n");
      throw new librdagException;
    };
    complex16 ** toComplex16ArrayOfArrays() override
    {
      return this->toArrayOfArrays();
    };     
};

/**
 * Things what extend OGSparseMatrix
 */
template <typename T> class OGSparseMatrix: public OGArray<T>
{
  public:
    OGSparseMatrix(const OGSparseMatrix& copy)
    {
      OGSparseMatrix(copy._colPtr, copy._rowIdx, copy._data, copy._rows, copy._cols);
    }
    OGSparseMatrix(int * colPtr, int * rowIdx, T * data, int rows, int cols)
    {
      this->setData(data);
      this->setRows(rows);
      this->setCols(cols);
      this->setDatalen(colPtr[cols]);
      this->setColPtr(colPtr);
      this->setRowIdx(rowIdx);
    };
    ~OGSparseMatrix()
    {
      delete(this->getData());
      delete(this->_colPtr);
      delete(this->_rowIdx);
    }
    void accept(Visitor &v)
    {
      v.visit(this);
    };
    /**
     * gets the column pointers
     */
    int * getColPtr()
    {
      return _colPtr;
    }
    /**
     * gets the row indexes
     */
    int * getRowIdx()
    {
      return _rowIdx;
    }
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
    int * _colPtr; // the column pointer index
    int * _rowIdx; // the row index
    OGSparseMatrix()=delete;
};

class OGRealSparseMatrix: public OGSparseMatrix<real16>
{
  public:
    using OGSparseMatrix::OGSparseMatrix;
    void debug_print()
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
};

class OGComplexSparseMatrix: public OGSparseMatrix<complex16>
{
  public:
    using OGSparseMatrix::OGSparseMatrix;
    void debug_print()
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
};

} // namespace librdag

#endif
