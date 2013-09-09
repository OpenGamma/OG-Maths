/**
 * Copyright (C) 2012 - present by OpenGamma Inc. and the OpenGamma group of companies
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

using namespace std;


/*
 * The namespace for the DAG library
 */
namespace librdag
{
  
/*
 * General execption thing
 */
class librdagException: public exception
{
    public:
    virtual const char* what() const throw()
    {
      return "Exception occurred.";
    }
};

/*
 * Base class for absolutely everything!
 */
class OGNumeric
{
  public:
    OGNumeric();
    virtual ~OGNumeric();
    virtual void debug_print();
    virtual void accept(Visitor &v)=0;
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



/**
 *  Expr type
 */
class OGExpr: public OGNumeric
{
  public:
    OGExpr();
    explicit OGExpr(OGExpr& copy);
    OGExpr(const OGNumeric * const args, const int nargs);
    virtual ~OGExpr();

    OGExpr& operator=(OGExpr& rhs);

    std::vector<OGNumeric *> * getArgs();

    size_t getNArgs();
    virtual void debug_print();
    void accept(Visitor &v);
  private:
    std::vector<OGNumeric *> * _args = NULL;
  protected:
    void setArgs(std::vector<OGNumeric *> * args);
};

/**
 * Things that extend OGExpr
 */

class OGBinaryExpr : virtual public OGExpr
{
  public:
	  OGBinaryExpr();
	  OGBinaryExpr(OGNumeric* left, OGNumeric* right);
};

class COPY: public OGExpr
{
  public:
	  COPY();
    void debug_print();
};


class PLUS: public OGBinaryExpr
{
  public:
	  PLUS();
	  PLUS(OGNumeric* left, OGNumeric* right);
    void debug_print();
};


class MINUS: public OGBinaryExpr
{
  public:
    MINUS();
	  MINUS(OGNumeric* left, OGNumeric* right);
    void debug_print();
};

class SVD: public OGExpr
{
  public:
    SVD();
    void debug_print();
};

class SELECTRESULT: public OGExpr
{
  public:
    SELECTRESULT();
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
    OGScalar() {};

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
    OGMatrix() {};
    OGMatrix(T * data, int rows, int cols)
    {
      this->_datalen = rows*cols;
      T * tmpdata =new T [this->getDatalen()];
      memcpy(tmpdata, data, sizeof(T)*this->_datalen);
      this->_data=tmpdata;
      this->_rows=rows;
      this->_cols=cols;
    };
    ~OGMatrix()
    {
      if(this->getData())
      {
        delete(this->getData());
      }
    }
    void noCopy_ctor(T * data, int rows, int cols)
    {
      this->setData(data);
      this->setRows(rows);
      this->setCols(cols);
      this->setDatalen(rows*cols);
    };
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
};

class OGRealMatrix: public OGMatrix<real16>
{
  public:
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
    OGDiagonalMatrix(const OGDiagonalMatrix& copy)  // do we want deep copy?
    {
      // deep for now
      this->_rows = copy._rows;
      this->_cols = copy._cols;
      this->_datalen = this->_rows>this->_cols?this->_cols:this->_rows;
      T * tmpdata=new T [this->_datalen];
      memcpy(tmpdata, copy._data, sizeof(T)*this->_datalen);
      this->setData(tmpdata);
    }
    OGDiagonalMatrix() {};
    OGDiagonalMatrix(T * data, int rows, int cols)
    {
      this->_datalen = rows>cols?cols:rows;
      T * tmpdata = new T [this->_datalen];
      memcpy(tmpdata, data, sizeof(T)*this->_datalen);
      this->setData(tmpdata);
      this->_rows = rows;
      this->_cols = cols;
    };
    ~OGDiagonalMatrix()
    {
      delete(this->_data);
    }
    void noCopy_ctor(T * data, int rows, int cols)
    {
      this->setData(data);
      this->_rows = rows;
      this->_cols = cols;
      this->_datalen = rows>cols?cols:rows;
    };
    void accept(Visitor &v)
    {
      v.visit(this);
    };
  private:
};

class OGRealDiagonalMatrix: public OGDiagonalMatrix<real16>
{
  public:
    void debug_print()
    {
      size_t ptr=0;
      printf("\n");
      for(int i = 0 ; i < this->getRows(); i++)
      {
        for(int j = 0 ; j < this->getCols()-1; j++)
        {
          if(i==j)
          {
            printf("%6.4f, ",this->getData()[ptr++]);
          }
        }
        if(i==this->getCols())
        {
          printf("%6.4f\n",this->getData()[ptr++]);
        }
      }
    }
};

class OGComplexDiagonalMatrix: public OGDiagonalMatrix<complex16>
{
  public:
    void debug_print()
    {
      size_t ptr=0;
      printf("\n");
      for(int i = 0 ; i < this->getRows(); i++)
      {
        for(int j = 0 ; j < this->getCols()-1; j++)
        {
          if(i==j)
          {
            printf("%6.4f + %6.4fi, ",this->getData()[ptr].real(),this->getData()[ptr].imag());
            ptr++;
          }
        }
        if(i==this->getCols())
        {
          printf("%6.4f + %6.4fi\n",this->getData()[ptr].real(),this->getData()[ptr].imag());
          ptr++;
        }
      }
    }
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
    OGSparseMatrix() {};
    OGSparseMatrix(int * colPtr, int * rowIdx, T * data, int rows, int cols)
    {
      this->_datalen = colPtr[cols+1];

      T * tmpdata = new T [this->_datalen];
      memcpy(this->tmpdata, data, sizeof(T)*this->_datalen);
      this->setData(tmpdata);

      this->_colPtr = new int [cols+1];
      memcpy(this->_colPtr, colPtr, sizeof(int)*(this->cols+1));

      this->_rowIdx = new int [this->_datalen];
      memcpy(this->_rowIdx, rowIdx, sizeof(int)*this->_datalen);

      this->_rows = rows;
      this->_cols = cols;
    };
    ~OGSparseMatrix()
    {
      delete(this->getData());
      delete(this->_colPtr);
      delete(this->_rowIdx);
    }
    void noCopy_ctor(int * colPtr, int * rowIdx, T * data, int rows, int cols)
    {
      this->setData(data);
      this->setRows(rows);
      this->setCols(cols);
      this->setDatalen(colPtr[cols]);
      this->setColPtr(colPtr);
      this->setRowIdx(rowIdx);
    };
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
};

class OGRealSparseMatrix: public OGSparseMatrix<real16>
{
  public:
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

}

#endif
