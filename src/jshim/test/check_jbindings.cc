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
private:
    jclass _someNonClass;
    jmethodID _someNonMethodID;
    jfieldID _someNonFieldID;
};

class bindRunner
{
  public:
    void run(jobject obj, jmethodID meth)
    {
      bindPrimitiveArrayData<real16, jdoubleArray>(obj, meth);
    }
};

class unbindRunner
{
  public:
    void run(real16 * nativedata, jobject obj, jmethodID meth)
    {
      unbindPrimitiveArrayData<real16, jdoubleArray>(nativedata,obj, meth);
    }
};

TEST(JBindings, Test_bindPrimitiveArrayData_bad_obj)
{
    jobject obj = nullptr;
    jmethodID meth = new _jmethodID();
    bindRunner * clazz = new bindRunner();
    ASSERT_ANY_THROW(clazz->run(obj,meth));
    delete clazz;
    delete meth;
}

TEST(JBindings, Test_bindPrimitiveArrayData_bad_method)
{
    jobject obj = new _jobject();
    jmethodID meth = nullptr;
    bindRunner * clazz = new bindRunner();
    ASSERT_ANY_THROW(clazz->run(obj,meth));
    delete obj;
    delete clazz;
}

TEST(JBindings, Test_bindPrimitiveArrayData_bad_env)
{
    class Fake_JavaVM_bad_env: public Fake_JavaVM
    {
      virtual jint AttachCurrentThread(void SUPPRESS_UNUSED **penv, void SUPPRESS_UNUSED *args) override
      {
        *penv = this->_env;
        return JNI_ERR;
      }
    };
    Fake_JavaVM * jvm = new Fake_JavaVM_bad_env();
    Fake_JNIEnv * env  = new Fake_JNIEnv();
    jvm->setEnv(env);
    JVMManager::initialize(jvm);
    jobject obj = new _jobject();
    jmethodID meth = new _jmethodID();
    bindRunner * clazz = new bindRunner();
    ASSERT_ANY_THROW(clazz->run(obj,meth));
    delete jvm;
    delete env;
    delete obj;
    delete meth;
    delete clazz;
}

TEST(JBindings, Test_bindPrimitiveArrayData_bad_callobjmethod)
{
    class Fake_JNIEnv_bad_com: public Fake_JNIEnv
    {
      virtual jobject CallObjectMethod(jobject SUPPRESS_UNUSED obj, jmethodID SUPPRESS_UNUSED methodID, ...) override
      {
        return nullptr;
      }
    };
    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_JNIEnv * env  = new Fake_JNIEnv_bad_com();
    jvm->setEnv(env);
    JVMManager::initialize(jvm);
    jobject obj = new _jobject();
    jmethodID meth = new _jmethodID();
    bindRunner * clazz = new bindRunner();
    ASSERT_ANY_THROW(clazz->run(obj,meth));
    delete jvm;
    delete env;
    delete obj;
    delete meth;
    delete clazz;
}

TEST(JBindings, Test_unbindPrimitiveArrayData_bad_data)
{
  real16 * data = nullptr;
  jobject obj = new _jobject();
  jmethodID meth = new _jmethodID();
  unbindRunner * clazz = new unbindRunner();
  ASSERT_ANY_THROW(clazz->run(data,obj,meth));
  delete obj;
  delete meth;
  delete clazz;
}

TEST(JBindings, Test_unbindPrimitiveArrayData_bad_obj)
{
  real16 * data = new real16[1];
  jobject obj = nullptr;
  jmethodID meth = new _jmethodID();
  unbindRunner * clazz = new unbindRunner();
  ASSERT_ANY_THROW(clazz->run(data,obj,meth));
  delete[] data;
  delete meth;
  delete clazz;
}

TEST(JBindings, Test_unbindPrimitiveArrayData_bad_meth)
{
  real16 * data = new real16[1];
  jobject obj = new _jobject();
  jmethodID meth = nullptr;
  unbindRunner * clazz = new unbindRunner();
  ASSERT_ANY_THROW(clazz->run(data,obj,meth));
  delete[] data;
  delete obj;
  delete clazz;
}

TEST(JBindings, Test_unbindPrimitiveArrayData_bad_env)
{
    class Fake_JavaVM_bad_env: public Fake_JavaVM
    {
      virtual jint AttachCurrentThread(void SUPPRESS_UNUSED **penv, void SUPPRESS_UNUSED *args) override
      {
        *penv = this->_env;
        return JNI_ERR;
      }
    };
    Fake_JavaVM * jvm = new Fake_JavaVM_bad_env();
    Fake_JNIEnv * env  = new Fake_JNIEnv();
    jvm->setEnv(env);
    JVMManager::initialize(jvm);
    real16 * data = new real16[1];
    jobject obj = new _jobject();
    jmethodID meth = new _jmethodID();
    unbindRunner * clazz = new unbindRunner();
    ASSERT_ANY_THROW(clazz->run(data,obj,meth));
    delete jvm;
    delete env;
    delete[] data;
    delete obj;
    delete meth;
    delete clazz;
}

TEST(JBindings, Test_unbindPrimitiveArrayData_bad_callobjmethod)
{
    class Fake_JNIEnv_bad_com: public Fake_JNIEnv
    {
      virtual jobject CallObjectMethod(jobject SUPPRESS_UNUSED obj, jmethodID SUPPRESS_UNUSED methodID, ...) override
      {
        return nullptr;
      }
    };
    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_JNIEnv * env  = new Fake_JNIEnv_bad_com();
    jvm->setEnv(env);
    JVMManager::initialize(jvm);
    real16 * data = new real16[1];
    jobject obj = new _jobject();
    jmethodID meth = new _jmethodID();
    unbindRunner * clazz = new unbindRunner();
    ASSERT_ANY_THROW(clazz->run(data,obj,meth));
    delete jvm;
    delete env;
    delete[] data;
    delete obj;
    delete meth;
    delete clazz;
}