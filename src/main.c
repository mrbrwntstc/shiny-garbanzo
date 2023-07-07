#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include "engine/config.h"
#include "engine/entity.h"
#include "engine/global.h"
#include "engine/input.h"
#include "engine/physics.h"
#include "engine/render.h"
#include "engine/time.h"
#include "engine/util.h"

// clang-format off
typedef enum collision_layer {
  Collision_Layer_Player = 1,
  Collision_Layer_Enemy = 1 << 1,
  Collision_Layer_Terrain = 1 << 2
} Collision_Layer;
// clang-format on

vec4 player_color = { 0, 1, 1, 1 };
bool player_is_grounded = true;

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

static void
input_handle (Body *body_player)
{
  f32 velx = 0;
  f32 vely = body_player->velocity[1];

  if (global.input.right) {
    velx += 600;
  }
  if (global.input.left) {
    velx -= 600;
  }
  if (global.input.up && player_is_grounded) {
    player_is_grounded = false;
    vely = 2000;
  }

  body_player->velocity[0] = velx;
  body_player->velocity[1] = vely;
}

void
player_on_hit (Body *self, Body *other, Hit hit)
{
  if (other->collision_layer == Collision_Layer_Enemy) {
    player_color[0] = 1;
    player_color[2] = 0;
  }
}

void
player_on_hit_static (Body *self, Static_Body *other, Hit hit)
{
  if (hit.normal[1] > 0) {
    player_is_grounded = true;
  }
}

void
enemy_on_hit_static (Body *self, Static_Body *other, Hit hit)
{
  if (hit.normal[0] > 0) {
    self->velocity[0] = 400;
  }

  if (hit.normal[0] < 0) {
    self->velocity[0] = -400;
  }
}

int
main (void)
{
  glfwSetErrorCallback (glfw_error_callback);

  GLFWwindow *game_window = render_init ();
  glfwSetFramebufferSizeCallback (game_window, glfw_framebuffer_size_callback);
  glfwSetKeyCallback (game_window, glfw_key_callback);
  glfwSetInputMode (game_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  config_init ();
  time_init (60);
  physics_init ();
  entity_init ();

  u8 enemy_mask = Collision_Layer_Player | Collision_Layer_Terrain;
  u8 player_mask = Collision_Layer_Enemy | Collision_Layer_Terrain;

  usize player_id = entity_create ((vec2){ 100, 200 }, (vec2){ 24, 24 }, (vec2){ 0, 0 }, Collision_Layer_Player, player_mask, player_on_hit, player_on_hit_static);

  i32 game_window_width, game_window_height;
  glfwGetWindowSize (game_window, &game_window_width, &game_window_height);
  f32 width = game_window_width / render_get_scale ();
  f32 height = game_window_height / render_get_scale ();

  u32 static_body_a_id = physics_static_body_create ((vec2){ width * 0.5 - 12.5, height - 12.5 }, (vec2){ width - 25, 25 }, Collision_Layer_Terrain);
  u32 static_body_b_id = physics_static_body_create ((vec2){ width - 12.5, height * 0.5 + 12.5 }, (vec2){ 25, height - 25 }, Collision_Layer_Terrain);
  u32 static_body_c_id = physics_static_body_create ((vec2){ width * 0.5 + 12.5, 12.5 }, (vec2){ width - 25, 25 }, Collision_Layer_Terrain);
  u32 static_body_d_id = physics_static_body_create ((vec2){ 12.5, height * 0.5 - 12.5 }, (vec2){ 25, height - 25 }, Collision_Layer_Terrain);
  u32 static_body_e_id = physics_static_body_create ((vec2){ width * 0.5, height * 0.5 }, (vec2){ 62.5, 62.5 }, Collision_Layer_Terrain);

  usize entity_a_id = entity_create ((vec2){ 200, 200 }, (vec2){ 25, 25 }, (vec2){ 400, 0 }, Collision_Layer_Enemy, enemy_mask, NULL, enemy_on_hit_static);
  usize entity_b_id = entity_create ((vec2){ 300, 300 }, (vec2){ 25, 25 }, (vec2){ 400, 0 }, 0, enemy_mask, NULL, enemy_on_hit_static);

  while (!glfwWindowShouldClose (game_window)) {
    time_update ();

    Entity *player = entity_get (player_id);
    Body *body_player = physics_body_get (player->body_id);
    Static_Body *static_body_a = physics_static_body_get (static_body_a_id);
    Static_Body *static_body_b = physics_static_body_get (static_body_b_id);
    Static_Body *static_body_c = physics_static_body_get (static_body_c_id);
    Static_Body *static_body_d = physics_static_body_get (static_body_d_id);
    Static_Body *static_body_e = physics_static_body_get (static_body_e_id);

    input_handle (body_player);
    physics_update ();

    render_begin ();

    render_aabb ((f32 *)static_body_a, WHITE);
    render_aabb ((f32 *)static_body_b, WHITE);
    render_aabb ((f32 *)static_body_c, WHITE);
    render_aabb ((f32 *)static_body_d, WHITE);
    render_aabb ((f32 *)static_body_e, WHITE);
    render_aabb ((f32 *)body_player, player_color);

    render_aabb ((f32 *)physics_body_get (entity_get (entity_a_id)->body_id), WHITE);
    render_aabb ((f32 *)physics_body_get (entity_get (entity_b_id)->body_id), WHITE);

    for (u32 i = 0; i < 10000; ++i) {
      vec4 color = { (rand () % 255) / 255.0, (rand () % 255) / 255.0, (rand () % 255) / 255.0, (rand () % 255) / 255.0 };
      append_quad ((vec2){ rand () % 640, rand () % 360 }, (vec2){ rand () % 100, rand () % 100 }, NULL, color);
    }

    render_end (game_window);

    player_color[0] = 0;
    player_color[2] = 1;

    time_update_late ();
  }

  glfwDestroyWindow (game_window);
  glfwTerminate ();

  exit (EXIT_SUCCESS);
}
