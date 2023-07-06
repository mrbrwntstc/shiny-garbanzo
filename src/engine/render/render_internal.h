#ifndef __ENGINE_RENDER_INTERNAL_H__
#define __ENGINE_RENDER_INTERNAL_H__

#include <GLFW/glfw3.h>

#include "../render.h"
#include "../types.h"

GLFWwindow *render_init_window (f32 width, f32 height);
void render_init_quad (u32 *vao_quad, u32 *vbo_quad, u32 *ebo_quad);
void render_init_color_texture (u32 *texture);
void render_init_shaders (u32 *shader_default, f32 render_width, f32 render_height);
u32 render_shader_create (const char *path_vertex_shader, const char *path_fragment_shader);
void render_init_line (u32 *vao, u32 *vbo);

#endif // __ENGINE_RENDER_INTERNAL_H__