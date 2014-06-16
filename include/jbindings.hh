/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _JBINDINGS_HH
#define _JBINDINGS_HH

#include "jvmmanager.hh"
#include "numerictypes.hh"

namespace convert {

/**
 * free (unbind) the data in an OGArray class from a nativeT pointer
 * @param nativeT the class of the native copy of the data
 * @param javaT the type of the java data
 * @param nativeData the native data to unbind
 * @param obj the java object from which the native data is pinned
 */
template <typename nativeT, typename javaT>
DLLEXPORT_C void unbindOGArrayData(nativeT* nativeData, jobject obj);

extern template DLLEXPORT_C  void
unbindOGArrayData<real8, jdoubleArray>(real8* nativeData, jobject obj);

extern template DLLEXPORT_C  void
unbindOGArrayData<complex16, jdoubleArray>(complex16* nativeData, jobject obj);

// We must use jint instead of int4 because the definition of jint varies between int and long int
extern template DLLEXPORT_C  void
unbindOGArrayData<jint, jintArray>(jint* nativeData, jobject obj);

/**
 * binds the data in an OGArray class to a nativeT pointer
 * @param nativeT the type of the native copy of the data
 * @param javaT the type of the Java data
 * @param obj the object from which the data shall be extracted
 * @param method the method to "get" the native data reference
 * @return a pointer to a native version of the data
 */
template <typename nativeT, typename javaT>
DLLEXPORT_C nativeT* bindPrimitiveArrayData(jobject obj, jmethodID method);

extern template DLLEXPORT_C real8*
bindPrimitiveArrayData<real8, jdoubleArray>(jobject obj, jmethodID method);

extern template DLLEXPORT_C complex16*
bindPrimitiveArrayData<complex16, jdoubleArray>(jobject obj, jmethodID method);

// We must use jint instead of int4 because the definition of jint varies between int and long int
extern template DLLEXPORT_C jint*
bindPrimitiveArrayData<jint, jintArray>(jobject obj, jmethodID method);

/**
 * free (unbind) the data in an OGArray class from a nativeT pointer
 * @param nativeT the type of the native copy of the data
 * @param javaT the type of the Java data
 * @param nativeData the native data to unbind
 * @param obj the java object from which the native data is pinned
 * @param method the method that refers to the pinned data
 */
template <typename nativeT, typename javaT>
DLLEXPORT_C void unbindPrimitiveArrayData(nativeT * nativeData, jobject obj, jmethodID method);

extern template DLLEXPORT_C void
unbindPrimitiveArrayData<real8, jdoubleArray>(real8* nativeData, jobject obj, jmethodID method);

extern template DLLEXPORT_C void
unbindPrimitiveArrayData<complex16, jdoubleArray>(complex16* nativeData, jobject obj, jmethodID method);

// We must use jint instead of int4 because the definition of jint varies between int and long int
extern template DLLEXPORT_C void
unbindPrimitiveArrayData<jint, jintArray>(jint* nativeData, jobject obj, jmethodID method);

} // namespace convert

#endif // _JBINDINGS_HH
