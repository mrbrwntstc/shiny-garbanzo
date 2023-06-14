#ifndef __ENGINE_RENDER_INTERNAL_H__
#define __ENGINE_RENDER_INTERNAL_H__

#include <GLFW/glfw3.h>

#include "../types.h"
#include "../render.h"

typedef struct render_state_internal_t
{
  u32 vao_quad;
  u32 vbo_quad;
  u32 ebo_quad;
} render_state_internal;

/// @brief Create the game window using GLFW
/// @param width  starting width
/// @param height starting height
/// @return Pointer to the game window
GLFWwindow* render_init_window(f32 width, f32 height);

/// @brief Initialize render_state_internal.vao_quad, render_state_internal.vbo_quad, and render_state_internal.ebo_quad
/// @param vao_quad render_state_internal.vao_quad 
/// @param vbo_quad render_state_internal.vbo_quad
/// @param ebo_quad render_state_internal.ebo_quad
void render_init_quad(u32 *vao_quad, u32 *vbo_quad, u32 *ebo_quad);

#endif // __ENGINE_RENDER_INTERNAL_H__