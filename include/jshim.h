#ifndef jshim_h
#define jshim_h

/*
 * Cached static classes methods, fields and the JVM pointer.
 */
#ifdef __cplusplus
extern "C" {
#endif
extern JavaVM *JVMcache;
extern jclass OGNumericClazz;
extern jclass OGExprClazz;
extern jclass OGArrayClazz;
extern jclass OGTerminalClazz;
extern jclass OGScalarClazz;
extern jclass OGSparseMatrixClazz;
extern jmethodID OGTerminalClazz_getData;
extern jmethodID OGNumericClazz_getType;
extern jmethodID OGExprClazz_getExprs;
extern jmethodID OGExprClazz_getNExprs;
extern jmethodID OGArrayClazz_getRows;
extern jmethodID OGArrayClazz_getCols;
extern jmethodID OGSparseMatrixClazz_getColPtr;
extern jmethodID OGSparseMatrixClazz_getRowIdx;
extern jclass OGExprTypeEnumClazz;
extern jfieldID OGExprTypeEnumClazz__hashdefined;
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C"
#endif
void * instantiateJClassAsCXXClass(jobject obj);

#endif
