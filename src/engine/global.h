#ifndef __ENGINE_GLOBAL_H__
#define __ENGINE_GLOBAL_H__

// game data that will be modified by different modules

#include "render.h"

typedef struct global_variables_t
{
  render_state render;
} global_variables;

extern global_variables global;

#endif // __ENGINE_GLOBAL_H__