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
render_init_shaders (u32 *shader_default, u32 *shader_batch, f32 render_width, f32 render_height)
{
  mat4x4 projection_matrix;
  *shader_default = render_shader_create ("shaders/default.vert", "shaders/default.frag");
  *shader_batch = render_shader_create ("shaders/batch_quad.vert", "shaders/batch_quad.frag");

  mat4x4_ortho (projection_matrix, 0, render_width, 0, render_height, -2, 2);

  glUseProgram (*shader_default);
  // clang-format off
  glUniformMatrix4fv(
    glGetUniformLocation(*shader_default, "projection"),
    1,
    GL_FALSE,
    &projection_matrix[0][0]
  );
  
  glUseProgram (*shader_batch);
  // clang-format off
  glUniformMatrix4fv(
    glGetUniformLocation(*shader_batch, "projection"),
    1,
    GL_FALSE,
    &projection_matrix[0][0]
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

void
render_init_line (u32 *vao, u32 *vbo)
{
  glGenVertexArrays (1, vao);
  glBindVertexArray (*vao);

  glGenBuffers (1, vbo);
  glBindBuffer (GL_ARRAY_BUFFER, *vbo);
  glBufferData (GL_ARRAY_BUFFER, 6 * sizeof (f32), NULL, GL_DYNAMIC_DRAW);

  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof (f32), NULL);
  glEnableVertexAttribArray (0);
  glBindBuffer (GL_ARRAY_BUFFER, 0);

  glBindVertexArray (0);
}

void
render_init_batch_quads (u32 *vao_batch, u32 *vbo_batch, u32 *ebo_batch)
{
  glGenVertexArrays (1, vao_batch);
  glBindVertexArray (*vao_batch);

  u32 indices[MAX_BATCH_ELEMENTS];
  for (u32 i = 0, offset = 0; i < MAX_BATCH_ELEMENTS; i += 6, offset += 4) {
    indices[i + 0] = offset + 0;
    indices[i + 1] = offset + 1;
    indices[i + 2] = offset + 2;
    indices[i + 3] = offset + 2;
    indices[i + 4] = offset + 3;
    indices[i + 5] = offset + 0;
  }

  glGenBuffers (1, vbo_batch);
  glBindBuffer (GL_ARRAY_BUFFER, *vbo_batch);
  glBufferData (GL_ARRAY_BUFFER, MAX_BATCH_VERTICES * sizeof (batch_vertex), NULL, GL_DYNAMIC_DRAW);

  // [x,y]
  glEnableVertexAttribArray (0);
  glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof (batch_vertex), (void *)offsetof (batch_vertex, position));
  // [u,v]
  glEnableVertexAttribArray (1);
  glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, sizeof (batch_vertex), (void *)offsetof (batch_vertex, uvs));
  // [r, g, b, a]
  glEnableVertexAttribArray (2);
  glVertexAttribPointer (2, 4, GL_FLOAT, GL_FALSE, sizeof (batch_vertex), (void *)offsetof (batch_vertex, color));

  glGenBuffers (1, ebo_batch);
  glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, *ebo_batch);
  glBufferData (GL_ELEMENT_ARRAY_BUFFER, MAX_BATCH_ELEMENTS * sizeof (u32), indices, GL_STATIC_DRAW);

  glBindVertexArray (0);
  glBindBuffer (GL_ARRAY_BUFFER, 0);
  glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
}
