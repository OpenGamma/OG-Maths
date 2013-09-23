/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _JBINDINGS_HH
#define _JBINDINGS_HH

#include "jni.h"
#include "numerictypes.hh"

namespace convert {

/*
 * Binds some OGArray data in an OGArray object obj to a type T (T extends double)
 * FIXME: Not implemented. Is it needed?
 */
template <typename T>
T* bindOGArrayData(jobject obj);

/**
 * free (unbind) the data in an OGArray class from a T pointer
 * @param T the class of the nativeData
 * @param nativeData the native data to unbind
 * @param obj the java object from which the native data is pinned
 */
template <typename T>
void unbindOGArrayData(T* nativeData, jobject obj);

extern template void
unbindOGArrayData<real16>(real16* nativeData, jobject obj);

extern template void
unbindOGArrayData<complex16>(complex16* nativeData, jobject obj);

/**
 * binds the data in an OGArray class to a T pointer
 * @param T the class of the underlying data
 * @param S the class representation in the pinning call
 * @param obj the object from which the data shall be extracted
 * @param method the method to "get" the native data reference
 */
template <typename T, typename S>
T* bindPrimitiveArrayData(jobject obj, jmethodID method);

extern template real16*
bindPrimitiveArrayData<real16, jdoubleArray>(jobject obj, jmethodID method);

extern template complex16*
bindPrimitiveArrayData<complex16, jdoubleArray>(jobject obj, jmethodID method);

extern template int*
bindPrimitiveArrayData<int, jintArray>(jobject obj, jmethodID method);

/**
 * free (unbind) the data in an OGArray class from a T pointer
 * @param T the class of the nativeData
 * @param S the class representation in the pinning call
 * @param nativeData the native data to unbind
 * @param obj the java object from which the native data is pinned
 * @param method the method that refers to the pinned data
 */
template <typename T, typename S>
void unbindPrimitiveArrayData(T * nativeData, jobject obj, jmethodID method);

extern template void
unbindPrimitiveArrayData<real16, jdoubleArray>(real16* nativeData, jobject obj, jmethodID method);

extern template void
unbindPrimitiveArrayData<complex16, jdoubleArray>(complex16* nativeData, jobject obj, jmethodID method);

extern template void
unbindPrimitiveArrayData<int, jintArray>(int* nativeData, jobject obj, jmethodID method);

} // namespace convert

#endif // _JBINDINGS_HH
