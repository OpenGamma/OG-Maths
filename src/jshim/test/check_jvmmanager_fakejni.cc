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

// Fake VM
class Fake_JavaVM: public JavaVM
{
  public:   
    virtual ~Fake_JavaVM(){};
    virtual int GetEnv(void SUPPRESS_UNUSED ** env, int SUPPRESS_UNUSED version) override
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
    virtual jint AttachCurrentThread(void SUPPRESS_UNUSED **penv, void SUPPRESS_UNUSED *args) { return JNI_OK; }
};

// Test bad JVM (GetEnv() env ptr returns JNI_ERR)
TEST(JVMManagerFakeJNITest, Test_JVMManager_initialize_badversion)
{
    class Fake_JavaVM_badinit: public JavaVM
    {
      public:   
        virtual ~Fake_JavaVM_badinit(){};
        virtual int GetEnv(void SUPPRESS_UNUSED ** env, int SUPPRESS_UNUSED version) override
        {
            *env = _env;
            return JNI_ERR;
        }
        void setEnv(void ** env)
        {
            this->_env = (JNIEnv *)*env;
        }
        virtual jint AttachCurrentThread(void SUPPRESS_UNUSED **penv, void SUPPRESS_UNUSED *args) { return 0; }
    };
   
    JVMManager * jvm_manager = new JVMManager();
    Fake_JavaVM_badinit * jvm = new Fake_JavaVM_badinit();
    JNIEnv theEnv;
    JNIEnv * env  = &theEnv;   
    jvm->setEnv((void **) &env);
    ASSERT_ANY_THROW(jvm_manager->initialize(jvm));
    
    
    delete jvm;
    delete jvm_manager;
}


// although override on env isn't needed, it tests the override mech as well as the intended code
class Fake_JNIEnv_testRegister: public JNIEnv
{
  public:
    Fake_JNIEnv_testRegister()
    {
      _someNonClass = new _jclass();
      _someNonMethodID = new _jmethodID();
      _someNonFieldID = new _jfieldID();
    }
    virtual ~Fake_JNIEnv_testRegister()
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
    virtual jfieldID GetFieldID(jclass SUPPRESS_UNUSED clazz, const char SUPPRESS_UNUSED *name, const char SUPPRESS_UNUSED *sig)
    {
      return _someNonFieldID;
    }              
private:
    jclass _someNonClass;
    jmethodID _someNonMethodID;
    jfieldID _someNonFieldID;
};

TEST(JVMManagerFakeJNITest, Test_JVMManager_test_registerReferences)
{
    
    JVMManager * jvm_manager = new JVMManager();
    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_JNIEnv_testRegister * env  = new Fake_JNIEnv_testRegister();
    jvm->setEnv(env);
    jvm_manager->initialize(jvm);
    JNIEnv * ptrEnv = jvm->getEnv();
    jmethodID SUPPRESS_UNUSED foo = ptrEnv->GetMethodID(nullptr, nullptr, nullptr);
    
    ASSERT_TRUE(jvm_manager->getOGNumericClazz()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGExprClazz()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGArrayClazz()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGTerminalClazz()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGSparseMatrixClazz()!=nullptr);
    ASSERT_TRUE(jvm_manager->getBigDDoubleArrayClazz()!=nullptr);
    ASSERT_TRUE(jvm_manager->getComplexArrayContainerClazz()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGExprTypeEnumClazz()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGScalarClazz()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGTerminalClazz_getData()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGNumericClazz_getType()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGExprClazz_getExprs()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGExprClazz_getNExprs()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGArrayClazz_getRows()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGArrayClazz_getCols()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGSparseMatrixClazz_getColPtr()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGSparseMatrixClazz_getRowIdx()!=nullptr);
    ASSERT_TRUE(jvm_manager->getComplexArrayContainerClazz_ctor_DAoA_DAoA()!=nullptr);
    ASSERT_TRUE(jvm_manager-> getOGExprTypeEnumClazz__hashdefined()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGRealScalarClazz()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGComplexScalarClazz()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGRealDenseMatrixClazz()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGComplexDenseMatrixClazz()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGRealDiagonalMatrixClazz()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGComplexDiagonalMatrixClazz()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGRealSparseMatrixClazz()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGComplexSparseMatrixClazz()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGRealScalarClazz_init()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGComplexScalarClazz_init()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGRealDenseMatrixClazz_init()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGComplexDenseMatrixClazz_init()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGRealDiagonalMatrixClazz_init()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGComplexDiagonalMatrixClazz_init()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGRealSparseMatrixClazz_init()!=nullptr);
    ASSERT_TRUE(jvm_manager->getOGComplexSparseMatrixClazz_init()!=nullptr);

    delete env;
    delete jvm;
    delete jvm_manager;
}



// this class and its children are in some way broken to trip various throws  
class Fake_Broken_Base_JNIEnv: public Fake_JNIEnv_testRegister
{
  public:
    Fake_Broken_Base_JNIEnv(){}
    virtual ~Fake_Broken_Base_JNIEnv(){}
    virtual jclass FindClass(const char SUPPRESS_UNUSED * name) 
    {
      return _someNonClass;
    }
    virtual jmethodID GetMethodID(jclass SUPPRESS_UNUSED clazz, const char SUPPRESS_UNUSED *name, const char SUPPRESS_UNUSED *sig) override
    { 
      return _someNonMethodID;
    }
    virtual jfieldID GetFieldID(jclass SUPPRESS_UNUSED clazz, const char SUPPRESS_UNUSED *name, const char SUPPRESS_UNUSED *sig)
    {
      return _someNonFieldID;
    }              
protected:
    jclass _someNonClass = nullptr;
    jmethodID _someNonMethodID = nullptr;
    jfieldID _someNonFieldID = nullptr;
};

// Test broken FindClass()
TEST(JVMManagerFakeJNITest, Test_JVMManager_test_registerReferences_badFindClass)
{
    class Fake_Broken_ClassRef_JNIEnv: public Fake_Broken_Base_JNIEnv
    {
      public:
        Fake_Broken_ClassRef_JNIEnv(){
          _someNonMethodID = new _jmethodID();
          _someNonFieldID = new _jfieldID();
        }
        virtual ~Fake_Broken_ClassRef_JNIEnv()
        {
          delete _someNonMethodID;
          delete _someNonFieldID;          
        }
    };
   
    JVMManager * jvm_manager = new JVMManager();
    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_Broken_ClassRef_JNIEnv * broken_env  = new Fake_Broken_ClassRef_JNIEnv();
    jvm->setEnv(broken_env);
    ASSERT_ANY_THROW(jvm_manager->initialize(jvm));
       
    delete broken_env;
    delete jvm;
    delete jvm_manager; 
}

// Test broken NewGlobalRef()
TEST(JVMManagerFakeJNITest, Test_JVMManager_test_registerReferences_badNewGlobalRef)
{
    class Fake_Broken_NewGlobalRef_JNIEnv: public Fake_JNIEnv_testRegister
    {
      public:
        virtual jobject NewGlobalRef(jobject SUPPRESS_UNUSED lobj)
        {
          return _ref;
        }
      private:
        jobject _ref = nullptr;
    };  

   
    JVMManager * jvm_manager = new JVMManager();
    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_Broken_NewGlobalRef_JNIEnv * broken_env  = new Fake_Broken_NewGlobalRef_JNIEnv();
    jvm->setEnv(broken_env);
    ASSERT_ANY_THROW(jvm_manager->initialize(jvm));
       
    delete broken_env;
    delete jvm;
    delete jvm_manager; 
}

// Test broken GetMethodID
TEST(JVMManagerFakeJNITest, Test_JVMManager_test_registerReferences_badGetMethodID)
{
 
    class Fake_Broken_MethodIDRef_JNIEnv: public Fake_Broken_Base_JNIEnv
    {
      public:
        Fake_Broken_MethodIDRef_JNIEnv(){
          _someNonClass = new _jclass();
          _someNonFieldID = new _jfieldID();
          _someNonMethodID = nullptr;
        }
        virtual ~Fake_Broken_MethodIDRef_JNIEnv()
        {
          delete _someNonClass;
          delete _someNonFieldID;          
        }          
    }; 

   
    JVMManager * jvm_manager = new JVMManager();
    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_Broken_MethodIDRef_JNIEnv * broken_env  = new Fake_Broken_MethodIDRef_JNIEnv();
    jvm->setEnv(broken_env);
    ASSERT_ANY_THROW(jvm_manager->initialize(jvm));
       
    delete broken_env;
    delete jvm;
    delete jvm_manager;    
}


// Test broken GetFieldID
TEST(JVMManagerFakeJNITest, Test_JVMManager_test_registerReferences_badGetFieldID)
{
 
    class Fake_Broken_FieldIDRef_JNIEnv: public Fake_Broken_Base_JNIEnv
    {
      public:
        Fake_Broken_FieldIDRef_JNIEnv(){
          _someNonClass = new _jclass();
          _someNonFieldID = nullptr;
          _someNonMethodID = new _jmethodID();
        }
        virtual ~Fake_Broken_FieldIDRef_JNIEnv()
        {
          delete _someNonClass;
          delete _someNonMethodID;
        }          
    }; 
   
    JVMManager * jvm_manager = new JVMManager();
    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_Broken_FieldIDRef_JNIEnv * broken_env  = new Fake_Broken_FieldIDRef_JNIEnv();
    jvm->setEnv(broken_env);
    ASSERT_ANY_THROW(jvm_manager->initialize(jvm));
       
    delete broken_env;
    delete jvm;
    delete jvm_manager;    
}

// test NewObjectArray()
TEST(JVMManagerFakeJNITest, Test_JVMManager_newObjectArray)
{  
    class _jobjectArrayImpl: public _jobjectArray
    {
      public:
        _jobjectArrayImpl(int len)
        {
            _len = len;
        }
        int getLen()
        {
          return _len;
        }      
      private:
        int _len;      
    };
  
    class Fake_JNIEnv: public Fake_JNIEnv_testRegister
    {
      public:
        Fake_JNIEnv()
        {
          _arr = new _jobjectArrayImpl(10);
        }
        virtual ~Fake_JNIEnv()
        {
          delete _arr;
        }
        virtual jobjectArray NewObjectArray(jsize SUPPRESS_UNUSED len, jclass SUPPRESS_UNUSED clazz, jobject SUPPRESS_UNUSED init)
        {
          return _arr;
        }
        jobjectArray getArr()
        {
          return _arr;
        }
    private:
        jobjectArray _arr;
    };
    
    class Fake_Broken_JNIEnv: public Fake_JNIEnv_testRegister
    {
      public:
        Fake_Broken_JNIEnv() {}
        virtual ~Fake_Broken_JNIEnv(){}
        virtual jobjectArray NewObjectArray(jsize SUPPRESS_UNUSED len, jclass SUPPRESS_UNUSED clazz, jobject SUPPRESS_UNUSED init)
        {
          return _arr;
        }
        jobjectArray getArr()
        {
          return _arr;
        }
    private:
        jobjectArray _arr = NULL;
    };    

    JVMManager * jvm_manager = new JVMManager();
    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_JNIEnv * env  = new Fake_JNIEnv();
    jvm->setEnv(env);
    jvm_manager->initialize(jvm);
    JNIEnv * ptrEnv = jvm->getEnv();
    jobject init = (jobject) nullptr;
    jclass clazz = new _jclass();
    jobjectArray obj = jvm_manager->newObjectArray(ptrEnv, 10, clazz , init);
    ASSERT_TRUE(obj == static_cast<Fake_JNIEnv *>(ptrEnv)->getArr());
    ASSERT_TRUE(static_cast<_jobjectArrayImpl *>(obj)->getLen()==10);
    
    Fake_Broken_JNIEnv * env_broken  = new Fake_Broken_JNIEnv();
    jvm->setEnv(env_broken);
    ASSERT_ANY_THROW(jvm_manager->newObjectArray(env_broken, 10, clazz , init));
    
    delete clazz;
    delete env;
    delete env_broken;
    delete jvm;
    delete jvm_manager;
}


// test NewDoubleArray()
TEST(JVMManagerFakeJNITest, Test_JVMManager_newDoubleArray)
{  
    class _jdoubleArrayImpl: public _jdoubleArray
    {
      public:
        _jdoubleArrayImpl(int len)
        {
            _len = len;
        }
        int getLen()
        {
          return _len;
        }      
      private:
        int _len;      
    };
  
    class Fake_JNIEnv: public Fake_JNIEnv_testRegister
    {
      public:
        Fake_JNIEnv()
        {
          _arr = new _jdoubleArrayImpl(10);
        }
        virtual ~Fake_JNIEnv()
        {
          delete _arr;
        }
        virtual jdoubleArray NewDoubleArray(jsize SUPPRESS_UNUSED len)
        {
          return _arr;
        }
        jdoubleArray getArr()
        {
          return _arr;
        }
    private:
        jdoubleArray _arr;
    };
    
    class Fake_Broken_JNIEnv: public Fake_JNIEnv_testRegister
    {
      public:
        Fake_Broken_JNIEnv() {}
        virtual ~Fake_Broken_JNIEnv(){}
        virtual jdoubleArray NewDoubleArray(jsize SUPPRESS_UNUSED len)
        {
          return _arr;
        }
        jdoubleArray getArr()
        {
          return _arr;
        }
    private:
        jdoubleArray _arr = NULL;
    };    

    JVMManager * jvm_manager = new JVMManager();
    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_JNIEnv * env  = new Fake_JNIEnv();
    jvm->setEnv(env);
    jvm_manager->initialize(jvm);
    JNIEnv * ptrEnv = jvm->getEnv();
    jclass clazz = new _jclass();
    jdoubleArray obj = jvm_manager->newDoubleArray(ptrEnv, 10);
    ASSERT_TRUE(obj == static_cast<Fake_JNIEnv *>(ptrEnv)->getArr());
    ASSERT_TRUE(static_cast<_jdoubleArrayImpl *>(obj)->getLen()==10);
    
    Fake_Broken_JNIEnv * env_broken  = new Fake_Broken_JNIEnv();
    jvm->setEnv(env_broken);
    ASSERT_ANY_THROW(jvm_manager->newDoubleArray(env_broken, 10));
    
    delete clazz;
    delete env;
    delete env_broken;
    delete jvm;
    delete jvm_manager;
}


// test callObjectMethod()
TEST(JVMManagerFakeJNITest, Test_JVMManager_CallObjectMethod)
{ 
    class Fake_JNIEnv: public Fake_JNIEnv_testRegister
    {
      public:
        Fake_JNIEnv()
        {
          _obj = new _jobject();
        }
        virtual ~Fake_JNIEnv()
        {
          delete _obj;
        }
        virtual jobject CallObjectMethod(jobject SUPPRESS_UNUSED obj, jmethodID SUPPRESS_UNUSED methodID, ...)
        {
          return _obj;
        }
        jobject getObj()
        {
          return _obj;
        }
    private:
        jobject _obj = nullptr;
    };
    
    
    class Fake_Broken_JNIEnv: public Fake_JNIEnv_testRegister
    {
      public:
        Fake_Broken_JNIEnv() {}
        virtual ~Fake_Broken_JNIEnv(){}
        virtual jobject CallObjectMethod(jobject SUPPRESS_UNUSED obj, jmethodID SUPPRESS_UNUSED methodID, ...)
        {
          return _obj;
        }
        jobject getObj()
        {
          return _obj;
        }
    private:
        jobject _obj = nullptr;
    };   
    
    JVMManager * jvm_manager = new JVMManager();
    Fake_JavaVM * jvm = new Fake_JavaVM();
    Fake_JNIEnv * env  = new Fake_JNIEnv();
    jvm->setEnv(env);
    jvm_manager->initialize(jvm);
    JNIEnv * ptrEnv = jvm->getEnv();
    jobject someobj = new _jobject();
    jmethodID somemeth = new _jmethodID();    
    jobject obj = jvm_manager->callObjectMethod(ptrEnv, someobj, somemeth, nullptr);
    ASSERT_TRUE(obj == static_cast<Fake_JNIEnv *>(ptrEnv)->getObj());

    Fake_Broken_JNIEnv * env_broken  = new Fake_Broken_JNIEnv();
    jvm->setEnv(env_broken);
    ptrEnv = jvm->getEnv();
    ASSERT_ANY_THROW(jvm_manager->callObjectMethod(ptrEnv, someobj, somemeth, nullptr));    
    
    delete someobj;
    delete somemeth;
    delete env;
    delete env_broken;
    delete jvm;
    delete jvm_manager;
    
}

// test getEnv()
TEST(JVMManagerFakeJNITest, Test_JVMManager_getEnv)
{ 
// Fake VM
    class Fake_JavaVM_wOKAttach: public Fake_JavaVM
    {
      public:
      Fake_JavaVM_wOKAttach(){}
      virtual ~Fake_JavaVM_wOKAttach(){};
      virtual jint AttachCurrentThread(void SUPPRESS_UNUSED **penv, void SUPPRESS_UNUSED *args)
      {
       *penv = (void*)_env;
       return JNI_OK;
      }
    };
    
    JVMManager * jvm_manager = new JVMManager();
    Fake_JavaVM_wOKAttach * jvm = new Fake_JavaVM_wOKAttach();
    Fake_JNIEnv_testRegister * env  = new Fake_JNIEnv_testRegister();
    jvm->setEnv(env);
    jvm_manager->initialize(jvm);
    JNIEnv newJNIEnv;
    JNIEnv * newJNIEnvPtr;
    jvm_manager->getEnv((void **) &newJNIEnvPtr);
    ASSERT_TRUE(newJNIEnvPtr == env);
    
    delete jvm_manager;
    delete jvm;
    delete env;
}


// test bad getEnv(), this is done by making the thread attachment return an error.
TEST(JVMManagerFakeJNITest, Test_JVMManager_badgetEnv)
{ 
    class Fake_JavaVM_wBadAttach: public Fake_JavaVM
    {
      public:
      Fake_JavaVM_wBadAttach(){}
      virtual ~Fake_JavaVM_wBadAttach(){};
      virtual jint AttachCurrentThread(void SUPPRESS_UNUSED **penv, void SUPPRESS_UNUSED *args)
      {
       *penv = (void*)_env;
       return JNI_ERR;
      }
    };
    
    JVMManager * jvm_manager = new JVMManager();    
    Fake_JavaVM_wBadAttach * jvm = new Fake_JavaVM_wBadAttach();
    Fake_JNIEnv_testRegister * env  = new Fake_JNIEnv_testRegister();    
    jvm->setEnv(env);
    jvm_manager->initialize(jvm);
    JNIEnv newJNIEnv;
    JNIEnv * newJNIEnvPtr;
    ASSERT_ANY_THROW(jvm_manager->getEnv((void **) &newJNIEnvPtr));
      
    delete jvm_manager;
    delete jvm;
    delete env;
}


// test ok getJVM()
TEST(JVMManagerFakeJNITest, Test_JVMManager_getJVM)
{
  JVMManager * jvm_manager = new JVMManager();    
  Fake_JavaVM * jvm = new Fake_JavaVM();
  Fake_JNIEnv_testRegister * env  = new Fake_JNIEnv_testRegister();    
  jvm->setEnv(env);  
  jvm_manager->initialize(jvm);
  
  ASSERT_TRUE(jvm_manager->getJVM()==jvm);
  
  delete jvm_manager;
  delete jvm;
  delete env;  
}

// test JNI_OnLoad
TEST(JVMManagerFakeJNITest, Test_JVMManager_JNI_OnLoad)
{
  Fake_JavaVM * jvm = new Fake_JavaVM();
  Fake_JNIEnv_testRegister * env  = new Fake_JNIEnv_testRegister();    
  jvm->setEnv(env);
  ASSERT_TRUE(JNI_OnLoad(jvm, nullptr)==JNI_VERSION_1_2);
  delete jvm;
  delete env;    
}

// test JNI_OnLoad
TEST(JVMManagerFakeJNITest, Test_JVMManager_bad_JNI_OnLoad)
{
  class Fake_JavaVM: public JavaVM
  {
    public:   
      virtual ~Fake_JavaVM(){};
      virtual int GetEnv(void SUPPRESS_UNUSED ** env, int SUPPRESS_UNUSED version) override
      {
          *env = _env;
          return JNI_ERR;
      }
      void setEnv(void ** env)
      {
          this->_env = (JNIEnv *)*env;
      }
      virtual jint AttachCurrentThread(void SUPPRESS_UNUSED **penv, void SUPPRESS_UNUSED *args) { return 0; }
  };
  
  Fake_JavaVM * jvm = new Fake_JavaVM();  
  Fake_JNIEnv_testRegister * env  = new Fake_JNIEnv_testRegister();    
  jvm->setEnv((void**)&env);
  ASSERT_TRUE(JNI_OnLoad(jvm, nullptr)==JNI_ERR);
  delete jvm;
  delete env;    
}

TEST(JVMManagerFakeJNITest, Test_JVMManager_checkEx_exfound)
{
  JVMManager * jvm_manager = new JVMManager();
  Fake_JavaVM * jvm = new Fake_JavaVM();
  Fake_JNIEnv_testRegister * env  = new Fake_JNIEnv_testRegister();
  jvm->setEnv(env);
  jvm_manager->initialize(jvm);
  ASSERT_ANY_THROW(checkEx(env));
  delete jvm_manager;
  delete jvm;
  delete env;
}

TEST(JVMManagerFakeJNITest, Test_JVMManager_checkEx_noexfound)
{
  JVMManager * jvm_manager = new JVMManager();
  Fake_JavaVM * jvm = new Fake_JavaVM();
  class Fake_JNIEnv_testRegister_noex: public Fake_JNIEnv_testRegister
  {
    virtual jthrowable ExceptionOccurred() override
    {
      return nullptr;
    }
  };
  Fake_JNIEnv_testRegister * env  = new Fake_JNIEnv_testRegister_noex();
  jvm->setEnv(env);
  jvm_manager->initialize(jvm);
  ASSERT_NO_THROW(checkEx(env));
  delete jvm_manager;
  delete jvm;
  delete env;
}