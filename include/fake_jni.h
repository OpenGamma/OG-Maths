/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Java Runtime Interface.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation and Sun Microsystems, Inc.
 * Portions created by the Initial Developer are Copyright (C) 1993-1996
 * the Initial Developer. All Rights Reserved.
 *
 * Portions Copyright (C) 2013 OpenGamma Inc. and the
 *                             OpenGamma group of companies.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */


// this is a fake JNI.h for the purposes of testing
#ifndef _FAKE_JNI_HH
#define _FAKE_JNI_HH

#include "warningmacros.h"
#include "fake_jni_md.h"

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
#define JNI_ENOMEM       (-4)              /* not enough memory */
#define JNI_EEXIST       (-5)              /* VM already created */
#define JNI_EINVAL       (-6)              /* invalid arguments */

#define JNI_VERSION_1_2 0x00010002

typedef unsigned char   jboolean;
typedef unsigned short  jchar;
typedef short           jshort;
typedef float           jfloat;
typedef double          jdouble;
typedef jint            jsize;


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

typedef _jobject *jobject;
typedef _jclass *jclass;
typedef _jthrowable *jthrowable;
typedef _jstring *jstring;
typedef _jarray *jarray;
typedef _jbooleanArray *jbooleanArray;
typedef _jbyteArray *jbyteArray;
typedef _jcharArray *jcharArray;
typedef _jshortArray *jshortArray;
typedef _jintArray *jintArray;
typedef _jlongArray *jlongArray;
typedef _jfloatArray *jfloatArray;
typedef _jdoubleArray *jdoubleArray;
typedef _jobjectArray *jobjectArray;


class _jfieldID {
  public:
    _jfieldID(){};
    virtual ~_jfieldID(){};
};

typedef struct _jfieldID *jfieldID;

class _jmethodID {
  public:
    _jmethodID(){};
    virtual ~_jmethodID(){};
};
typedef class _jmethodID *jmethodID;


// This is a quick hack so the first jshim test doesn't blow up.
static _jclass allClasses;
static _jobject allGlobalRefs;
static _jfieldID allFieldIds;
static _jthrowable allThrows;


class JNIEnv_
{
  public:
    virtual jthrowable ExceptionOccurred() {return &allThrows; }
    virtual void ExceptionDescribe() {};
    virtual void SetDoubleArrayRegion(jdoubleArray SUPPRESS_UNUSED array, jsize SUPPRESS_UNUSED start, jsize SUPPRESS_UNUSED len, const jdouble SUPPRESS_UNUSED *buf)
    {
      
    }
    virtual void SetObjectArrayElement(jobjectArray SUPPRESS_UNUSED array, SUPPRESS_UNUSED jsize index, SUPPRESS_UNUSED jobject val)
    {
      
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
    virtual jclass FindClass(const char SUPPRESS_UNUSED *name)
    {
      return &allClasses;
    }
    virtual ~JNIEnv_()
    {
      
    };
};

typedef JNIEnv_ JNIEnv;

class JavaVM_{
  public:
        virtual ~JavaVM_(){};
        virtual int GetEnv(void ** env, int version) = 0;
        virtual jint AttachCurrentThread(void **penv, void *args) = 0;
  protected:
    int _version;
    JNIEnv * _env;
};
typedef JavaVM_ JavaVM;

typedef int jsize;

#ifdef __cplusplus
extern "C"
#endif
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved);

#endif
