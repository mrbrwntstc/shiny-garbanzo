#ifndef __ENGINE_PHYSICS_H__
#define __ENGINE_PHYSICS_H__

#include "types.h"
#include <linmath.h/linmath.h>
#include <stdbool.h>

typedef struct aabb_t {
  vec2 position;
  vec2 half_size;
} aabb;

typedef struct body_t {
  aabb aabb;
  vec2 velocity;
  vec2 acceleration;
} body;

void physics_init (void);
void physics_update (void);
usize physics_body_create (vec2 position, vec2 size);
body *physics_body_get (usize index);
bool physics_point_intersect_aabb (vec2 point, aabb aabb);
bool physics_aabb_intersect_aabb (aabb a, aabb b);
aabb aabb_minkowski_difference (aabb a, aabb b);
void aabb_penetration_vector (vec2 r, aabb aabb);

#endif // __ENGINE_PHYSICS_H__