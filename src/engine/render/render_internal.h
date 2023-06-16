#ifndef __ENGINE_RENDER_INTERNAL_H__
#define __ENGINE_RENDER_INTERNAL_H__

#include <GLFW/glfw3.h>

#include "../render.h"
#include "../types.h"

typedef struct render_state_internal_t
{
  u32 vao_quad;
  u32 vbo_quad;
  u32 ebo_quad;

  u32 shader_default;
  u32 texture_color;
  mat4x4 projection_matrix;
} render_state_internal;

/// @brief Create the game window using GLFW
/// @param width  starting width
/// @param height starting height
/// @return Pointer to the game window
GLFWwindow *render_init_window (f32 width, f32 height);

/// @brief Initialize render_state_internal.vao_quad, render_state_internal.vbo_quad, and render_state_internal.ebo_quad
/// @param vao_quad render_state_internal.vao_quad
/// @param vbo_quad render_state_internal.vbo_quad
/// @param ebo_quad render_state_internal.ebo_quad
void render_init_quad (u32 *vao_quad, u32 *vbo_quad, u32 *ebo_quad);

/// @brief Initialize render_state_internal.texture_color
/// @param texture render_state_internal.texture_color
void render_init_color_texture (u32 *texture);

/// @brief Initialize render_state_internal.shader_default and render_state_internal.projection_matrix
/// @param state an instance of the render_state_internal struct
void render_init_shaders (render_state_internal *state);

/// @brief Create an OpenGL shader program
/// @param path_vertex_shader    vertex shader file path location
/// @param path_fragment_shader  fragment shader file path location
/// @return OpenGL shader program id
u32 render_shader_create (const char *path_vertex_shader, const char *path_fragment_shader);

#endif // __ENGINE_RENDER_INTERNAL_H__