#include <linmath.h/linmath.h>

#include "../array_list.h"
#include "../global.h"
#include "../physics.h"
#include "../util.h"
#include "physics_internal.h"

static physics_state_internal state;

void
physics_init (void)
{
  state.body_list = array_list_create (sizeof (body), 0);
}

void
physics_update (void)
{
  body *body;

  for (u32 i = 0; i < state.body_list->len; i++) {
    body = array_list_get (state.body_list, i);
    body->velocity[0] += body->acceleration[0] * global.time.dt;
    body->velocity[1] += body->acceleration[1] * global.time.dt;
    body->aabb.position[0] += body->velocity[0] * global.time.dt;
    body->aabb.position[1] += body->velocity[1] * global.time.dt;
  }
}

usize
physics_body_create (vec2 position, vec2 size)
{
  body body = {
    .aabb = {
      .position = { position[0], position[1] },
      .half_size = {size[0] * 0.5f, size[1] * 0.5f},
    },
    .velocity = { 0.0f, 0.0f },
  };

  if (array_list_append (state.body_list, &body) == (usize)-1)
    ERROR_EXIT ("Could not append body to array list.\n");

  return state.body_list->len - 1;
}

body *
physics_body_get (usize index)
{
  return array_list_get (state.body_list, index);
}

void
aabb_min_max (vec2 min, vec2 max, aabb aabb)
{
  vec2_sub (min, aabb.position, aabb.half_size);
  vec2_add (max, aabb.position, aabb.half_size);
}

bool
physics_point_intersect_aabb (vec2 point, aabb aabb)
{
  vec2 min, max;
  aabb_min_max (min, max, aabb);

  return point[0] >= min[0] && point[0] <= max[0] && point[1] >= min[1] && point[1] <= max[1];
}