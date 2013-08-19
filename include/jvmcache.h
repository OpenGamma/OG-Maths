#ifndef jvmcache_h
#define jvmcache_h
#ifdef __cplusplus
extern "C"
#endif
jint registerGlobalClassReference(JNIEnv * env, const char * FQclassname, jclass * globalRef);
jint registerGlobalMethodReference(JNIEnv * env, jclass * globalRef, jmethodID * methodToSet, const char * methodName, const char * methodSignature);
#endif