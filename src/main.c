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
  if (scancode == global.config.keybinds[Input_Key_Left]) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
      global.input.left = Key_State_Pressed;
    } else {
      global.input.left = Key_State_Released;
    }
  }
  if (scancode == global.config.keybinds[Input_Key_Right]) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
      global.input.right = Key_State_Pressed;
    } else {
      global.input.right = Key_State_Released;
    }
  }
  if (scancode == global.config.keybinds[Input_Key_Up]) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
      global.input.up = Key_State_Pressed;
    } else {
      global.input.up = Key_State_Released;
    }
  }
  if (scancode == global.config.keybinds[Input_Key_Down]) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
      global.input.down = Key_State_Pressed;
    } else {
      global.input.down = Key_State_Released;
    }
  }
  if (scancode == global.config.keybinds[Input_Key_Escape] && action == GLFW_PRESS) {
    glfwSetWindowShouldClose (game_window, GLFW_TRUE);
  }
}

static vec2 pos;

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
  config_init ();
  time_init (60);
  physics_init ();

  u32 body_count = 100;
  for (u32 i = 0; i < body_count; ++i) {
    usize body_index = physics_body_create ((vec2){ rand () % (i32)global.render.game_window_width, rand () % (i32)global.render.game_window_height }, (vec2){ rand () % 100, rand () % 100 });
    body *body = physics_body_get (body_index);
    body->acceleration[0] = rand () % 200 - 100;
    body->acceleration[1] = rand () % 200 - 100;
  }

  pos[0] = global.render.game_window_width * 0.5f;
  pos[1] = global.render.game_window_height * 0.5f;

  while (!glfwWindowShouldClose (global.render.game_window)) {
    time_update ();

    input_handle ();
    physics_update ();

    render_begin ();
    // clang-format off
    render_quad (
      pos, 
      (vec2){ 50.0f, 50.0f }, 
      (vec4){ 0.0f, 1.0f, 0.0f, 1.0f }
    );

    for(u32 i = 0; i < body_count; ++i) {
      body *body = physics_body_get (i);
      render_quad(body->aabb.position, body->aabb.half_size, (vec4){ 1.0f, 0.0f, 0.0f, 1.0f });

      if(body->aabb.position[0] > global.render.game_window_width || body->aabb.position[0] < 0) {
        body->velocity[0] *= -1;
      }
      if(body->aabb.position[1] > global.render.game_window_height || body->aabb.position[1] < 0) {
        body->velocity[1] *= -1;
      }

      if(body->velocity[0] > 500)
        body->velocity[0] = 500;
      if(body->velocity[0] < -500)
        body->velocity[0] = -500;
      if(body->velocity[1] > 500)
        body->velocity[1] = 500;
      if(body->velocity[1] < -500)
        body->velocity[1] = -500;
    }
    // clang-format on
    render_end ();
    time_update_late ();
  }

  glfwDestroyWindow (global.render.game_window);
  glfwTerminate ();

  exit (EXIT_SUCCESS);
}
