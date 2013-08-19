// bindings
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <complex>
#include <vector>
#include <memory>

using namespace std;

typedef complex<double> complex16;
typedef complex<float> complex8;
typedef double real16;
typedef float real8;

/*
 * The namespace for the
 */
namespace librdag
{

/*
 * Forward declare types for visitor class
 */
class OGExpr;
template  <typename T> class OGArray;
template  <typename T> class OGMatrix;
template  <typename T> class OGScalar;

/*
 * Pure function visitor class to access all fundamental types
 */
class Visitor
{
  public:
    virtual void visit(OGExpr *thing) = 0;
    virtual void visit(OGArray<real16> *thing) = 0;
    virtual void visit(OGArray<complex16> *thing) = 0;
    virtual void visit(OGMatrix<real16> *thing) = 0;
    virtual void visit(OGScalar<real16> *thing) = 0;
    virtual void visit(OGScalar<complex16> *thing) = 0;
};

/*
 * Base class for absolutely everything!
 */
class OGNumeric
{
  public:
    OGNumeric() {};
    ~OGNumeric()
    {
      printf("in OGNumeric destructor\n");
    };
    virtual void debug_print()
    {
      printf("Abstract OGNumeric type\n");
    }
    virtual void accept(Visitor &v)=0;
};

/**
 *  expr type
 */
class OGExpr: public OGNumeric
{
  public:
    OGExpr() {};
    explicit OGExpr(OGExpr& copy)
    {
      this->_args = new std::vector<OGNumeric *>(*copy.getArgs());
    }
    OGExpr& operator=(OGExpr& rhs)
    {
      rhs.setArgs(this->getArgs());
      return *this;
    }
    OGExpr(const librdag::OGNumeric * const args, const int nargs)
    {
      for(size_t i = 0; i < nargs; i++)
      {
        this->_args->push_back(const_cast<OGNumeric *> (&args[i]));
      }
    };
    virtual ~OGExpr()
    {
      for (std::vector<OGNumeric *>::iterator it = this->_args->begin() ; it != this->_args->end(); it++)
      {
        delete &it;
      }
      delete this->_args;
    };
    std::vector<OGNumeric *> * getArgs()
    {
      return this->_args;
    };
    // should replace this with a construct from vector
    void setArgs(std::vector<OGNumeric *> * args)
    {
      _args = args;
    };
    size_t getNArgs()
    {
      return this->_args->size();
    };
    void debug_print()
    {
      printf("OGExpr base class\n");
    }
    void accept(Visitor &v)
    {
      v.visit(this);
    };
  private:
    std::vector<OGNumeric *> * _args = NULL;
};

/**
 * Things what extend OGExpr
 */

class COPY: virtual public OGExpr
{
  public:
    COPY()
    {
    };
    void debug_print()
    {
      printf("COPY base class\n");
    }
};


class PLUS: virtual public OGExpr
{
  public:
    PLUS() {};
    void debug_print()
    {
      printf("PLUS base class\n");
    }
};


class MINUS: virtual public OGExpr
{
  public:
    MINUS() {};
    void debug_print()
    {
      printf("MINUS base class\n");
    }
};

/**
 * Things what extend OGScalar
 */
template <class T> class OGScalar: public OGNumeric
{
    T _value;
  public:
    OGScalar(const OGScalar& copy)
    {
      copy._value=_value;
    }
    OGScalar() {};
    OGScalar(T data)
    {
      this._value=data;
    };
    void accept(Visitor &v)
    {
      v.visit(this);
    };
};

class OGRealScalar: public OGScalar<real16>
{

};

class OGComplexScalar: public OGScalar<complex16>
{

};


template <typename T> class OGArray: public OGNumeric
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
    int setRows(int rows)
    {
      _rows = rows;
    }
    int getCols()
    {
      return _cols;
    }
    int setCols(int cols)
    {
      _cols = cols;
    }
    int getDatalen()
    {
      return _datalen;
    }
    int setDatalen(int datalen)
    {
      _datalen = datalen;
    }
  private:
    T * _data = NULL;
    int _rows  = 0;
    int _cols  = 0;
    int _datalen = 0;
};

// force instantiation
template class OGArray<real16>;
template class OGArray<complex16>;

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
      T * tmpdata =new T [this.getDatalen()];
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
  private:

};

class OGRealMatrix: public OGMatrix<real16>
{
  public:
    void debug_print()
    {
      size_t ptr=0;
      printf("\n");
      for(size_t i = 0 ; i < this->getRows(); i++)
      {
        for(size_t j = 0 ; j < this->getCols()-1; j++)
        {
          printf("%6.4f, ",this->getData()[ptr++]);
        }
        printf("%6.4f\n",this->getData()[ptr++]);
      }
    }
};


class OGComplexMatrix: public OGMatrix<complex16>
{
  public:
    void debug_print()
    {
      size_t ptr=0;
      printf("\n");
      for(size_t i = 0 ; i < this->getRows(); i++)
      {
        for(size_t j = 0 ; j < this->getCols()-1; j++)
        {
          printf("%6.4f + %6.4fi, ",this->getData()[ptr].real(),this->getData()[ptr].imag());
          ptr++;
        }
        printf("%6.4f + %6.4fi\n",this->getData()[ptr].real(),this->getData()[ptr].imag());
        ptr++;
      }
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
      for(size_t i = 0 ; i < this->getRows(); i++)
      {
        for(size_t j = 0 ; j < this->getCols()-1; j++)
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
      for(size_t i = 0 ; i < this->getRows(); i++)
      {
        for(size_t j = 0 ; j < this->getCols()-1; j++)
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
      printf("[nnz density = %4.2f\%. rows = %d, columns = %d]\n", nnz, this->getRows(), this->getCols());
      int * colPtr = this->getColPtr();
      for (size_t ir = 0; ir < this->getCols(); ir++)
      {
        for (size_t i = colPtr[ir]; i < colPtr[ir + 1]; i++)
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
      printf("[nnz density = %4.2f\%. rows = %d, columns = %d]\n", nnz, this->getRows(), this->getCols());
      int * colPtr = this->getColPtr();
      for (size_t ir = 0; ir < this->getCols(); ir++)
      {
        for (size_t i = colPtr[ir]; i < colPtr[ir + 1]; i++)
        {
          printf("(%d,%d) = %6.4f + %6.4fi \n",this->getRowIdx()[i],ir,this->getData()[i].real(),this->getData()[i].imag());
        }
      }
    }
};

}