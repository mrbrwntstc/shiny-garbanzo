#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include "engine/global.h"

static void glfw_error_callback(int error, const char *description)
{
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void glfw_framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

int main(void)
{
  glfwSetErrorCallback(glfw_error_callback);
  
  render_init();
  glfwSetFramebufferSizeCallback(global.render.game_window, glfw_framebuffer_size_callback);

  while(!glfwWindowShouldClose(global.render.game_window))
  {
    render_begin();

    render_quad(
      (vec2) {global.render.game_window_width * 0.5f, global.render.game_window_height * 0.5f},
      (vec2) {50.0f, 50.0f},
      (vec4) {1.0f, 1.0f, 1.0f, 1.0f}
    );

    render_end();    
  }

  glfwDestroyWindow(global.render.game_window);
  glfwTerminate();

  exit(EXIT_SUCCESS);
}


