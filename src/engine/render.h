#ifndef __ENGINE_RENDER_H__
#define __ENGINE_RENDER_H__

// Generic rendering methods, but not generic algorithms; these methods will be tailored towards the game

#include <GLFW/glfw3.h>
#include <linmath.h/linmath.h>

#include "types.h"

GLFWwindow *render_init (void);
void render_begin (void);
void render_end (GLFWwindow *game_window);
void render_quad (vec2 pos, vec2 size, vec4 color);
void render_quad_line (vec2 pos, vec2 size, vec4 color);
void render_line_segment (vec2 start, vec2 end, vec4 color);
void render_aabb (f32 *aabb, vec4 color);
f32 render_get_scale ();

#endif // __ENGINE_RENDER_H__
