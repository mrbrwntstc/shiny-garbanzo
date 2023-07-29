#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include "engine/animation.h"
#include "engine/config.h"
#include "engine/entity.h"
#include "engine/global.h"
#include "engine/input.h"
#include "engine/physics.h"
#include "engine/render.h"
#include "engine/time.h"
#include "engine/util.h"

static const f32 SPPED_ENEMY_LARGE = 200;
static const f32 SPEED_ENEMY_SMALL = 4000;
static const f32 HEALTH_ENEMY_LARGE = 7;
static const f32 HEALTH_ENEMY_SMALL = 3;

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
enemy_small_on_hit_static (Body *self, Static_Body *other, Hit hit)
{
  if (hit.normal[0] > 0) {
    self->velocity[0] = SPEED_ENEMY_SMALL;
  }

  if (hit.normal[0] < 0) {
    self->velocity[0] = -SPEED_ENEMY_SMALL;
  }
}

void
enemy_large_on_hit_static (Body *self, Static_Body *other, Hit hit)
{
  if (hit.normal[0] > 0) {
    self->velocity[0] = SPPED_ENEMY_LARGE;
  }

  if (hit.normal[0] < 0) {
    self->velocity[0] = -SPPED_ENEMY_LARGE;
  }
}

void
fire_on_hit (Body *self, Body *other, Hit hit)
{
  if (other->collision_layer == Collision_Layer_Enemy) {
    for (usize i = 0; i < entity_count (); i++) {
      Entity *entity = entity_get (i);

      if (entity->body_id == hit.other_id) {
        Body *body = physics_body_get (entity->body_id);
        body->is_active = false;
        entity->is_active = false;
        break;
      }
    }
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
  animation_init ();

  u8 enemy_mask = Collision_Layer_Player | Collision_Layer_Terrain;
  u8 player_mask = Collision_Layer_Enemy | Collision_Layer_Terrain;
  u8 fire_mask = Collision_Layer_Enemy | Collision_Layer_Player;

  usize player_id = entity_create ((vec2){ 100, 200 }, (vec2){ 24, 24 }, (vec2){ 0, 0 }, Collision_Layer_Player, player_mask, false, player_on_hit, player_on_hit_static);

  i32 game_window_width, game_window_height;
  glfwGetWindowSize (game_window, &game_window_width, &game_window_height);
  f32 width = game_window_width / render_get_scale ();
  f32 height = game_window_height / render_get_scale ();

  u32 static_body_a_id = physics_static_body_create ((vec2){ width * 0.5 - 12.5, height - 12.5 }, (vec2){ width - 25, 25 }, Collision_Layer_Terrain);
  u32 static_body_b_id = physics_static_body_create ((vec2){ width - 12.5, height * 0.5 + 12.5 }, (vec2){ 25, height - 25 }, Collision_Layer_Terrain);
  u32 static_body_c_id = physics_static_body_create ((vec2){ width * 0.5 + 12.5, 12.5 }, (vec2){ width - 25, 25 }, Collision_Layer_Terrain);
  u32 static_body_d_id = physics_static_body_create ((vec2){ 12.5, height * 0.5 - 12.5 }, (vec2){ 25, height - 25 }, Collision_Layer_Terrain);
  u32 static_body_e_id = physics_static_body_create ((vec2){ width * 0.5, height * 0.5 }, (vec2){ 62.5, 62.5 }, Collision_Layer_Terrain);

  usize entity_fire = entity_create ((vec2){ 370, 50 }, (vec2){ 25, 25 }, (vec2){ 0 }, 0, fire_mask, true, fire_on_hit, NULL);

  Sprite_Sheet sprite_sheet_player;
  render_sprite_sheet_init (&sprite_sheet_player, "assets/player.png", 24, 24);

  usize adef_player_walk_id = animation_definition_create (&sprite_sheet_player, (f32[]){ 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1 }, (u8[]){ 0, 0, 0, 0, 0, 0, 0 }, (u8[]){ 1, 2, 3, 4, 5, 6, 7 }, 7);

  usize adef_player_idle_id = animation_definition_create (&sprite_sheet_player, (f32[]){ 0 }, (u8[]){ 0 }, (u8[]){ 0 }, 1);
  usize anim_player_walk_id = animation_create (adef_player_walk_id, true);
  usize anim_plyer_idle_id = animation_create (adef_player_idle_id, false);

  f32 spawn_timer = 0;

  while (!glfwWindowShouldClose (game_window)) {
    time_update ();

    Entity *player = entity_get (player_id);
    Body *body_player = physics_body_get (player->body_id);

    if (body_player->velocity[0] != 0) {
      player->animation_id = anim_player_walk_id;
    } else {
      player->animation_id = anim_plyer_idle_id;
    }

    Static_Body *static_body_a = physics_static_body_get (static_body_a_id);
    Static_Body *static_body_b = physics_static_body_get (static_body_b_id);
    Static_Body *static_body_c = physics_static_body_get (static_body_c_id);
    Static_Body *static_body_d = physics_static_body_get (static_body_d_id);
    Static_Body *static_body_e = physics_static_body_get (static_body_e_id);

    input_handle (body_player);
    physics_update ();
    animation_update (global.time.dt);

    // spawn enemies
    {
      spawn_timer -= global.time.dt;
      if (spawn_timer <= 0) {
        spawn_timer = (f32)((rand () % 200) + 200) / 100.f;
        spawn_timer *= 0.2;

        for (u32 i = 0; i < 50; ++i) {

          bool is_flipped = rand () % 100 >= 50;

          f32 spawn_x = is_flipped ? 540 : 100;

          // clang-format off
          usize entity_id = entity_create(
            (vec2){spawn_x, 200},
            (vec2){20, 20},
            (vec2){0, 0},
            Collision_Layer_Enemy,
            enemy_mask,
            false,
            NULL,
            enemy_small_on_hit_static
          );
          // clang-format on
          Entity *entity = entity_get (entity_id);
          Body *body = physics_body_get (entity->body_id);
          float speed = SPEED_ENEMY_SMALL * ((rand () % 100) * 0.01) + 100;
          body->velocity[0] = is_flipped ? -speed : speed;
        }
      }
    }

    render_begin ();

    for (usize i = 0; i < entity_count (); i++) {
      Entity *entity = entity_get (i);
      Body *body = physics_body_get (entity->body_id);

      if (body->is_active) {
        render_aabb ((f32 *)body, TURQUOISE);
      } else {
        render_aabb ((f32 *)body, RED);
      }
    }

    render_aabb ((f32 *)static_body_a, WHITE);
    render_aabb ((f32 *)static_body_b, WHITE);
    render_aabb ((f32 *)static_body_c, WHITE);
    render_aabb ((f32 *)static_body_d, WHITE);
    render_aabb ((f32 *)static_body_e, WHITE);
    render_aabb ((f32 *)body_player, player_color);

    // render animated entities
    for (usize i = 0; i < entity_count (); i++) {
      Entity *entity = entity_get (i);
      if (entity->animation_id == (usize)-1) {
        continue;
      }

      Body *body = physics_body_get (entity->body_id);
      Animation *anim = animation_get (entity->animation_id);
      Animation_Definition *adef = anim->definition;
      Animation_Frame *aframe = &adef->frames[anim->current_frame_index];

      if (body->velocity[0] < 0) {
        anim->is_flipped = true;
      } else if (body->velocity[0] > 0) {
        anim->is_flipped = false;
      }

      render_sprite_sheet_frame (adef->sprite_sheet, aframe->row, aframe->column, body->aabb.position, anim->is_flipped);
    }

    render_sprite_sheet_frame (&sprite_sheet_player, 1, 2, (vec2){ 100, 100 }, false);
    render_sprite_sheet_frame (&sprite_sheet_player, 0, 4, (vec2){ 200, 200 }, false);

    render_end (game_window, sprite_sheet_player.texture_id);

    player_color[0] = 0;
    player_color[2] = 1;

    time_update_late ();
  }

  glfwDestroyWindow (game_window);
  glfwTerminate ();

  exit (EXIT_SUCCESS);
}
