#ifndef __ENGINE_RENDER_H__
#define __ENGINE_RENDER_H__

// Generic rendering methods, but not generic algorithms; these methods will be tailored towards the game

#include <GLFW/glfw3.h>
#include <linmath.h/linmath.h>

#include "types.h"

/// @brief All information regarding the game window
typedef struct render_state_t
{
  GLFWwindow* game_window;
  f32 game_window_width;
  f32 game_window_height;
} render_state;

/// @brief Set the game window's dimensinos, then call all render_init methods
void render_init(void);
/// @brief Clear the screen for the upcoming frame 
void render_begin(void);
/// @brief Poll for events and swap frame buffers 
void render_end(void);
/// @brief Use OpenGL methods to draw a quad
/// @param pos    {x, y}
/// @param size   {length, width}
/// @param color  {R, G, B, A}
void render_quad(vec2 pos, vec2 size, vec4 color);

#endif // __ENGINE_RENDER_H__