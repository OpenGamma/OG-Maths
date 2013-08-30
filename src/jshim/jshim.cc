#include "com_opengamma_longdog_materialisers_Materialisers.h"
#include "entrypt.h"
#include "jshim.h"
#include "bindings.hh"
#include "exprtypeenum.h"
#include <stdio.h>
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
      for(size_t i = 0 ; i < this->getRows(); i++)
      {
        for(size_t j = 0 ; j < this->getCols()-1; j++)
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
      for(size_t i = 0 ; i < this->getRows(); i++)
      {
        for(size_t j = 0 ; j < this->getCols()-1; j++)
        {
          ptr++;
          printf("%6.4f + %6.4fi, ",this->getData()[ptr].real(),this->getData()[ptr].imag());
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
      printf("[nnz density = %4.2f\%. rows = %d, columns = %d]\n", nnz, this->getRows(), this->getCols());
      int * colPtr = this->getColPtr();
      for (size_t ir = 0; ir < this->getCols(); ir++)
      {
        for (size_t i = colPtr[ir]; i < colPtr[1]; i++)
        {
          printf("(%d,%d) = %6.4f + %6.4fi \n",this->getRowIdx()[i],ir,this->getData()[i].real(),this->getData()[i].imag());
        }
      }
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
};

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
};

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
};

/**
 * Spec for an OGExpr node derived from a java object
 */
class JOGExpr: virtual public OGExpr
{
  public:
    JOGExpr() {};
    JOGExpr(jobject * obj);
    ~JOGExpr();
  private:
    jobject * _backingObject = NULL;
};
/**
 * COPY node spec derived from a java COPY node
 */
class JCOPY: public JOGExpr, COPY
{
  public:
    JCOPY(jobject * obj);
    ~JCOPY();
    void debug_print();
  private:
    jobject * _backingObject;
};


/**
 * PLUS node spec derived from a java PLUS node
 */
class JPLUS: public JOGExpr, PLUS
{
  public:
    JPLUS(jobject * obj);
    ~JPLUS();
    void debug_print();
  private:
    jobject * _backingObject = NULL;

};

/**
 * MINUS node spec derived from a java MINUS node
 */
class JMINUS: public JOGExpr, MINUS
{
  public:
    JMINUS(jobject * obj);
    ~JMINUS();
    void debug_print();
  private:
    jobject * _backingObject = NULL;
};

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
      printf("Clazz Type is hashdefined as %lld\n",ID);
      switch(ID)
      {
      case OGREALMATRIX_ENUM:
      {
        printf("Binding a JOGRealMatrix\n");
        _expr = new JOGRealMatrix(&obj);
        _expr->debug_print();
      }
      break;
      case OGCOMPLEXMATRIX_ENUM:
      {
        printf("Binding a JOGComplexMatrix\n");
        _expr = new JOGComplexMatrix(&obj);
        _expr->debug_print();
      }
      break;
      case OGREALSPARSEMATRIX_ENUM:
      {
        printf("Binding a JOGRealSparseMatrix\n");
        _expr = new JOGRealSparseMatrix(&obj);
        _expr->debug_print();
      }
      break;
      case OGCOMPLEXSPARSEMATRIX_ENUM:
      {
        printf("Binding a JOGComplexSparseMatrix\n");
        _expr = new JOGComplexSparseMatrix(&obj);
        _expr->debug_print();
      }
      break;
      case COPY_ENUM:
      {
        printf("COPY function\n");
        _expr = new JCOPY(&obj);
      }
      break;
      case MINUS_ENUM:
      {
        printf("MINUS function\n");
        _expr = new JMINUS(&obj);
      }
      break;
      case PLUS_ENUM:
      {
        printf("PLUS function\n");
        _expr = new JPLUS(&obj);
      }
      break;
      default:
      {
        throw convertExcept;
      }
      break;
      }

      if(!_expr)
      {
        printf("_expr is NULL so hasn't been set by the factory, exiting\n");
        exit(1);
      }

      printf("Returning from exprfactory\n");
      return _expr;
    };
  private:
    librdag::OGNumeric * _expr = NULL;
};


/**
 * OGExpr implementation details
 * Create an expression node from a java expression node
 */
JOGExpr::JOGExpr(jobject * obj)
{
  this->_backingObject = obj;
  // get object array
  if(!obj)
  {
#ifdef _DEBUG
    printf("JOGExpr: null obj\n");
#endif
    exit(1);
  }
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
  std::vector<OGNumeric *> * local_args = new std::vector<librdag::OGNumeric *>;
  ExprFactory * factory = new ExprFactory();
  for(int i=0; i<len; i++)
  {

    jobject tmp = (jobject) env->GetObjectArrayElement(*args, i);
    local_args->push_back(factory->getExpr(tmp));
  }
#ifdef _DEBUG
  printf("Making assignment\n");
#endif
  this->setArgs(local_args);
};

JOGExpr:: ~JOGExpr()
{
  _backingObject = NULL;
};


/**
 * JCOPY implementation details
 * COPY node derived from a java COPY node
 */
JCOPY::JCOPY(jobject * obj): JOGExpr(obj) { };
void JCOPY::debug_print()
{
  printf("JCOPY class\n");
};
JCOPY::~JCOPY()
{
  _backingObject = NULL;
};

/**
 * JPLUS implementation details
 * PLUS node derived from a java PLUS node
 */
JPLUS::JPLUS(jobject * obj): JOGExpr(obj) { };
void JPLUS::debug_print()
{
  printf("JPLUS class\n");
};
JPLUS::~JPLUS()
{
  _backingObject = NULL;
};

/**
 * JMINUS implementation details
 * MINUS node derived from a java MINUS node
 */
JMINUS::JMINUS(jobject * obj): JOGExpr(obj) { };
void JMINUS::debug_print()
{
  printf("JMINUS class\n");
};
JMINUS::~JMINUS()
{
  _backingObject = NULL;
};



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
};


}; // namespace ends

#ifdef __cplusplus
extern "C" {
#endif
  /*
   * javap generated signature class for the Materialisers.java class
   *
   * This code is the entry and exit point for java, it takes a the pointer to
   * the root node of a tree and then converts the java tree to a C++ tree and passes it
   * to the entry point code when then runs its walkers.
   *
   * Class:     com_opengamma_longdog_materialisers_Materialisers
   * Method:    materialise
   * Signature: (Lcom/opengamma/longdog/datacontainers/OGNumeric;)Lcom/opengamma/longdog/datacontainers/OGNumeric;
   */
  JNIEXPORT jobject JNICALL Java_com_opengamma_longdog_materialisers_Materialisers_materialise
  (JNIEnv * env, jclass clazz, jobject obj)
  {

    printf("Entering materialise function\n");


    printf("Calling convert::instantiateJClassAsCXXClass\n");
    // convert obj to OGNumeric objs
    librdag::OGNumeric * chain = (librdag::OGNumeric *) convert::instantiateJClassAsCXXClass(obj);

    printf("Calling entrypt function\n");
    // pass to entrypt
    struct c_OGNumeric * answer = entrypt((struct c_OGNumeric *) chain);

    printf("Calling delete\n");
//     delete chain;

    printf("Returning from materialise function\n\n\n");
    // convert to a jObj
    return (jobject) obj;
  }

#ifdef __cplusplus
}
#endif
