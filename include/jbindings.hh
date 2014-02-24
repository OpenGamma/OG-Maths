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

/*
 * Binds some OGArray data in an OGArray object obj to a type T (T extends double)
 * FIXME: Not implemented. Is it needed?
 */
template <typename T>
T* bindOGArrayData(jobject obj);

/**
 * free (unbind) the data in an OGArray class from a nativeT pointer
 * @param nativeT the class of the native copy of the data
 * @param javaT the type of the java data
 * @param nativeData the native data to unbind
 * @param obj the java object from which the native data is pinned
 */
template <typename nativeT, typename javaT>
void unbindOGArrayData(nativeT* nativeData, jobject obj);

extern template void
unbindOGArrayData<real16, jdoubleArray>(real16* nativeData, jobject obj);

extern template void
unbindOGArrayData<complex16, jdoubleArray>(complex16* nativeData, jobject obj);

extern template void
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
nativeT* bindPrimitiveArrayData(jobject obj, jmethodID method);

extern template real16*
bindPrimitiveArrayData<real16, jdoubleArray>(jobject obj, jmethodID method);

extern template complex16*
bindPrimitiveArrayData<complex16, jdoubleArray>(jobject obj, jmethodID method);

extern template int*
bindPrimitiveArrayData<int, jintArray>(jobject obj, jmethodID method);

/**
 * free (unbind) the data in an OGArray class from a nativeT pointer
 * @param nativeT the type of the native copy of the data
 * @param javaT the type of the Java data
 * @param nativeData the native data to unbind
 * @param obj the java object from which the native data is pinned
 * @param method the method that refers to the pinned data
 */
template <typename nativeT, typename javaT>
void unbindPrimitiveArrayData(nativeT * nativeData, jobject obj, jmethodID method);

extern template void
unbindPrimitiveArrayData<real16, jdoubleArray>(real16* nativeData, jobject obj, jmethodID method);

extern template void
unbindPrimitiveArrayData<complex16, jdoubleArray>(complex16* nativeData, jobject obj, jmethodID method);

extern template void
unbindPrimitiveArrayData<jint, jintArray>(jint* nativeData, jobject obj, jmethodID method);

} // namespace convert

#endif // _JBINDINGS_HH
