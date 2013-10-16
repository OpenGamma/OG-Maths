/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "terminal.hh"
#include "expression.hh"
#include "gtest/gtest.h"
#include <string>
#include "warningmacros.h"
#include <iostream>
#include "jvmmanager.hh"
#include "debug.h"

using namespace std;
using namespace convert;

JavaVM::~JavaVM(){}

// create a fake VM
class Fake_JavaVM: public JavaVM
{
  public:   
    virtual int GetEnv(void SUPPRESS_UNUSED ** env, int SUPPRESS_UNUSED version) override
    {
        *env = _env;
        return JNI_OK;
    }
    void setVersion(int value)
    {
        this->_version = value;
    }
    int getVersion()
    {
        return this->_version;
    }
    void setEnv(void ** env)
    {
        this->_env = (JNIEnv *)*env;
    }
    virtual jint AttachCurrentThread(void SUPPRESS_UNUSED **penv, void SUPPRESS_UNUSED *args) { return 0; }
};


TEST(JVMManagerFakeJNITest, Test_JVMManager_initialize)
{
    JVMManager * jvm_manager = new JVMManager();
    Fake_JavaVM * jvm = new Fake_JavaVM();

    JNIEnv theEnv;
    JNIEnv * env  = &theEnv;   
    jvm->setEnv((void **) &env);
    jvm->setVersion(4);
    jvm->GetEnv((void **) &env, 4);
    jvm_manager->initialize(jvm);

    delete jvm;
    delete jvm_manager;
}
