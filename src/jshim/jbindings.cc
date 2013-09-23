/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "jbindings.hh"
#include "jni.h"
#include "jvmcache.h"
#include "exceptions.hh"
#include "jshim.h"

namespace convert {

/**
 * bindPrimitiveArrayData
 */

template <typename T, typename S> T * bindPrimitiveArrayData(jobject obj, jmethodID method)
{
  if(!obj)
  {
    fprintf(stderr, "bindPrimitiveArrayData: null obj\n");
    throw new convertException();
  }
  if(!method)
  {
    fprintf(stderr, "bindPrimitiveArrayData: null method\n");
    throw new convertException();
  }
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMcache->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
    fprintf(stderr, "Thread attach failed\n");
    throw new convertException();
  }
  jobject dataobj = NULL;
  dataobj = env->CallObjectMethod(obj, method);
  if(!dataobj)
  {
    fprintf(stderr, "CallObjectMethod failed\n");
    throw new convertException();
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
    fprintf(stderr, "unbindPrimitiveArrayData: null nativeData\n");
    throw new convertException();
  }
  if(!obj)
  {
    fprintf(stderr, "unbindPrimitiveArrayData: null obj\n");
    throw new convertException();
  }
  if(!method)
  {
    fprintf(stderr, "unbindPrimitiveArrayData: null method\n");
    throw new convertException();
  }
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMcache->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
    fprintf(stderr, "Thread attach failed\n");
    throw new convertException();
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
  jStatus=JVMcache->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
    fprintf(stderr, "Thread attach failed\n");
    throw new convertException();
  }
  jobject dataobj = env->CallObjectMethod(obj, OGTerminalClazz_getData);
  jdoubleArray * array = reinterpret_cast<jdoubleArray *>(&dataobj);
  env->ReleasePrimitiveArrayCritical(*array, (void *)nativeData, 0);
}

template void
unbindOGArrayData<real16>(real16* nativeData, jobject obj);

template void
unbindOGArrayData<complex16>(complex16* nativeData, jobject obj);

} //namespace convert
