#ifndef __ENGINE_IO_H__
#define __ENGINE_IO_H__

// file manager for the engine and the game

#include <stdbool.h>
#include <stdlib.h>

#include "types.h"

/// @brief simple struct to retrieve data about a file
typedef struct engine_file_t {
  char *data;
  size_t size;
  bool is_valid;
} engine_file;

/// @brief store the data of a file into an instance of engine_file. Adapted from https://stackoverflow.com/a/44894946 (not the chosen answer) by Nominal Animal
/// @param path relative to the directory where the game is executed from
/// @return the engine_file object containing the data of the file
engine_file io_file_read (const char *path);

/// @brief Write buffer as a single chunk of size size to path
/// @param buffer engine_file.data
/// @param size   engine_file.size
/// @param path   relative to the directory where the game is executed from
/// @return success of the operation
i32 io_file_write (void *buffer, size_t size, const char *path);

#endif // __ENGINE_UTIL_H__