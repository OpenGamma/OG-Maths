/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "terminal.hh"
#include "jterminals.hh"
#include "jbindings.hh"
#include "gtest/gtest.h"
#include <string>
#include "warningmacros.h"
#include <iostream>
#include "jvmmanager.hh"
#include "debug.h"
#include "equals.hh"
#include "test/fake_jvm.hh"

using namespace std;
using namespace convert;


TEST(JTerminals, Test_getIntFromVoidJMethod_null_meth)
{
  jobject obj = new _jobject();
  ASSERT_ANY_THROW(getIntFromVoidJMethod(nullptr,obj));
  delete obj;
}

TEST(JTerminals, Test_getIntFromVoidJMethod_null_obj)
{
  jmethodID meth = new _jmethodID();
  ASSERT_ANY_THROW(getIntFromVoidJMethod(meth,nullptr));
  delete meth;
}

// MAT-329 disabling this test as all calls requiring *env now go via
// AttachCurrentThread so the code is always safe WRT the *env reference being valid.
// The test below will fail as a result of impairing the AttachCurrentThread function.
// 
// TEST(JTerminals, Test_getIntFromVoidJMethod_null_envptr)
// {
//   class Fake_JavaVM_bad_attach: public Fake_JavaVM
//   {
//     virtual jint AttachCurrentThread(void **penv, void SUPPRESS_UNUSED *args) override
//     {
//       *penv = nullptr;
//       return JNI_OK;
//     }
//   };
//   Fake_JavaVM * jvm = new Fake_JavaVM_bad_attach();
//   Fake_JNIEnv * env  = new Fake_JNIEnv();
//   jvm->setEnv(env);
//   JVMManager::initialize(jvm);
//   jmethodID meth = new _jmethodID();
//   jobject obj = new _jobject();
//   ASSERT_ANY_THROW(getIntFromVoidJMethod(meth,obj));
//   delete env;
//   delete jvm;
//   delete meth;
//   delete obj;
// }

// Required to allow a single FakeJNIEnv_for_binding for all types
template<typename T> jint toJint(T v);
template<> jint toJint<jint>(jint v) { return v; }
template<> jint toJint<real16>(real16 v) { return v; }
template<> jint toJint<complex16>(complex16 v) { return v.real(); }

template<typename T> class Fake_JNIEnv_for_binding: public Fake_JNIEnv
{
  public:
    Fake_JNIEnv_for_binding(T value):Fake_JNIEnv()
    {
      _value = value;
    }
    // fake object call returns obj
    virtual jobject CallObjectMethod(jobject obj, jmethodID SUPPRESS_UNUSED methodID, ...) override
    {
        return obj;
    }
    virtual real16 * GetDoubleArrayElements(jdoubleArray SUPPRESS_UNUSED arr, bool  SUPPRESS_UNUSED *isCopy) override
    {
      return reinterpret_cast<real16 *>(&_value);
    }
    virtual int * GetIntArrayElements(jintArray SUPPRESS_UNUSED arr, bool  SUPPRESS_UNUSED *isCopy) override
    {
      return reinterpret_cast<int *>(&_value);
    }
    virtual jint CallIntMethod(jobject SUPPRESS_UNUSED obj, jmethodID SUPPRESS_UNUSED methodID, ...) override
    {
      return toJint<T>(_value);
    }
  private:
    T _value;
};

TEST(JTerminals, Test_JOGRealScalar_ctor)
{
    real16 value = 1234;
    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_JNIEnv * env  = new Fake_JNIEnv_for_binding<real16>(value);
    jvm->setEnv(env);
    JVMManager::initialize(jvm);

    jobject obj =  new _jobject();
    JOGRealScalar * scalar = new JOGRealScalar(obj);
    ASSERT_TRUE(scalar->getValue()==value);

    // debug print for coverage purposes
    scalar->debug_print();

    delete scalar;
    delete obj;
    delete env;
    delete jvm;
}

TEST(JTerminals, Test_JOGComplexScalar_ctor)
{
    complex16 value = {1234,5678};
    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_JNIEnv * env  = new Fake_JNIEnv_for_binding<complex16>(value);
    jvm->setEnv(env);
    JVMManager::initialize(jvm);

    jobject obj =  new _jobject();
    JOGComplexScalar * scalar = new JOGComplexScalar(obj);
    ASSERT_TRUE(scalar->getValue()==value);

    // debug print for coverage purposes
    scalar->debug_print();

    delete scalar;
    delete obj;
    delete env;
    delete jvm;
}

TEST(JTerminals, Test_JOGIntegerScalar_ctor)
{
    int value = 1234;
    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_JNIEnv * env  = new Fake_JNIEnv_for_binding<int>(value);
    jvm->setEnv(env);
    JVMManager::initialize(jvm);

    jobject obj =  new _jobject();
    JOGIntegerScalar * scalar = new JOGIntegerScalar(obj);
    ASSERT_TRUE(scalar->getValue()==value);

    // debug print for coverage purposes
    scalar->debug_print();

    delete scalar;
    delete obj;
    delete env;
    delete jvm;
}

TEST(JTerminals, Test_binding_getArrayFromJava_bad_GetDoubleArrayElements)
{
    real16 value = 1234;
    Fake_JavaVM * jvm = new Fake_JavaVM();
    class Fake_JNIEnv_for_binding_bad: public Fake_JNIEnv_for_binding<real16>
    {
      using Fake_JNIEnv_for_binding<real16>::Fake_JNIEnv_for_binding;
      virtual jboolean ExceptionCheck() override
      {
        return JNI_TRUE;
      }
    };
    Fake_JNIEnv * env  = new Fake_JNIEnv_for_binding_bad(value);
    jvm->setEnv(env);
    JVMManager::initialize(jvm);

    jobject obj =  new _jobject();
    ASSERT_ANY_THROW(new JOGRealScalar(obj));

    delete obj;
    delete env;
    delete jvm;
}

TEST(JTerminals, Test_binding_getArrayFromJava_bad_GetIntArrayElements)
{
    real16 value = 1234;
    Fake_JavaVM * jvm = new Fake_JavaVM();
    class Fake_JNIEnv_for_binding_bad: public Fake_JNIEnv_for_binding<real16>
    {
      using Fake_JNIEnv_for_binding<real16>::Fake_JNIEnv_for_binding;
      virtual jboolean ExceptionCheck() override
      {
        return JNI_TRUE;
      }
    };
    Fake_JNIEnv * env  = new Fake_JNIEnv_for_binding_bad(value);
    jvm->setEnv(env);
    JVMManager::initialize(jvm);

    jobject obj =  new _jobject();
    ASSERT_ANY_THROW(new JOGIntegerScalar(obj));

    delete obj;
    delete env;
    delete jvm;
}

template<typename T>class Fake_JNIEnv_for_OGMatrix_T: public Fake_JNIEnv
{
  public:
    Fake_JNIEnv_for_OGMatrix_T(int rows, T * value):Fake_JNIEnv()
    {
      _rows = rows;
      _value = value;
    }
    virtual jint CallIntMethod(jobject SUPPRESS_UNUSED obj, jmethodID SUPPRESS_UNUSED methodID, ...) {
      return _rows;
    }
    virtual jobject CallObjectMethod(jobject obj, jmethodID SUPPRESS_UNUSED methodID, ...) override
    {
        return obj;
    }
    virtual real16 * GetDoubleArrayElements(jdoubleArray SUPPRESS_UNUSED arr, bool  SUPPRESS_UNUSED *isCopy) override
    {
      return reinterpret_cast<real16 *>(_value);
    }
  protected:
    int _rows;
    T * _value;
};

TEST(JTerminals, Test_JOGRealMatrix_ctor)
{
    int rval = 2;
    real16 * datav = new real16[4]{1,2,3,4};
    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_JNIEnv * env  = new Fake_JNIEnv_for_OGMatrix_T<real16>(rval, datav);
    jvm->setEnv(env);
    JVMManager::initialize(jvm);

    jobject obj =  new _jobject();
    JOGRealMatrix * mat = new JOGRealMatrix(obj);
    ASSERT_TRUE(mat->getRows()==rval);
    ASSERT_TRUE(mat->getCols()==rval);
    ASSERT_TRUE(ArrayFuzzyEquals(mat->getData(), datav, 4));

    // debug print for coverage purposes
    mat->debug_print();

    delete[] datav;
    delete mat;
    delete obj;
    delete env;
    delete jvm;
}

TEST(JTerminals, Test_JOGLogicalMatrix_ctor)
{
    int rval = 2;
    real16 * datav = new real16[4]{1,2,3,4};
    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_JNIEnv * env  = new Fake_JNIEnv_for_OGMatrix_T<real16>(rval, datav);
    jvm->setEnv(env);
    JVMManager::initialize(jvm);

    jobject obj =  new _jobject();
    JOGLogicalMatrix * mat = new JOGLogicalMatrix(obj);
    ASSERT_TRUE(mat->getRows()==rval);
    ASSERT_TRUE(mat->getCols()==rval);
    ASSERT_TRUE(ArrayFuzzyEquals(mat->getData(), datav, 4));

    // debug print for coverage purposes
    mat->debug_print();

    delete[] datav;
    delete mat;
    delete obj;
    delete env;
    delete jvm;
}

TEST(JTerminals, Test_JOGComplexMatrix_ctor)
{
    int rval = 2;
    complex16 * datav = new complex16[4]{{1,10},{2,20},{3,30},{4,40}};
    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_JNIEnv * env  = new Fake_JNIEnv_for_OGMatrix_T<complex16>(rval, datav);
    jvm->setEnv(env);
    JVMManager::initialize(jvm);

    jobject obj =  new _jobject();
    JOGComplexMatrix * mat = new JOGComplexMatrix(obj);
    ASSERT_TRUE(mat->getRows()==rval);
    ASSERT_TRUE(mat->getCols()==rval);
    ASSERT_TRUE(ArrayFuzzyEquals(mat->getData(), datav, 4));

    // debug print for coverage purposes
    mat->debug_print();

    delete[] datav;
    delete mat;
    delete obj;
    delete env;
    delete jvm;
}

TEST(JTerminals, Test_JOGRealDiagonalMatrix_ctor)
{
    int rval = 2;
    real16 * datav = new real16[2]{10,20};
    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_JNIEnv * env  = new Fake_JNIEnv_for_OGMatrix_T<real16>(rval, datav);
    jvm->setEnv(env);
    JVMManager::initialize(jvm);

    jobject obj =  new _jobject();
    JOGRealDiagonalMatrix * mat = new JOGRealDiagonalMatrix(obj);
    ASSERT_TRUE(mat->getRows()==rval);
    ASSERT_TRUE(mat->getCols()==rval);
    ASSERT_TRUE(ArrayFuzzyEquals(mat->getData(), datav, 2));

    // debug print for coverage purposes
    mat->debug_print();

    delete[] datav;
    delete mat;
    delete obj;
    delete env;
    delete jvm;
}

TEST(JTerminals, Test_JOGComplexDiagonalMatrix_ctor)
{
    int rval = 2;
    complex16 * datav = new complex16[2]{{1,10},{2,20}};
    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_JNIEnv * env  = new Fake_JNIEnv_for_OGMatrix_T<complex16>(rval, datav);
    jvm->setEnv(env);
    JVMManager::initialize(jvm);

    jobject obj =  new _jobject();
    JOGComplexDiagonalMatrix * mat = new JOGComplexDiagonalMatrix(obj);
    ASSERT_TRUE(mat->getRows()==rval);
    ASSERT_TRUE(mat->getCols()==rval);
    ASSERT_TRUE(ArrayFuzzyEquals(mat->getData(), datav, 2));

    // debug print for coverage purposes
    mat->debug_print();

    delete[] datav;
    delete mat;
    delete obj;
    delete env;
    delete jvm;
}


template<typename T>class Fake_JNIEnv_for_OGSparseMatrix_T: public Fake_JNIEnv_for_OGMatrix_T<T>
{
  public:
    Fake_JNIEnv_for_OGSparseMatrix_T(int rows, T * value, int * idx):Fake_JNIEnv_for_OGMatrix_T<T>(rows,value)
    {
      _idx = idx;
    }
    virtual jint CallIntMethod(jobject SUPPRESS_UNUSED obj, jmethodID SUPPRESS_UNUSED methodID, ...) {
      return this->_rows;
    }
    virtual jobject CallObjectMethod(jobject obj, jmethodID SUPPRESS_UNUSED methodID, ...) override
    {
        return obj;
    }
    virtual real16 * GetDoubleArrayElements(jdoubleArray SUPPRESS_UNUSED arr, bool  SUPPRESS_UNUSED *isCopy) override
    {
      return reinterpret_cast<real16 *>(this->_value);
    }
    virtual jint * GetIntArrayElements(jintArray SUPPRESS_UNUSED arr, bool  SUPPRESS_UNUSED *isCopy) override
    {
      return reinterpret_cast<jint *>(_idx);
    }
  private:
    int * _idx;
};

// The data for these two JOG*SparseMatrix tests is utter nonsense and totally invalid
// the mechanism opposed to the answer is tested

TEST(JTerminals, Test_JOGRealSparseMatrix_ctor)
{
    int rval = 2;
    real16 * datav = new real16[4]{1,2,3,4};
    int * idx = new int[4]{1,2,2,2};

    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_JNIEnv * env  = new Fake_JNIEnv_for_OGSparseMatrix_T<real16>(rval, datav, idx);
    jvm->setEnv(env);
    JVMManager::initialize(jvm);

    jobject obj =  new _jobject();
    JOGRealSparseMatrix * mat = new JOGRealSparseMatrix(obj);
    ASSERT_TRUE(mat->getRows()==rval);
    ASSERT_TRUE(mat->getCols()==rval);
    ASSERT_TRUE(ArrayFuzzyEquals(mat->getData(), datav, 4));
    ASSERT_TRUE(ArrayBitEquals(mat->getColPtr(), idx, 4));
    ASSERT_TRUE(ArrayBitEquals(mat->getRowIdx(), idx, 4));

    mat->debug_print();

    ASSERT_ANY_THROW(mat->toReal16ArrayOfArrays());
    ASSERT_ANY_THROW(mat->toComplex16ArrayOfArrays());

    delete[] datav;
    delete[] idx;
    delete mat;
    delete obj;
    delete env;
    delete jvm;
}

TEST(JTerminals, Test_JOGComplexSparseMatrix_ctor)
{
    int rval = 2;
    complex16 * datav = new complex16[4]{{1,10},{2,20},{3,30},{4,40}};
    int * idx = new int[4]{1,2,2,2};

    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_JNIEnv * env  = new Fake_JNIEnv_for_OGSparseMatrix_T<complex16>(rval, datav, idx);
    jvm->setEnv(env);
    JVMManager::initialize(jvm);

    jobject obj =  new _jobject();
    JOGComplexSparseMatrix * mat = new JOGComplexSparseMatrix(obj);
    ASSERT_TRUE(mat->getRows()==rval);
    ASSERT_TRUE(mat->getCols()==rval);
    ASSERT_TRUE(ArrayFuzzyEquals(mat->getData(), datav, 4));
    ASSERT_TRUE(ArrayBitEquals(mat->getColPtr(), idx, 4));
    ASSERT_TRUE(ArrayBitEquals(mat->getRowIdx(), idx, 4));

    mat->debug_print();

    ASSERT_ANY_THROW(mat->toReal16ArrayOfArrays());
    ASSERT_ANY_THROW(mat->toComplex16ArrayOfArrays());

    delete[] datav;
    delete[] idx;
    delete mat;
    delete obj;
    delete env;
    delete jvm;
}
