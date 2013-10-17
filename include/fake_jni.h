/*
 * Copyright (C) 2006 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * NOTICE: This file contains modifications made by OpenGamma Inc and
 * the OpenGamma group of companies
 *
 * Portions Copyright (C) 2013-present OpenGamma Inc. and the
 * OpenGamma group of companies. Please see distribution for license.
 *
 * Originally obtained from Android libnativehelper:
 * https://android.googlesource.com/platform/libnativehelper/
 * Revision 97b655635ba6182013989f2d0b956fe8ff0b921d (27th Feb 2013)
 *
 */

// this is a fake JNI.h for the purposes of testing
#ifndef _FAKE_JNI_HH
#define _FAKE_JNI_HH

#include <stdarg.h>
#include <stdint.h>

#include "warningmacros.h"

#ifdef __MINGW32__
#define JNIEXPORT __declspec(dllexport)
#define JNICALL __stdcall
#else // __MINGW32__
#define JNIEXPORT
#define JNICALL
#endif // __MINGW32__

/*
 * jboolean constants
 */

#define JNI_FALSE 0
#define JNI_TRUE 1

/*
 * possible return values for JNI functions.
 */

#define JNI_OK           0                 /* success */
#define JNI_ERR          (-1)              /* unknown error */
#define JNI_EDETACHED    (-2)              /* thread detached from the VM */
#define JNI_EVERSION     (-3)              /* JNI version error */

#define JNI_VERSION_1_2 0x00010002

/* Primitive types that match up with Java equivalents. */
typedef uint8_t  jboolean; /* unsigned 8 bits */
typedef int8_t   jbyte;    /* signed 8 bits */
typedef uint16_t jchar;    /* unsigned 16 bits */
typedef int16_t  jshort;   /* signed 16 bits */
typedef int32_t  jint;     /* signed 32 bits */
typedef int64_t  jlong;    /* signed 64 bits */
typedef float    jfloat;   /* 32-bit IEEE 754 */
typedef double   jdouble;  /* 64-bit IEEE 754 */

/* "cardinal indices and sizes" */
typedef jint            jsize;

/*
 * Reference types, in C++
 */
class _jobject {};
class _jclass : public _jobject {};
class _jthrowable : public _jobject {};
class _jstring : public _jobject {};
class _jarray : public _jobject {};
class _jbooleanArray : public _jarray {};
class _jbyteArray : public _jarray {};
class _jcharArray : public _jarray {};
class _jshortArray : public _jarray {};
class _jintArray : public _jarray {};
class _jlongArray : public _jarray {};
class _jfloatArray : public _jarray {};
class _jdoubleArray : public _jarray {};
class _jobjectArray : public _jarray {};

typedef _jobject*       jobject;
typedef _jclass*        jclass;
typedef _jstring*       jstring;
typedef _jarray*        jarray;
typedef _jobjectArray*  jobjectArray;
typedef _jbooleanArray* jbooleanArray;
typedef _jbyteArray*    jbyteArray;
typedef _jcharArray*    jcharArray;
typedef _jshortArray*   jshortArray;
typedef _jintArray*     jintArray;
typedef _jlongArray*    jlongArray;
typedef _jfloatArray*   jfloatArray;
typedef _jdoubleArray*  jdoubleArray;
typedef _jthrowable*    jthrowable;
typedef _jobject*       jweak;

/*
 * _jfieldID was an opaque structure in libnativehelper but we need it not to
 * be an incomplete type.
 */
class _jfieldID {
  public:
    _jfieldID(){};
    virtual ~_jfieldID(){};
};
typedef struct _jfieldID *jfieldID; /* field IDs */

/*
 * _jmethodID was an opaque structure in libnativehelper but we need it not to
 * be an incomplete type.
 */
class _jmethodID {
  public:
    _jmethodID(){};
    virtual ~_jmethodID(){};
};
typedef class _jmethodID *jmethodID; /* method IDs */


// This is a quick hack so the first jshim test doesn't blow up.
static _jclass allClasses;
static _jobject allGlobalRefs;
static _jfieldID allFieldIds;
static _jthrowable allThrows;


class _JNIEnv
{
  public:
    virtual jthrowable ExceptionOccurred() {return &allThrows; }
    virtual void ExceptionDescribe() {};
    virtual void SetDoubleArrayRegion(jdoubleArray SUPPRESS_UNUSED array, jsize SUPPRESS_UNUSED start, jsize SUPPRESS_UNUSED len, const jdouble SUPPRESS_UNUSED *buf)
    {

    }
    virtual void SetIntArrayRegion(jintArray SUPPRESS_UNUSED array, jsize SUPPRESS_UNUSED start, jsize SUPPRESS_UNUSED len, const jint SUPPRESS_UNUSED *buf)
    {

    }
    virtual void SetObjectArrayElement(jobjectArray SUPPRESS_UNUSED array, SUPPRESS_UNUSED jsize index, SUPPRESS_UNUSED jobject val)
    {

    }
    virtual jint EnsureLocalCapacity(jint SUPPRESS_UNUSED capacity) {
      return 0;
    }
    virtual jobject NewObject(jclass SUPPRESS_UNUSED clazz, jmethodID SUPPRESS_UNUSED methodID, ...) {
      return nullptr;
    }
    virtual jint CallIntMethod(jobject SUPPRESS_UNUSED obj, jmethodID SUPPRESS_UNUSED methodID, ...) {
      return 0;
    }
    virtual jobject CallObjectMethod(jobject SUPPRESS_UNUSED obj, jmethodID SUPPRESS_UNUSED methodID, ...)
    {
      return nullptr;
    }
    virtual void * GetPrimitiveArrayCritical(jarray SUPPRESS_UNUSED array, jboolean SUPPRESS_UNUSED *isCopy)
    {
      return nullptr;
    }
    virtual double* GetDoubleArrayElements(jdoubleArray SUPPRESS_UNUSED arr, bool  SUPPRESS_UNUSED *isCopy)
    {
      return nullptr;
    }
    virtual int* GetIntArrayElements(jintArray SUPPRESS_UNUSED arr, bool SUPPRESS_UNUSED *isCopy)
    {
      return nullptr;
    }
    virtual void ReleaseDoubleArrayElements(jdoubleArray SUPPRESS_UNUSED arr, double SUPPRESS_UNUSED *nativeArr, int SUPPRESS_UNUSED mode)
    {

    }
    virtual void ReleaseIntArrayElements(jintArray SUPPRESS_UNUSED arr, int SUPPRESS_UNUSED *nativeArr, int SUPPRESS_UNUSED mode)
    {

    }

    virtual void ReleasePrimitiveArrayCritical(jarray SUPPRESS_UNUSED array, void SUPPRESS_UNUSED *carray, SUPPRESS_UNUSED jint mode)
    {

    }
    virtual jsize GetArrayLength(jarray SUPPRESS_UNUSED array)
    {
      return 0;
    }
    virtual jobject GetObjectArrayElement(jobjectArray SUPPRESS_UNUSED array, jsize SUPPRESS_UNUSED index)
    {
      return nullptr;
    }
    virtual jlong GetLongField(jobject SUPPRESS_UNUSED obj, jfieldID SUPPRESS_UNUSED fieldID)
    {
      return 0;
    }
    virtual jfieldID GetFieldID(jclass SUPPRESS_UNUSED clazz, const char SUPPRESS_UNUSED *name, const char SUPPRESS_UNUSED *sig)
    {
      return &allFieldIds;
    }
    virtual jmethodID GetMethodID(jclass SUPPRESS_UNUSED clazz, const char SUPPRESS_UNUSED *name, const char SUPPRESS_UNUSED *sig)
    {
      return nullptr;
    }
    virtual jobject NewGlobalRef(jobject SUPPRESS_UNUSED lobj)
    {
      return &allGlobalRefs;
    }
    virtual jobjectArray NewObjectArray(jsize SUPPRESS_UNUSED len, jclass SUPPRESS_UNUSED clazz, jobject SUPPRESS_UNUSED init)
    {
      return nullptr;
    }
    virtual jdoubleArray NewDoubleArray(jsize SUPPRESS_UNUSED len)
    {
      return nullptr;
    }
    virtual jstring NewStringUTF(const char SUPPRESS_UNUSED *bytes)
    {
      return nullptr;
    }
    virtual jint ThrowNew(jclass SUPPRESS_UNUSED clazz, const char SUPPRESS_UNUSED *msg)
    {
      return 0;
    }
    virtual jint Throw(jthrowable SUPPRESS_UNUSED throwable)
    {
      return 0;
    }
    virtual jintArray NewIntArray(jsize SUPPRESS_UNUSED len)
    {
      return nullptr;
    }
    virtual jclass FindClass(const char SUPPRESS_UNUSED *name)
    {
      return &allClasses;
    }
    virtual jboolean ExceptionCheck()
    {
      return JNI_FALSE;
    }
    virtual void ExceptionClear()
    {

    }
    virtual ~_JNIEnv()
    {

    };
};

typedef _JNIEnv JNIEnv;

class _JavaVM{
  public:
        virtual ~_JavaVM(){};
        virtual int GetEnv(void ** env, int version) = 0;
        virtual jint AttachCurrentThread(void **penv, void *args) = 0;
  protected:
    int _version;
    JNIEnv * _env;
};
typedef _JavaVM JavaVM;

typedef int jsize;

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved);

#endif
