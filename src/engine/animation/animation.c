#include <assert.h>

#include "../animation.h"
#include "../array_list.h"
#include "../util.h"

static Array_List *animation_definition_storage;
static Array_List *animation_storage;

void
animation_init (void)
{
  animation_definition_storage = array_list_create (sizeof (Animation_Definition), 0);
  animation_storage = array_list_create (sizeof (Animation), 0);
}

usize
animation_definition_create (Sprite_Sheet *sprite_sheet, f32 *duration, u8 *rows, u8 *columns, u8 frame_count)
{
  assert (frame_count <= MAX_FRAMES);

  Animation_Definition definition = { 0 };

  definition.sprite_sheet = sprite_sheet;
  definition.frame_count = frame_count;

  for (u8 i = 0; i < frame_count; i++) {
    definition.frames[i] = (Animation_Frame){ .column = columns[i], .row = rows[i], .duration = duration[i] };
  }

  return array_list_append (animation_definition_storage, &definition);
}

usize
animation_create (usize animation_definition_id, bool does_loop)
{
  usize id = animation_storage->len;
  Animation_Definition *adef = array_list_get (animation_definition_storage, animation_definition_id);
  if (adef == NULL) {
    ERROR_EXIT ("Animation_Definition with id %zu not found.", animation_definition_id);
  }

  // try to find free slot
  for (usize i = 0; i < animation_storage->len; i++) {
    Animation *animation = array_list_get (animation_storage, i);
    if (!animation->is_active) {
      id = i;
      break;
    }
  }

  if (id == animation_storage->len) {
    array_list_append (animation_storage, &(Animation){ 0 });
  }

  Animation *animation = array_list_get (animation_storage, id);

  // other fields default to 0 when using field dot system
  *animation = (Animation){ .definition = adef, .does_loop = does_loop, .is_active = true };

  return id;
}

void
animation_destroy (usize animation_id)
{
  Animation *animation = array_list_get (animation_storage, animation_id);
  animation->is_active = false;
}

Animation *
animation_get (usize animation_id)
{
  return array_list_get (animation_storage, animation_id);
}

void
animation_update (f32 dt)
{
  for (usize i = 0; i < animation_storage->len; i++) {
    Animation *animation = array_list_get (animation_storage, i);
    Animation_Definition *adef = animation->definition;
    animation->current_frame_time -= dt;

    if (animation->current_frame_time <= 0) {
      animation->current_frame_index += 1;

      // loop or stay on last frame
      if (animation->current_frame_index == animation->definition->frame_count) {
        if (animation->does_loop) {
          animation->current_frame_index = 0;
        } else {
          animation->current_frame_index -= 1;
        }
      }

      animation->current_frame_time = adef->frames[animation->current_frame_index].duration;
    }
  }
}
