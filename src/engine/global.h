#ifndef __ENGINE_GLOBAL_H__
#define __ENGINE_GLOBAL_H__

// game data that will be modified by different modules

#include "config.h"
#include "input.h"
#include "render.h"
#include "time.h"

typedef struct global_variables_t {
  config_state config;
  input_state input;
  time_state time;
} global_variables;

extern global_variables global;

#endif // __ENGINE_GLOBAL_H__