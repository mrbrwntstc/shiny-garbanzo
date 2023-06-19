#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include "engine/config.h"
#include "engine/global.h"
#include "engine/input.h"
#include "engine/physics.h"
#include "engine/time.h"
#include "engine/util.h"

static void
glfw_error_callback (int error, const char *description)
{
  fprintf (stderr, "Glfw Error %d: %s\n", error, description);
}

static void
glfw_framebuffer_size_callback (GLFWwindow *window, int width, int height)
{
  glViewport (0, 0, width, height);
}

static u8 key_press = GLFW_FALSE;

static void
glfw_key_callback (GLFWwindow *game_window, i32 key, i32 scancode, i32 action, i32 mods)
{
  // if (scancode == global.config.keybinds[Input_Key_Left]) {
  //   if (action == GLFW_PRESS || action == GLFW_REPEAT) {
  //     global.input.left = Key_State_Pressed;
  //   } else {
  //     global.input.left = Key_State_Released;
  //   }
  // }
  // if (scancode == global.config.keybinds[Input_Key_Right]) {
  //   if (action == GLFW_PRESS || action == GLFW_REPEAT) {
  //     global.input.right = Key_State_Pressed;
  //   } else {
  //     global.input.right = Key_State_Released;
  //   }
  // }
  // if (scancode == global.config.keybinds[Input_Key_Up]) {
  //   if (action == GLFW_PRESS || action == GLFW_REPEAT) {
  //     global.input.up = Key_State_Pressed;
  //   } else {
  //     global.input.up = Key_State_Released;
  //   }
  // }
  // if (scancode == global.config.keybinds[Input_Key_Down]) {
  //   if (action == GLFW_PRESS || action == GLFW_REPEAT) {
  //     global.input.down = Key_State_Pressed;
  //   } else {
  //     global.input.down = Key_State_Released;
  //   }
  // }
  if (scancode == global.config.keybinds[Input_Key_Escape] && action == GLFW_PRESS) {
    glfwSetWindowShouldClose (game_window, GLFW_TRUE);
  }
}

static vec2 pos;

static void
glfw_cursor_position_callback (GLFWwindow *game_window, f64 xpos, f64 ypos)
{
  pos[0] = (f32)xpos;
  pos[1] = global.render.game_window_height - (f32)ypos;
}

static void
input_handle (void)
{
  if (global.input.left == Key_State_Pressed) {
    pos[0] -= 500.0f * global.time.dt;
  }
  if (global.input.right == Key_State_Pressed) {
    pos[0] += 500.0f * global.time.dt;
  }
  if (global.input.up == Key_State_Pressed) {
    pos[1] += 500.0f * global.time.dt;
  }
  if (global.input.down == Key_State_Pressed) {
    pos[1] -= 500.0f * global.time.dt;
  }
}

int
main (void)
{
  glfwSetErrorCallback (glfw_error_callback);

  render_init ();
  glfwSetFramebufferSizeCallback (global.render.game_window, glfw_framebuffer_size_callback);
  glfwSetKeyCallback (global.render.game_window, glfw_key_callback);
  glfwSetCursorPosCallback (global.render.game_window, glfw_cursor_position_callback);
  glfwSetInputMode (global.render.game_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  config_init ();
  time_init (60);
  physics_init ();

  pos[0] = global.render.game_window_width * 0.5f;
  pos[1] = global.render.game_window_height * 0.5f;

  aabb test_aabb = { .position = { global.render.game_window_width * 0.5, global.render.game_window_height * 0.5 }, .half_size = { 50, 50 } };

  while (!glfwWindowShouldClose (global.render.game_window)) {
    time_update ();

    input_handle ();
    physics_update ();

    render_begin ();

    render_aabb ((f32 *)&test_aabb, (vec4){ 1, 1, 1, 0.5 });

    if (physics_point_intersect_aabb (pos, test_aabb)) {
      render_quad (pos, (vec2){ 5, 5 }, RED);
    } else {
      render_quad (pos, (vec2){ 5, 5 }, WHITE);
    }

    render_end ();

    time_update_late ();
  }

  glfwDestroyWindow (global.render.game_window);
  glfwTerminate ();

  exit (EXIT_SUCCESS);
}
