#ifndef __ENGINE_RENDER_H__
#define __ENGINE_RENDER_H__

// Generic rendering methods, but not generic algorithms; these methods will be tailored towards the game

#include <GLFW/glfw3.h>
#include <linmath.h/linmath.h>

#include "types.h"

typedef struct batch_vertex_t {
  vec2 position;
  vec2 uvs;
  vec4 color;
} batch_vertex;

typedef struct sprite_sheet {
  f32 width;
  f32 height;
  f32 cell_width;
  f32 cell_height;
  u32 texture_id;
} Sprite_Sheet;

#define MAX_BATCH_QUADS 10000
#define MAX_BATCH_VERTICES 40000
#define MAX_BATCH_ELEMENTS 60000

GLFWwindow *render_init (void);
void render_begin (void);
void render_end (GLFWwindow *game_window, u32 batch_texture_id);
void render_quad (vec2 pos, vec2 size, vec4 color);
void render_quad_line (vec2 pos, vec2 size, vec4 color);
void render_line_segment (vec2 start, vec2 end, vec4 color);
void render_aabb (f32 *aabb, vec4 color);
f32 render_get_scale ();

void render_sprite_sheet_init (Sprite_Sheet *sprite_sheet, const char *path, f32 cell_width, f32 cell_height);
void render_sprite_sheet_frame (Sprite_Sheet *sprite_sheet, f32 row, f32 column, vec2 position);

#endif // __ENGINE_RENDER_H__
