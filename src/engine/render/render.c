#include <glad/glad.h>

#include "../render.h"
#include "../global.h"
#include "render_internal.h"

render_state_internal state = {0};

void render_init(void)
{
  // set game window dimensions
  global.render.game_window_width = 800;
  global.render.game_window_height = 600;

  // call all render_init methods
  global.render.game_window = render_init_window(global.render.game_window_width, global.render.game_window_height);
  render_init_quad(&state.vao_quad, &state.vbo_quad, &state.ebo_quad);
}

void render_begin(void)
{
  glClearColor(0.08f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}


void render_end(void)
{
  glfwSwapBuffers(global.render.game_window);
  glfwPollEvents();
}


void render_quad(vec2 pos, vec2 size, vec4 color)
{
  glBindVertexArray(state.vao_quad);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

  glBindVertexArray(0);
}

