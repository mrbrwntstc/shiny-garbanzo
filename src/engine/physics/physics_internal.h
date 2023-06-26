#ifndef __ENGINE_PHYSICS_INTERNALL_H__
#define __ENGINE_PHYSICS_INTERNALL_H__

#include "../array_list.h"
#include "../types.h"

typedef struct physics_state_internal_t {
  f32 gravity;
  f32 terminal_velocity;
  Array_List *body_list;
  Array_List *static_body_list;
} physics_state_internal;

#endif // __ENGINE_PHYSICS_INTERNALL_H__
