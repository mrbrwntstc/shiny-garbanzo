#ifndef __ENGINE_RENDER_H__
#define __ENGINE_RENDER_H__

// Generic rendering methods, but not generic algorithms; these methods will be tailored towards the game

#include <GLFW/glfw3.h>
#include <linmath.h/linmath.h>

#include "types.h"

typedef struct batch_vertex_t {
  vec2 position;
  vec2 uvs;
  vec4 color;
} batch_vertex;

#define MAX_BATCH_QUADS 10000
#define MAX_BATCH_VERTICES 40000
#define MAX_BATCH_ELEMENTS 60000

GLFWwindow *render_init (void);
void render_begin (void);
void render_end (GLFWwindow *game_window);
void render_quad (vec2 pos, vec2 size, vec4 color);
void render_quad_line (vec2 pos, vec2 size, vec4 color);
void render_line_segment (vec2 start, vec2 end, vec4 color);
void render_aabb (f32 *aabb, vec4 color);
f32 render_get_scale ();

void append_quad (vec2 position, vec2 size, vec4 texture_coordinates, vec4 color);

#endif // __ENGINE_RENDER_H__
