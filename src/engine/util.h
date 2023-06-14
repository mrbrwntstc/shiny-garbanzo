#ifndef __ENGINE_UTIL_H__
#define __ENGINE_UTIL_H__

#include <stdio.h>
#include <stdlib.h>

// convenience macros for error handling

#define ERROR_EXIT(...)            \
{                                  \
  fprintf(stderr, __VA_ARGS__);    \
  exit(EXIT_FAILURE);              \
}

#define ERROR_RETURN(R, ...)      \
{                                 \
  fprintf(stderr, __VA_ARGS__);   \
  return R;                       \
}

#endif // __ENGINE_UTIL_H__
