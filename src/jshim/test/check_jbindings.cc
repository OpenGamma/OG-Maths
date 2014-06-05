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
#include "test/fake_jvm.hh"
#include "exceptions.hh"

using namespace std;
using namespace convert;

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
    ASSERT_THROW(clazz->run(obj,meth), convert_error);
    delete clazz;
    delete meth;
}

TEST(JBindings, Test_bindPrimitiveArrayData_bad_method)
{
    jobject obj = new _jobject();
    jmethodID meth = nullptr;
    bindRunner * clazz = new bindRunner();
    ASSERT_THROW(clazz->run(obj,meth), convert_error);
    delete obj;
    delete clazz;
}

// MAT-329 disabling this test as all calls requiring *env now go via
// AttachCurrentThread so the code is always safe WRT the *env reference being valid.
// The test below will fail as a result of impairing the AttachCurrentThread function
// as registerGlobal*() methods called from JVMManager::initialize() need a working version
// hence initialisation will not complete successfully.
// TEST(JBindings, Test_bindPrimitiveArrayData_bad_env)
// {
//     class Fake_JavaVM_bad_env: public Fake_JavaVM
//     {
//       virtual jint AttachCurrentThread(void **penv, void SUPPRESS_UNUSED *args) override
//       {
//         *penv = this->_env;
//         return JNI_ERR;
//       }
//     };
//     Fake_JavaVM * jvm = new Fake_JavaVM_bad_env();
//     Fake_JNIEnv * env  = new Fake_JNIEnv();
//     jvm->setEnv(env);
//     JVMManager::initialize(jvm);
//     jobject obj = new _jobject();
//     jmethodID meth = new _jmethodID();
//     bindRunner * clazz = new bindRunner();
//     ASSERT_THROW(clazz->run(obj,meth), convert_error);
//     delete jvm;
//     delete env;
//     delete obj;
//     delete meth;
//     delete clazz;
// }

TEST(JBindings, Test_bindPrimitiveArrayData_bad_callobjmethod)
{
    class Fake_JNIEnv_bad_com: public Fake_JNIEnv
    {
      virtual jboolean ExceptionCheck() override
      {
        return JNI_TRUE;
      }
    };
    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_JNIEnv * env  = new Fake_JNIEnv_bad_com();
    jvm->setEnv(env);
    JVMManager::initialize(jvm);
    jobject obj = new _jobject();
    jmethodID meth = new _jmethodID();
    bindRunner * clazz = new bindRunner();
    ASSERT_THROW(clazz->run(obj,meth), convert_error);
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
  ASSERT_THROW(clazz->run(data,obj,meth), convert_error);
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
  ASSERT_THROW(clazz->run(data,obj,meth), convert_error);
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
  ASSERT_THROW(clazz->run(data,obj,meth), convert_error);
  delete[] data;
  delete obj;
  delete clazz;
}


// MAT-329 disabling this test as all calls requiring *env now go via
// AttachCurrentThread so the code is always safe WRT the *env reference being valid.
// The test below will fail as a result of impairing the AttachCurrentThread function
// as registerGlobal*() methods called from JVMManager::initialize() need a working version
// hence initialisation will not complete successfully.
// TEST(JBindings, Test_unbindPrimitiveArrayData_bad_env)
// {
//     class Fake_JavaVM_bad_env: public Fake_JavaVM
//     {
//       virtual jint AttachCurrentThread(void **penv, void SUPPRESS_UNUSED *args) override
//       {
//         *penv = this->_env;
//         return JNI_ERR;
//       }
//     };
//     Fake_JavaVM * jvm = new Fake_JavaVM_bad_env();
//     Fake_JNIEnv * env  = new Fake_JNIEnv();
//     jvm->setEnv(env);
//     JVMManager::initialize(jvm);
//     real16 * data = new real16[1];
//     jobject obj = new _jobject();
//     jmethodID meth = new _jmethodID();
//     unbindRunner * clazz = new unbindRunner();
//     ASSERT_THROW(clazz->run(data,obj,meth), convert_error);
//     delete jvm;
//     delete env;
//     delete[] data;
//     delete obj;
//     delete meth;
//     delete clazz;
// }

TEST(JBindings, Test_unbindPrimitiveArrayData_bad_callobjmethod)
{
    class Fake_JNIEnv_bad_com: public Fake_JNIEnv
    {
      virtual jboolean ExceptionCheck() override
      {
        return JNI_TRUE;
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
    ASSERT_THROW(clazz->run(data,obj,meth), convert_error);
    delete jvm;
    delete env;
    delete[] data;
    delete obj;
    delete meth;
    delete clazz;
}
