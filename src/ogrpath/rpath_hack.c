#include <jni.h>
#include "rpath_hack.h"
#include "warningmacros.h"

#ifdef __MINGW32__

#ifdef __cplusplus
extern "C"
#endif
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void SUPPRESS_UNUSED *reserved)
{
     printf("OnLoad called for rpath_hack\n"); 
     
     rpath_hack();
    
     JNIEnv *env=NULL;
     if ((*jvm)->GetEnv(jvm, (void **)&env, JNI_VERSION_1_2)) {
         return JNI_ERR; /* JNI version not supported */
     }
     return JNI_VERSION_1_2;    
}

int varInThisLib = 0;

#ifdef __cplusplus
extern "C"
#endif
// gets the handle to *the current* module, use address of this lib and don't increment ref count
HMODULE getThisLibHandle() {
    HMODULE lib = NULL;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS|GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCTSTR)&varInThisLib, &lib);
    return lib;
}

#ifdef __cplusplus
extern "C"
#endif
// This is un peut hacky.
// We unpack all libs to a temporary location in a flat structure.
// System.load() can be told to find a given wrapper lib based on some predefined path, so
// essentially we can get send a thread into the native wrapper lib. This is where the fun starts.
// On linux/osx we link with -Wl,-rpath so that the wrapper library knows where it's dependencies
// are in a relative sense (we put them in the same place as the wrapper lib).
// On windows there is no such joy, either the %PATH% needs mangling or they have to be somewhere
// that's windows special or they have to be in the dir where execution is taking place.
// We use this hack:
// 1) Unpack all the libs to a temporary location
// 2) Locate the appropriate wrapper lib and do System.load() on that lib
// 3) As the load() takes place JNI_OnLoad() will be called
// 4) During JNI_OnLoad() we invoke the rpath_hack() function below, this function
//    a) gets a handle to the library it is in
//    b) derefs the handle to an abolute path
//    c) mangles the path appropriately and adds said path to the windows search path
// 5) As a result of 4) we now have rpath behaviour for a flat lib structure,
///   undefined links can now be resolved

void rpath_hack() {
        printf("RUNNING RPATH HACK");
#define _DEBUG
    char libpathbuf[FILENAME_MAX]; // path buffer
    HMODULE thislibname = getThisLibHandle(); // get *this* module handle
    if(!thislibname) {
#ifdef _DEBUG
        printf("Failed to get handle for current module.\n");
#endif // _DEBUG        
        exit(-1);
    }
    // get file name for the handle e.g. SOME_PATH\module.dll
    int libpath_len = GetModuleFileName(thislibname ,libpathbuf,FILENAME_MAX);
    if(libpath_len<=0) {
#ifdef _DEBUG
        printf("Failed to get module name from handle.\n");
#endif // _DEBUG           
        exit(-1);
    }
    if(libpath_len+1>=FILENAME_MAX) { // catch potential buffer overflow
#ifdef _DEBUG
        printf("NUL termination of libpath name will result in buffer overflow.\n");
#endif // _DEBUG           
        exit(-1);
    }
#ifdef _DEBUG
    libpathbuf[libpath_len] = '\0'; // nul terminate compound string
    libpath_len++; // for nul terminate
    printf("Got hmodule name as: %s.\n",libpathbuf);
#endif // _DEBUG             

    // find last instance of the WINSLASH, this gives path to dir where currently executing lib is
    char * last = strrchr(libpathbuf, WINSLASH);
    *last='\0'; // set last WINSLASH to nul
    // set dll dir to now also search for other libs where this lib is
    int setdllret = SetDllDirectory(libpathbuf);
    // check for fail
    if(!setdllret) {
#ifdef _DEBUG
        printf("attempted to set lib dir as %s and failed.\n",libpathbuf);
#endif // _DEBUG      
        exit(-1);
    } else {
#ifdef _DEBUG
        printf("setdlldir() as %s\n",libpathbuf);
#endif // _DEBUG            
    }
}

#endif // __MINGW32__
