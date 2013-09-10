#include "com_opengamma_longdog_materialisers_Materialisers.h"
#include "entrypt.h"
#include "jshim.h"
#include "expression.hh"
#include "exprtypeenum.h"
#include "warningmacros.h"
#include <complex.h>
#include <stdio.h>
#include <stdexcept>
#include <stdlib.h>

namespace convert
{
using namespace librdag;

/*
 * General execption thing
 */
class convertException: public exception
{
    virtual const char* what() const throw()
    {
      return "Exception occurred.";
    }
} convertExcept;


/**
 * helper function to get ints from int getFOO() in java
 */
jint getIntFromVoidJMethod(jmethodID id, jobject obj)
{
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMcache->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
#ifdef _DEBUG
    printf("Thread attach failed\n");
#endif
    exit(1);
  }
  jint data = 0x7ff00000;
  data = env->CallIntMethod(obj, id);
  return data;
}

/*
 * Template signatures
 */

/*
 * Binds some OGArray data in an OGArray object obj to a type T (T extends double)
 */
template <typename T> T * bindOGArrayData(jobject obj);
/*
 * UnBinds some OGArray data of type "T" (T extends double) from an OGArray object "obj"
 */
template <typename T> void unbindOGArrayData(T * nativeData, jobject obj);
/*
 * Binds primitive array data of type "T" from an OGArray object "obj" accessed by getter method "method"
 */
template <typename T, typename S> T * bindPrimitiveArrayData(jobject obj, jmethodID method);
/*
 * UnBinds primitive array data "nativeData" of type "T" from an OGArray object "obj" accessed by getter method "method"
 */
template <typename T, typename S> void unbindPrimitiveArrayData(T * nativeData, jobject obj, jmethodID method);

/*
 * An OGRealMatrix backed by data pinned from a Java based OGRealMatrix
 */
class JOGRealMatrix: public OGRealMatrix
{
  public:
    JOGRealMatrix(jobject * obj)
    {
      this->_backingObject = obj;
      real16 * _dataptr = bindPrimitiveArrayData<real16, jdoubleArray>(*obj, OGTerminalClazz_getData);
      this->noCopy_ctor(_dataptr,getIntFromVoidJMethod(OGArrayClazz_getRows, *obj),getIntFromVoidJMethod(OGArrayClazz_getCols, *obj));
    };
    ~JOGRealMatrix()
    {
      unbindOGArrayData<real16>(this->getData(), *_backingObject);
    };
    void debug_print()
    {
      printf("\nJava bound OGRealMatrix\n");
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
  private:
    jobject * _backingObject = NULL;
};

/*
 * An OGComplexMatrix backed by data pinned from a Java based OGComplexMatrix
 */
class JOGComplexMatrix: public OGComplexMatrix
{
  public:
    JOGComplexMatrix(jobject * obj)
    {
      this->_backingObject = obj;
      complex16 * _dataptr = bindPrimitiveArrayData<complex16, jdoubleArray>(*obj,OGTerminalClazz_getData);
      this->noCopy_ctor(_dataptr,getIntFromVoidJMethod(OGArrayClazz_getRows, *obj),getIntFromVoidJMethod(OGArrayClazz_getCols, *obj));
    };
    ~JOGComplexMatrix()
    {
      unbindOGArrayData<complex16>(this->getData(), *_backingObject);
    };
    void debug_print()
    {
      printf("\nJava bound OGComplexMatrix\n");
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
  private:
    jobject * _backingObject = NULL;
};

/*
 * An OGRealSparseMatrix backed by data pinned from a Java based OGRealSparseMatrix
 */
class JOGRealSparseMatrix: public OGRealSparseMatrix
{
  public:
    JOGRealSparseMatrix(jobject * obj)
    {
      this->_backingObject = obj;
      real16 * _dataptr = bindPrimitiveArrayData<real16, jdoubleArray>(*obj, OGTerminalClazz_getData);
      int * _colPtr = bindPrimitiveArrayData<int, jintArray>(*obj, OGSparseMatrixClazz_getColPtr);
      int * _rowIdx = bindPrimitiveArrayData<int, jintArray>(*obj, OGSparseMatrixClazz_getRowIdx);
      this->noCopy_ctor(_colPtr, _rowIdx, _dataptr,getIntFromVoidJMethod(OGArrayClazz_getRows, *obj),getIntFromVoidJMethod(OGArrayClazz_getCols, *obj));
    };
    ~JOGRealSparseMatrix()
    {
      unbindPrimitiveArrayData<real16, jdoubleArray>(this->getData(), *_backingObject, OGTerminalClazz_getData);
      unbindPrimitiveArrayData<int, jintArray>(this->getColPtr(), *_backingObject, OGSparseMatrixClazz_getColPtr);
      unbindPrimitiveArrayData<int, jintArray>(this->getRowIdx(), *_backingObject, OGSparseMatrixClazz_getRowIdx);
    };
    void debug_print()
    {
      double nnz = 100.e0 * this->getDatalen() / (this->getRows() * this->getCols());
      printf("\nJava bound OGRealSparseMatrix\n");
      printf("\ndata len = %d\n",this->getDatalen());
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
    real16 ** toReal16ArrayOfArrays() override {
      double ** foo = NULL;
      printf("returning null as no impl yet!!!!\n");
      return foo;
    }   
    complex16 ** toComplex16ArrayOfArrays() override {
      complex16 ** foo = NULL;
      printf("returning null as no impl yet!!!!\n");
      return foo;
    }        
  private:
    jobject * _backingObject = NULL;
};

/*
 * An OGComplexSparseMatrix backed by data pinned from a Java based OGComplexSparseMatrix
 */
class JOGComplexSparseMatrix: public OGComplexSparseMatrix
{
  public:
    JOGComplexSparseMatrix(jobject * obj)
    {
      this->_backingObject = obj;
      complex16 * _dataptr = bindPrimitiveArrayData<complex16, jdoubleArray>(*obj, OGTerminalClazz_getData);
      int * _colPtr = bindPrimitiveArrayData<int, jintArray>(*obj, OGSparseMatrixClazz_getColPtr);
      int * _rowIdx = bindPrimitiveArrayData<int, jintArray>(*obj, OGSparseMatrixClazz_getRowIdx);
      this->noCopy_ctor(_colPtr, _rowIdx, _dataptr,getIntFromVoidJMethod(OGArrayClazz_getRows, *obj),getIntFromVoidJMethod(OGArrayClazz_getCols, *obj));
    };
    ~JOGComplexSparseMatrix()
    {
      unbindPrimitiveArrayData<complex16, jdoubleArray>(this->getData(), *_backingObject, OGTerminalClazz_getData);
      unbindPrimitiveArrayData<int, jintArray>(this->getColPtr(), *_backingObject, OGSparseMatrixClazz_getColPtr);
      unbindPrimitiveArrayData<int, jintArray>(this->getRowIdx(), *_backingObject, OGSparseMatrixClazz_getRowIdx);
    };
    void debug_print()
    {
      double nnz = 100.e0 * this->getDatalen()/ (this->getRows() * this->getCols());
      printf("\nJava bound OGComplexSparseMatrix\n");
      printf("datalen:%d\n",this->getDatalen());
      printf("rows:%d\n",this->getRows());
      printf("cols:%d\n",this->getCols());
      printf("[nnz density = %4.2f. rows = %d, columns = %d]\n", nnz, this->getRows(), this->getCols());
      int * colPtr = this->getColPtr();
      for (int ir = 0; ir < this->getCols(); ir++)
      {
        for (int i = colPtr[ir]; i < colPtr[1]; i++)
        {
          printf("(%d,%d) = %6.4f + %6.4fi \n",this->getRowIdx()[i],ir,this->getData()[i].real(),this->getData()[i].imag());
        }
      }
    }
    real16 ** toReal16ArrayOfArrays() override {
      double ** foo = NULL;
      printf("returning null as no impl yet!!!!\n");
      return foo;
    }  
    complex16 ** toComplex16ArrayOfArrays() override {
      complex16 ** foo = NULL;
      printf("returning null as no impl yet!!!!\n");
      return foo;
    }        
  private:
    jobject * _backingObject = NULL;
};


/*
 * An OGRealDiagonalMatrix backed by data pinned from a Java based OGRealDiagonalMatrix
 */
class JOGRealDiagonalMatrix: public OGRealDiagonalMatrix
{
  public:
    JOGRealDiagonalMatrix(jobject * obj)
    {
      this->_backingObject = obj;
      real16 * _dataptr = bindPrimitiveArrayData<real16, jdoubleArray>(*obj, OGTerminalClazz_getData);
      this->noCopy_ctor(_dataptr,getIntFromVoidJMethod(OGArrayClazz_getRows, *obj),getIntFromVoidJMethod(OGArrayClazz_getCols, *obj));
    };
    ~JOGRealDiagonalMatrix()
    {
      unbindOGArrayData<real16>(this->getData(), *_backingObject);
    };
    void debug_print()
    {
      printf("\nJava bound JOGRealDiagonalMatrix\n");
      OGRealDiagonalMatrix::debug_print();
    }
  private:
    jobject * _backingObject = NULL;
};

/*
 * An OGComplexDiagonalMatrix backed by data pinned from a Java based OGComplexDiagonalMatrix
 */
class JOGComplexDiagonalMatrix: public OGComplexDiagonalMatrix
{
  public:
    JOGComplexDiagonalMatrix(jobject * obj)
    {
      this->_backingObject = obj;
      complex16 * _dataptr = bindPrimitiveArrayData<complex16, jdoubleArray>(*obj,OGTerminalClazz_getData);
      this->noCopy_ctor(_dataptr,getIntFromVoidJMethod(OGArrayClazz_getRows, *obj),getIntFromVoidJMethod(OGArrayClazz_getCols, *obj));
    };
    ~JOGComplexDiagonalMatrix()
    {
      unbindOGArrayData<complex16>(this->getData(), *_backingObject);
    };
    void debug_print()
    {
      printf("\nJava bound JOGComplexDiagonalMatrix\n");
      OGComplexDiagonalMatrix::debug_print();
    }      
  private:
    jobject * _backingObject = NULL;
};

/**
 * binds the data in an OGArray class to a T pointer
 * @param T the class of the underlying data
 * @param S the class representation in the pinning call
 * @param obj the object from which the data shall be extracted
 * @param method the method to "get" the native data reference
 */
template <typename T, typename S> T * bindPrimitiveArrayData(jobject obj, jmethodID method)
{
  if(!obj)
  {
#ifdef _DEBUG
    printf("bindPrimitiveArrayData: null obj\n");
#endif
    exit(1);
  }
  if(!method)
  {
#ifdef _DEBUG
    printf("bindPrimitiveArrayData: null method\n");
#endif
    exit(1);
  }
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMcache->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
#ifdef _DEBUG
    printf("Thread attach failed\n");
#endif
    exit(1);
  }
  jobject dataobj = NULL;
  dataobj = env->CallObjectMethod(obj, method);
  if(!dataobj)
  {
#ifdef _DEBUG
    printf("CallObjectMethod failed\n");
#endif
    exit(1);
  }
  S * array = reinterpret_cast<S *>(&dataobj);
  T * _dataptr = (T *) env->GetPrimitiveArrayCritical(*array,NULL);
  return _dataptr;
}

/**
 * free (unbind) the data in an OGArray class from a T pointer
 * @param T the class of the nativeData
 * @param S the class representation in the pinning call
 * @param nativeData the native data to unbind
 * @param obj the java object from which the native data is pinned
 * @param method the method that refers to the pinned data
 */
template <typename T, typename S> void unbindPrimitiveArrayData(T * nativeData, jobject obj, jmethodID method)
{
  if(!nativeData)
  {
#ifdef _DEBUG
    printf("unbindPrimitiveArrayData: null nativeData\n");
#endif
    exit(1);
  }
  if(!obj)
  {
#ifdef _DEBUG
    printf("unbindPrimitiveArrayData: null obj\n");
#endif
    exit(1);
  }
  if(!method)
  {
#ifdef _DEBUG
    printf("unbindPrimitiveArrayData: null method\n");
#endif
    exit(1);
  }
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMcache->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
#ifdef _DEBUG
    printf("Thread attach failed\n");
#endif
    exit(1);
  }
  jobject dataobj = env->CallObjectMethod(obj, method);
  S * array = reinterpret_cast<S *>(&dataobj);
  env->ReleasePrimitiveArrayCritical(*array, (void *)nativeData, 0);
}

/**
 * free (unbind) the data in an OGArray class from a T pointer
 * @param T the class of the nativeData
 * @param nativeData the native data to unbind
 * @param obj the java object from which the native data is pinned
 */
template <typename T> void unbindOGArrayData(T * nativeData, jobject obj)
{
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMcache->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
#ifdef _DEBUG
    printf("Thread attach failed\n");
#endif
    exit(1);
  }
  jobject dataobj = env->CallObjectMethod(obj, OGTerminalClazz_getData);
  jdoubleArray * array = reinterpret_cast<jdoubleArray *>(&dataobj);
  env->ReleasePrimitiveArrayCritical(*array, (void *)nativeData, 0);
}

/**
 * Generates expression nodes from a java object
 */
class ExprFactory
{
  public:
    ExprFactory() {};
    librdag::OGNumeric * getExpr(jobject obj)
    {
      printf("In exprfactory\n");
#ifdef _DEBUG
      printf("vm ptr at 0x%llx\n",JVMcache);
#endif
      JNIEnv *env=NULL;
      jint jStatus = 0;
      jStatus=JVMcache->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
      if(jStatus)
      {
#ifdef _DEBUG
        printf("Thread attach failed\n");
#endif
        exit(1);
      }
      if(!OGNumericClazz_getType)
      {
#ifdef _DEBUG
        printf("Clazz Type is null, 0x%llx\n",OGNumericClazz_getType);
#endif
        exit(1);
      }
      jobject typeobj = env->CallObjectMethod(obj, OGNumericClazz_getType);
      jlong ID = env->GetLongField(typeobj,OGExprTypeEnumClazz__hashdefined);
#ifdef __MINGW32__
      unsigned int high, low;
      low =   (long long unsigned int)ID & 0x00000000FFFFFFFFULL;
      high = ((long long unsigned int)ID & 0xFFFFFFFF00000000ULL) >> 32;
      printf("Clazz Type is hashdefined as %x%x\n", high, low);
#else
      printf("Clazz Type is hashdefined as %lld\n",(long long int)ID);
#endif

      librdag::OGNumeric * _expr = nullptr;

      switch(ID)
      {
      case OGREALMATRIX_ENUM:
      {
        printf("Binding a JOGRealMatrix\n");
        _expr = new JOGRealMatrix(&obj);
      }
      break;
      case OGCOMPLEXMATRIX_ENUM:
      {
        printf("Binding a JOGComplexMatrix\n");
        _expr = new JOGComplexMatrix(&obj);
      }
      break;
      case OGREALSPARSEMATRIX_ENUM:
      {
        printf("Binding a JOGRealSparseMatrix\n");
        _expr = new JOGRealSparseMatrix(&obj);
      }
      break;
      case OGCOMPLEXSPARSEMATRIX_ENUM:
      {
        printf("Binding a JOGComplexSparseMatrix\n");
        _expr = new JOGComplexSparseMatrix(&obj);
      }
      break;
      case OGREALDIAGONALMATRIX_ENUM:
      {
        printf("Binding a JOGRealDiagonalMatrix\n");
        _expr = new JOGRealDiagonalMatrix(&obj);
      }
      break;
      case OGCOMPLEXDIAGONALMATRIX_ENUM:
      {
        printf("Binding a JOGComplexDiagonalMatrix\n");
        _expr = new JOGComplexDiagonalMatrix(&obj);
      }
      break;      
      case COPY_ENUM:
      {
        printf("COPY function\n");
        _expr = new COPY(generateArgs(&obj));
      }
      break;
      case MINUS_ENUM:
      {
        printf("MINUS function\n");
        _expr = new MINUS(generateArgs(&obj));
      }
      break;
      case PLUS_ENUM:
      {
        printf("PLUS function\n");
        _expr = new PLUS(generateArgs(&obj));
      }
      break;
      case SVD_ENUM:
      {
        printf("SVD function\n");
        _expr = new SVD(generateArgs(&obj));
      }
      break;
      case SELECTRESULT_ENUM:
      {
        printf("Select Result function\n");
        _expr = new SELECTRESULT(generateArgs(&obj));
      }
      break;
      default:
      {
        throw convertExcept;
      }
      break;
      }

      if(_expr == nullptr)
      {
        printf("_expr is NULL so hasn't been set by the factory, exiting\n");
        exit(1);
      }

      printf("Returning from exprfactory\n");
      return _expr;
    };
  private:
    ArgContainer* generateArgs(jobject *obj);
};


ArgContainer*
ExprFactory::generateArgs(jobject *obj)
{
  // get object array
  jmethodID method = OGExprClazz_getExprs;
  if(!method)
  {
#ifdef _DEBUG
    printf("JOGExpr: null method\n");
#endif
    exit(1);
  }
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMcache->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
#ifdef _DEBUG
    printf("Thread attach failed\n");
#endif
    exit(1);
  }
  jobject dataobj = NULL;
  dataobj = env->CallObjectMethod(*obj, method);
  if(!dataobj)
  {
#ifdef _DEBUG
    printf("CallObjectMethod failed\n");
#endif
    exit(1);
  }
  jobjectArray * args = reinterpret_cast<jobjectArray *>(&dataobj);
  jsize len = env->GetArrayLength((jarray)*args);
#ifdef _DEBUG
  printf("JOGExpr arg size is %d\n",len);
#endif
  ArgContainer* local_args = new ArgContainer();
  for(int i=0; i<len; i++)
  {

    jobject tmp = (jobject) env->GetObjectArrayElement(*args, i);
    local_args->push_back(getExpr(tmp));
  }
  return local_args;
}

/**
 * C shim to instantiate java tree as c++ tree
 */
#ifdef __cplusplus
extern "C"
#endif
void * instantiateJClassAsCXXClass(jobject obj)
{
  printf("In converter\n");
  convert::ExprFactory * factory = new convert::ExprFactory();
  return factory->getExpr(obj);
}

class DispatchToReal16ArrayOfArrays: public librdag::Visitor
{
  public:
    
    DispatchToReal16ArrayOfArrays(){}
    ~DispatchToReal16ArrayOfArrays()
    {
      real16 ** arr = this->getData();
      if(arr)
      {

        for(int i=0;i<this->getRows();i++)
        {
          delete arr[i];
        }
        delete arr;         
      }
    }
    void visit(librdag::OGExpr SUPPRESS_UNUSED *thing)
    {
        throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGExpr)");
    };
    void visit(librdag::OGArray<real16> *thing)
    {
      this->setData(thing->toReal16ArrayOfArrays());
      this->setRows(thing->getRows());
      this->setCols(thing->getCols());      
    }
    void visit(librdag::OGArray<complex16> SUPPRESS_UNUSED *thing)
    {
        throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGArray<complex16>)");
    }
    void visit(librdag::OGMatrix<real16> *thing)
    {
      this->setData(thing->toReal16ArrayOfArrays());
      this->setRows(thing->getRows());
      this->setCols(thing->getCols());    
    }
    void visit(librdag::OGMatrix<complex16> SUPPRESS_UNUSED *thing)
    {
        throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGMatrix<complex16>)");
    }    
    void visit(librdag::OGScalar<real16> *thing)
    {
      this->setData(thing->toReal16ArrayOfArrays());
      this->setRows(1);
      this->setCols(1);    
    }
    void visit(librdag::OGScalar<complex16> SUPPRESS_UNUSED *thing)
    {
        throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGScalar<complex16>)");
    }
    void visit(librdag::OGScalar<int> SUPPRESS_UNUSED *thing)
    {
        throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGScalar<int>)");
    }
    void setData(real16 ** data)
    {
      this->_data = data;
    }
    void setRows(int rows)
    {
     this->rows = rows; 
    }
    void setCols(int cols)
    {
     this->cols = cols;
    }        
    real16 ** getData()
    {
      return this->_data;
    }
    int getRows()
    {
     return this->rows; 
    }
    int getCols()
    {
     return this->cols; 
    }    
  private:
    real16 ** _data = NULL;
    int rows;
    int cols;
};

class DispatchToComplex16ArrayOfArrays: public librdag::Visitor
{
  public:
    
    DispatchToComplex16ArrayOfArrays(){}
    ~DispatchToComplex16ArrayOfArrays()
    {
      complex16 ** arr = this->getData();
      if(arr)
      {

        for(int i=0;i<this->getRows();i++)
        {
          delete arr[i];
        }
        delete arr;         
      }
    }
    void visit(librdag::OGExpr SUPPRESS_UNUSED *thing)
    {
        throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGExpr)");
    };
    void visit(librdag::OGArray<complex16> *thing)
    {
      printf("Visitor: librdag::OGArray<complex16> branch\n");
      this->setData(thing->toComplex16ArrayOfArrays());
      this->setRows(thing->getRows());
      this->setCols(thing->getCols());      
    }
    void visit(librdag::OGArray<real16> SUPPRESS_UNUSED *thing)
    {
        throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGArray<real16>)");
    }
    void visit(librdag::OGMatrix<complex16> *thing)
    {
      printf("Visitor: librdag::OGMatrix<complex16> branch\n");      
      this->setData(thing->toComplex16ArrayOfArrays());
      this->setRows(thing->getRows());
      this->setCols(thing->getCols());    
    }
    void visit(librdag::OGMatrix<real16> SUPPRESS_UNUSED *thing)
    {
        throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGMatrix<real16>)");
    }    
    void visit(librdag::OGScalar<complex16> *thing)
    {
      this->setData(thing->toComplex16ArrayOfArrays());
      this->setRows(1);
      this->setCols(1);    
    }
    void visit(librdag::OGScalar<real16> SUPPRESS_UNUSED *thing)
    {
        throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGScalar<real16>)");
    }
    void visit(librdag::OGScalar<int> SUPPRESS_UNUSED *thing)
    {
        throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGScalar<int>)");
    }
    void setData(complex16 ** data)
    {
      this->_data = data;
    }
    void setRows(int rows)
    {
     this->rows = rows; 
    }
    void setCols(int cols)
    {
     this->cols = cols;
    }        
    complex16 ** getData()
    {
      return this->_data;
    }
    int getRows()
    {
     return this->rows; 
    }
    int getCols()
    {
     return this->cols; 
    }    
  private:
    complex16 ** _data = NULL;
    int rows;
    int cols;
};


  /**
   * deletes an array of arrays of type T
   */
  template <typename T> void deleteArrOfArr(T ** buf, int lda)
  {
    for(int i=0;i<lda;i++)
    {
      delete buf[i];
    }
    delete buf;    
  }

} // namespace ends

#ifdef __cplusplus
extern "C" {
#endif
 
//  
//
// JNI methods and helpers
//
//  
  
  /*
   * Converts a real16 ** to a java double[][]
   * @param env, the JNI environment pointer
   * @param inputData, the real16 array of arrays to convert
   * @param rows the number of rows in the array
   * @param cols the number of columns in the array
   * @return a jobjectArray which is a double[][] equivalent of {@code inputData}
   */
  jobjectArray convertCreal16ArrOfArr2JDoubleArrOfArr(JNIEnv * env, real16 ** inputData, int rows, int cols)
  {
    jobjectArray returnVal = env->NewObjectArray(rows, BigDDoubleArrayClazz, NULL);
    if(!returnVal)
    {
#ifdef _DEBUG
      printf("Allocation of jobjectArray failed.\n");
#endif
      exit(1);
    }
    for(int i = 0; i < rows; i++)
    {
      jdoubleArray tmp = NULL;
      tmp = env->NewDoubleArray(cols);
      if(!tmp)
      {
#ifdef _DEBUG
        printf("Allocation of jobjectArray failed.\n");
#endif
        exit(1);
      }
      env->SetDoubleArrayRegion(tmp, 0, cols, inputData[i]);
      env->SetObjectArrayElement(returnVal, i, tmp);
    }
    return returnVal;
  }



  /**
   * 
   */
  jobjectArray extractRealPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(JNIEnv * env, SUPPRESS_UNUSED complex16 ** inputData, int rows, int cols) {
    jobjectArray returnVal = env->NewObjectArray(rows, BigDDoubleArrayClazz, NULL);
    if(!returnVal)
    {
#ifdef _DEBUG
      printf("Allocation of jobjectArray failed.\n");
#endif
      exit(1);
    }
    real16 * aRow = new real16[cols];
    for(int i = 0; i < rows; i++)
    {
      jdoubleArray tmp = NULL;
      tmp = env->NewDoubleArray(cols);
      if(!tmp)
      {
#ifdef _DEBUG
        printf("Allocation of jobjectArray failed.\n");
#endif
        exit(1);
      }
      for(int j = 0; j < cols; j++)
      {        
        aRow[j]=std::real(inputData[i][j]);
      }
      env->SetDoubleArrayRegion(tmp, 0, cols, aRow);
      env->SetObjectArrayElement(returnVal, i, tmp);
    }
    delete aRow;
    return returnVal;
  }
 
   /**
   * 
   */
  jobjectArray extractImagPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(JNIEnv * env, SUPPRESS_UNUSED complex16 ** inputData, int rows, int cols) {
    jobjectArray returnVal = env->NewObjectArray(rows, BigDDoubleArrayClazz, NULL);
    if(!returnVal)
    {
#ifdef _DEBUG
      printf("Allocation of jobjectArray failed.\n");
#endif
      exit(1);
    }
    real16 * aRow = new real16[cols];
    for(int i = 0; i < rows; i++)
    {
      jdoubleArray tmp = NULL;
      tmp = env->NewDoubleArray(cols);
      if(!tmp)
      {
#ifdef _DEBUG
        printf("Allocation of jobjectArray failed.\n");
#endif
        exit(1);
      }
      for(int j = 0; j < cols; j++)
      {
        aRow[j]=std::imag(inputData[i][j]);
      }
      env->SetDoubleArrayRegion(tmp, 0, cols, aRow);
      env->SetObjectArrayElement(returnVal, i, tmp);
    }
    delete aRow;
    return returnVal;
  }
 
  /*
   * Class:     com_opengamma_longdog_materialisers_Materialisers
   * Method:    materialiseToJDoubleArrayOfArrays
   * Signature: (Lcom/opengamma/longdog/datacontainers/OGNumeric;)[[D
   */
  JNIEXPORT jobjectArray JNICALL Java_com_opengamma_longdog_materialisers_Materialisers_materialiseToJDoubleArrayOfArrays
  (JNIEnv SUPPRESS_UNUSED *env, jclass SUPPRESS_UNUSED clazz, jobject SUPPRESS_UNUSED obj)
  {
    printf("Entering materialise function\n");

    printf("Calling convert::instantiateJClassAsCXXClass\n");
    // convert obj to OGNumeric objs
    librdag::OGNumeric * chain = (librdag::OGNumeric *) convert::instantiateJClassAsCXXClass(obj);

    printf("Calling entrypt function\n");
    librdag::OGTerminal * answer =  (librdag::OGTerminal *) entrypt((struct c_OGNumeric *) chain);
    
    convert::DispatchToReal16ArrayOfArrays * visitor = new convert::DispatchToReal16ArrayOfArrays();
    answer->accept(*visitor);
    real16 ** buf = visitor->getData();
    int rows = visitor->getRows();;
    int cols = visitor->getCols();
   
    jobjectArray returnVal = convertCreal16ArrOfArr2JDoubleArrOfArr(env, buf, rows, cols);

    delete visitor;
    
    printf("Returning\n");    
    return returnVal;
  }

  /*
   * Class:     com_opengamma_longdog_materialisers_Materialisers
   * Method:    materialiseToJComplexArrayContainer
   * Signature: (Lcom/opengamma/longdog/datacontainers/OGNumeric;)Lcom/opengamma/longdog/datacontainers/other/ComplexArrayContainer;
   */
  JNIEXPORT jobject JNICALL Java_com_opengamma_longdog_materialisers_Materialisers_materialiseToJComplexArrayContainer
  (JNIEnv SUPPRESS_UNUSED *env, jclass SUPPRESS_UNUSED clazz, jobject SUPPRESS_UNUSED obj)
  {
    printf("materialiseToJComplexArrayContainer\n");
    printf("Entering materialise function\n");

    printf("Calling convert::instantiateJClassAsCXXClass\n");
    // convert obj to OGNumeric objs
    librdag::OGNumeric * chain = (librdag::OGNumeric *) convert::instantiateJClassAsCXXClass(obj);

    printf("Calling entrypt function\n");
    librdag::OGTerminal * answer =  (librdag::OGTerminal *) entrypt((struct c_OGNumeric *) chain);
    
    convert::DispatchToComplex16ArrayOfArrays * visitor = new convert::DispatchToComplex16ArrayOfArrays();
    answer->accept(*visitor);
    complex16 ** buf = visitor->getData();
    int rows = visitor->getRows();
    int cols = visitor->getCols(); 
       
    jobjectArray realPart = extractRealPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(env, buf, rows, cols);
    jobjectArray complexPart = extractImagPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(env, buf, rows, cols);

    jobject returnVal = env->NewObject(ComplexArrayContainerClazz,ComplexArrayContainerClazz_ctor_DAoA_DAoA, realPart, complexPart);
    
    delete visitor;
    
    printf("Returning\n");    
    return returnVal;
  }

  /*
   * Class:     com_opengamma_longdog_materialisers_Materialisers
   * Method:    materialiseToJBoolean
   * Signature: (Lcom/opengamma/longdog/datacontainers/OGNumeric;)Z
   */
  JNIEXPORT jboolean JNICALL Java_com_opengamma_longdog_materialisers_Materialisers_materialiseToJBoolean
  (JNIEnv SUPPRESS_UNUSED *env, jclass SUPPRESS_UNUSED clazz, jobject SUPPRESS_UNUSED obj)
  {
    jboolean returnVal = 0;
    return returnVal;
  }


#ifdef __cplusplus
}
#endif
