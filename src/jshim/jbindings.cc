/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "debug.h"
#include "jbindings.hh"
#include "jvmmanager.hh"
#include "exceptions.hh"

namespace convert {

/**
 * bindPrimitiveArrayData
 */

template <typename T, typename S> T * bindPrimitiveArrayData(jobject obj, jmethodID method)
{
  if(!obj)
  {
    throw convert_error("bindPrimitiveArrayData: null obj");
  }
  if(!method)
  {
    throw convert_error("bindPrimitiveArrayData: null method");
  }
  VAL64BIT_PRINT("Binding for jobject", obj);
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMManager::getJVM()->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
    throw convert_error("Thread attach failed");
  }
  jobject dataobj = NULL;
  dataobj = env->CallObjectMethod(obj, method);
  if(!dataobj)
  {
    throw convert_error("CallObjectMethod failed");
  }
  S * array = reinterpret_cast<S *>(&dataobj);
  T * _dataptr = (T *) env->GetPrimitiveArrayCritical(*array,NULL);
  return _dataptr;
}

template real16*
bindPrimitiveArrayData<real16, jdoubleArray>(jobject obj, jmethodID method);

template complex16*
bindPrimitiveArrayData<complex16, jdoubleArray>(jobject obj, jmethodID method);

template int*
bindPrimitiveArrayData<int, jintArray>(jobject obj, jmethodID method);

/**
 * unbindPrimitiveArrayData
 */

template <typename T, typename S> void unbindPrimitiveArrayData(T * nativeData, jobject obj, jmethodID method)
{
  if(!nativeData)
  {
    throw convert_error("unbindPrimitiveArrayData: null nativeData");
  }
  if(!obj)
  {
    throw convert_error("unbindPrimitiveArrayData: null obj");
  }
  if(!method)
  {
    throw convert_error("unbindPrimitiveArrayData: null method");
  }
  VAL64BIT_PRINT("Unbinding for jobject", obj);
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMManager::getJVM()->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
    throw convert_error("Thread attach failed");
  }
  jobject dataobj = env->CallObjectMethod(obj, method);
  S * array = reinterpret_cast<S *>(&dataobj);
  env->ReleasePrimitiveArrayCritical(*array, (void *)nativeData, 0);
}

template void
unbindPrimitiveArrayData<real16, jdoubleArray>(real16* nativeData, jobject obj, jmethodID method);

template void
unbindPrimitiveArrayData<complex16, jdoubleArray>(complex16* nativeData, jobject obj, jmethodID method);

template void
unbindPrimitiveArrayData<int, jintArray>(int* nativeData, jobject obj, jmethodID method);

/**
 * unbindOGArrayData
 */

template <typename T> void unbindOGArrayData(T * nativeData, jobject obj)
{
  JNIEnv *env = NULL;
  jint jStatus = 0;
  VAL64BIT_PRINT("Unbinding for OGArrayData", obj);
  jStatus=JVMManager::getJVM()->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
    throw convert_error("Thread attach failed");
  }
  jobject dataobj = env->CallObjectMethod(obj, JVMManager::getOGTerminalClazz_getData());
  jdoubleArray * array = reinterpret_cast<jdoubleArray *>(&dataobj);
  env->ReleasePrimitiveArrayCritical(*array, (void *)nativeData, 0);
}

template void
unbindOGArrayData<real16>(real16* nativeData, jobject obj);

template void
unbindOGArrayData<complex16>(complex16* nativeData, jobject obj);

template void
unbindOGArrayData<int>(int* nativeData, jobject obj);

} //namespace convert
