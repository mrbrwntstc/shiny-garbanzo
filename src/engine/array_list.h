#ifndef __ENGINE_ARRAY_LIST_H__
#define __ENGINE_ARRAY_LIST_H__

// an array-backed list
// convenience methods of list data strucutres, but with the iteration speed of arrays

#include "types.h"

/// @brief an array-backed list
typedef struct array_list_t {
  usize len;
  usize capacity;
  usize item_size;
  void *items;
} Array_List;

Array_List *array_list_create (usize item_size, usize initial_capacity);
usize array_list_append (Array_List *list, void *item);
void *array_list_get (Array_List *list, usize index);
u8 array_list_remove (Array_List *list, usize index);

#endif // __ENGINE_ARRAY_LIST_H__