#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#include "../global.h"
#include "../render.h"
#include "render_internal.h"

GLFWwindow *
render_init_window (f32 width, f32 height)
{
  if (!glfwInit ())
    exit (EXIT_FAILURE);

  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);

  GLFWwindow *window = glfwCreateWindow (width, height, "My OpenGL + GLFW Game", NULL, NULL);
  if (!window) {
    glfwTerminate ();
    exit (EXIT_FAILURE);
  }
  glfwMakeContextCurrent (window);

  if (!gladLoadGLLoader ((GLADloadproc)glfwGetProcAddress)) {
    glfwTerminate ();
    exit (EXIT_FAILURE);
  }

  puts ("OpenGL Loaded");
  printf ("Vendor:   %s\n", glGetString (GL_VENDOR));
  printf ("Renderer: %s\n", glGetString (GL_RENDERER));
  printf ("Version:  %s\n", glGetString (GL_VERSION));

  return window;
}

void
render_init_shaders (render_state_internal *state)
{
  state->shader_default = render_shader_create ("shaders/default.vert", "shaders/default.frag");

  mat4x4_ortho (state->projection_matrix, 0, global.render.game_window_width, 0, global.render.game_window_height, -2, 2);

  glUseProgram (state->shader_default);
  // clang-format off
  glUniformMatrix4fv(
    glGetUniformLocation(state->shader_default, "projection"),
    1,
    GL_FALSE,
    &state->projection_matrix[0][0]
  );
  // clang-format on
}

void
render_init_color_texture (u32 *texture)
{
  glGenTextures (1, texture);
  glBindTexture (GL_TEXTURE_2D, *texture);

  u8 solid_white[4] = { 255, 255, 255, 255 };
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, solid_white);

  glBindTexture (GL_TEXTURE_2D, 0);
}

void
render_init_quad (u32 *vao_quad, u32 *vbo_quad, u32 *ebo_quad)
{
  // clang-format off
  f32 vertices[] = {
    // x      y     z     u     v
    0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 
   -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
   -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
  };

  u32 indices[] = { 
    0, 1, 3,
    1, 2, 3
  };
  // clang-format on

  glGenVertexArrays (1, vao_quad);
  glGenBuffers (1, vbo_quad);
  glGenBuffers (1, ebo_quad);

  glBindVertexArray (*vao_quad);

  glBindBuffer (GL_ARRAY_BUFFER, *vbo_quad);
  glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, *ebo_quad);
  glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), indices, GL_STATIC_DRAW);

  // xyz
  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof (f32), (void *)0);
  glEnableVertexAttribArray (0);
  // uv
  glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof (f32), (void *)(3 * sizeof (f32)));
  glEnableVertexAttribArray (1);

  glBindVertexArray (0);
}
