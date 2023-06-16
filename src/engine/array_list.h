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
} array_list;

array_list *array_list_create (usize item_size, usize initial_capacity);
usize array_list_append (array_list *list, void *item);
void *array_list_get (array_list *list, usize index);
u8 array_list_remove (array_list *list, usize index);

#endif // __ENGINE_ARRAY_LIST_H__