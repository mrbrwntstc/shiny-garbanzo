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

  // clang-format off

  aabb test_aabb = { 
    .position = { 
      global.render.game_window_width * 0.5,
      global.render.game_window_height * 0.5 
    },
    .half_size = { 50, 50 }
  };

  aabb cursor_aabb = { .half_size = { 75, 75 } };

  aabb start_aabb = {.half_size = {75,75}};

  aabb sum_aabb ={
    .position = {test_aabb.position[0], test_aabb.position[1]},
		.half_size = {
			test_aabb.half_size[0] + cursor_aabb.half_size[0],
			test_aabb.half_size[1] + cursor_aabb.half_size[1]
    }
  };

  // clang-format on

  while (!glfwWindowShouldClose (global.render.game_window)) {
    time_update ();

    input_handle ();
    physics_update ();

    i32 mouse_left_button_state = glfwGetMouseButton (global.render.game_window, GLFW_MOUSE_BUTTON_LEFT);
    if (mouse_left_button_state == GLFW_PRESS) {
      start_aabb.position[0] = pos[0];
      start_aabb.position[1] = pos[1];
    }

    render_begin ();

    cursor_aabb.position[0] = pos[0];
    cursor_aabb.position[1] = pos[1];

    render_aabb ((f32 *)&test_aabb, WHITE);

    vec4 faded = { 1, 1, 1, 0.3 };

    if (physics_aabb_intersect_aabb (test_aabb, cursor_aabb)) {
      render_aabb ((f32 *)&cursor_aabb, RED);
    } else {
      render_aabb ((f32 *)&cursor_aabb, WHITE);
    }

    render_aabb ((f32 *)&start_aabb, faded);
    render_line_segment (start_aabb.position, pos, WHITE);

    f32 x = sum_aabb.position[0];
    f32 y = sum_aabb.position[1];
    f32 size = sum_aabb.half_size[0];

    render_line_segment ((vec2){ x - size, 0 }, (vec2){ x - size, global.render.game_window_height }, faded);
    render_line_segment ((vec2){ x + size, 0 }, (vec2){ x + size, global.render.game_window_height }, faded);
    render_line_segment ((vec2){ 0, y - size }, (vec2){ global.render.game_window_width, y - size }, faded);
    render_line_segment ((vec2){ 0, y + size }, (vec2){ global.render.game_window_width, y + size }, faded);

    vec2 min, max;
    aabb_min_max (min, max, sum_aabb);

    vec2 magnitude;
    vec2_sub (magnitude, pos, start_aabb.position);

    hit hit = ray_intersect_aabb (start_aabb.position, magnitude, sum_aabb);

    if (hit.is_hit) {
      aabb hit_aabb = { .position = { hit.position[0], hit.position[1] }, .half_size = { start_aabb.half_size[0], start_aabb.half_size[1] } };
      render_aabb ((f32 *)&hit_aabb, CYAN);
      render_quad (hit.position, (vec2){ 5, 5 }, CYAN);
    }

    for (u8 i = 0; i < 2; ++i) {
      if (magnitude[i] != 0) {
        f32 t1 = (min[i] - pos[i]) / magnitude[i];
        f32 t2 = (max[i] - pos[i]) / magnitude[i];

        vec2 point;
        vec2_scale (point, magnitude, t1);
        vec2_add (point, point, pos);
        if (min[i] < start_aabb.position[i])
          render_quad (point, (vec2){ 5, 5 }, ORANGE);
        else
          render_quad (point, (vec2){ 5, 5 }, CYAN);

        vec2_scale (point, magnitude, t2);
        vec2_add (point, point, pos);
        if (max[i] < start_aabb.position[i])
          render_quad (point, (vec2){ 5, 5 }, CYAN);
        else
          render_quad (point, (vec2){ 5, 5 }, ORANGE);
      }
    }

    render_end ();

    time_update_late ();
  }

  glfwDestroyWindow (global.render.game_window);
  glfwTerminate ();

  exit (EXIT_SUCCESS);
}
