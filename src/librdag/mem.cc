//
// Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
//
// Please see distribution for license.
//


#ifndef __WIN32

#include <stdlib.h>
#include <errno.h>

#else

// This order is important for mingw-w64
// http://sourceforge.net/apps/trac/mingw-w64/wiki/Missing%20_aligned_malloc
#include <stdlib.h>
#include <intrin.h>
#include <malloc.h>
#include <windows.h>

#endif // __WIN32

#include <stdint.h>
#include <stdio.h>

#include "mem.h"

// Memory management functions

extern "C" void * malloc_aligned(size_t size)
{

uintptr_t * memptr = NULL;

#ifndef __WIN32
  int ret = posix_memalign((void**)&memptr, __ALIGNMENT, size);
  if(ret)
  {
    printf("Error: could not obtain aligned memory\n");
    if(ret==EINVAL)
    {
      printf("The alignment argument not a multiple of sizeof(void *) or a power of two\n");
    }
    else if (ret==ENOMEM)
    {
      printf("There was insufficient memory to meet the allocation requested\n");
    }
    exit(1);
  }

#else

  memptr = (uintptr_t*) _aligned_malloc (size, __ALIGNMENT);
  if(memptr == NULL)
  {
    printf("Error: could not obtain aligned memory\n");
    printf("_aligned_malloc() failed.\n");
    exit(1);
  }

#endif // __WIN32

return (void * )memptr;

}



extern "C" void free_aligned(void * mem)
{
#ifndef __WIN32
  free(mem);
#else
  _aligned_free(mem);
#endif
}

