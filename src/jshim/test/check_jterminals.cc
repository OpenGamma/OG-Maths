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

using namespace std;
using namespace convert;

// mock JVM
class Fake_JavaVM: public JavaVM
{
  public:
    virtual ~Fake_JavaVM(){};
    virtual int GetEnv(void ** env, int SUPPRESS_UNUSED version) override
    {
     *env = _env;
     return JNI_OK;
    }
    void setEnv(JNIEnv * env)
    {
        this->_env = env;
    }
    JNIEnv * getEnv()
    {
        return this->_env;
    }
    virtual jint AttachCurrentThread(void SUPPRESS_UNUSED **penv, void SUPPRESS_UNUSED *args)
    {
      *penv = this->_env;
      return JNI_OK;
    }
};

// mock JNIEnv
class Fake_JNIEnv: public JNIEnv
{
  public:
    Fake_JNIEnv()
    {
      _someNonClass = new _jclass();
      _someNonMethodID = new _jmethodID();
      _someNonFieldID = new _jfieldID();
    }
    virtual ~Fake_JNIEnv()
    {
      delete _someNonMethodID;
      delete _someNonFieldID;
      delete _someNonClass;
    }
    virtual jclass FindClass(const char SUPPRESS_UNUSED * name)
    {
      return _someNonClass;
    }
    virtual jmethodID GetMethodID(jclass SUPPRESS_UNUSED clazz, const char SUPPRESS_UNUSED *name, const char SUPPRESS_UNUSED *sig) override
    {
      return _someNonMethodID;
    }
    virtual jfieldID GetFieldID(jclass SUPPRESS_UNUSED clazz, const char SUPPRESS_UNUSED *name, const char SUPPRESS_UNUSED *sig) override
    {
      return _someNonFieldID;
    }
    using JNIEnv::CallIntMethod;
    virtual jobject CallObjectMethod(jobject SUPPRESS_UNUSED obj, jmethodID SUPPRESS_UNUSED methodID, ...) override
    {
      return nullptr;
    }
private:
    jclass _someNonClass;
    jmethodID _someNonMethodID;
    jfieldID _someNonFieldID;
};

template<typename T> class Fake_JNIEnv_for_binding: public Fake_JNIEnv
{
  public:
    Fake_JNIEnv_for_binding(T value):Fake_JNIEnv()
    {
      _value = value;
    }
    // fake object call returns obj
    virtual jobject CallObjectMethod(jobject SUPPRESS_UNUSED obj, jmethodID SUPPRESS_UNUSED methodID, ...) override
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
      virtual real16 * GetDoubleArrayElements(jdoubleArray SUPPRESS_UNUSED arr, bool  SUPPRESS_UNUSED *isCopy) override
      {
        return nullptr;
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
      virtual int * GetIntArrayElements(jintArray SUPPRESS_UNUSED arr, bool  SUPPRESS_UNUSED *isCopy) override
      {
        return nullptr;
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
