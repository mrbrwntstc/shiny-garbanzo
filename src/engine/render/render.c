#include <glad/glad.h>

#include "../global.h"
#include "../render.h"
#include "render_internal.h"

render_state_internal state = { 0 };

void
render_init (void)
{
  // set game window dimensions
  global.render.game_window_width = 800;
  global.render.game_window_height = 600;

  // call all render_init methods
  global.render.game_window = render_init_window (global.render.game_window_width, global.render.game_window_height);
  render_init_quad (&state.vao_quad, &state.vbo_quad, &state.ebo_quad);
  render_init_shaders (&state);
  render_init_color_texture (&state.texture_color);
}

void
render_begin (void)
{
  glClearColor (0.08f, 0.1f, 0.1f, 1.0f);
  glClear (GL_COLOR_BUFFER_BIT);
}

void
render_end (void)
{
  glfwSwapBuffers (global.render.game_window);
  glfwPollEvents ();
}

void
render_quad (vec2 pos, vec2 size, vec4 color)
{
  glUseProgram (state.shader_default);

  mat4x4 model;
  mat4x4_identity (model);

  mat4x4_translate (model, pos[0], pos[1], 0.0f);
  mat4x4_scale_aniso (model, model, size[0], size[1], 1.0f);

  glUniformMatrix4fv (glGetUniformLocation (state.shader_default, "model"), 1, GL_FALSE, &model[0][0]);
  glUniform4fv (glGetUniformLocation (state.shader_default, "color"), 1, &color[0]);

  glBindVertexArray (state.vao_quad);
  glBindTexture (GL_TEXTURE_2D, state.texture_color);
  glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glBindTexture (GL_TEXTURE_2D, 0);
  glBindVertexArray (0);
}
