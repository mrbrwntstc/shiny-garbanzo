#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "../io.h"
#include "../types.h"
#include "../util.h"

// 20 MiB, can probably change this to a higher value without issue.
// Check your target platform.
#define IO_READ_CHUNK_SIZE 2097152
#define IO_READ_ERROR_GENERAL "Error reading filie: %s. errno: %d\n"
#define IO_READ_ERROR_MEMORY "Not enough free memory to read file: %s\n"

engine_file
io_file_read (const char *path)
{
  engine_file file = { .is_valid = false };

  FILE *fp = fopen (path, "rb");
  if (!fp || ferror (fp)) {
    ERROR_RETURN (file, IO_READ_ERROR_GENERAL, path, errno);
  }

  char *data = NULL;
  char *tmp;
  size_t bytes_read = 0;
  size_t size = 0;
  size_t current_chunk;

  while (true) {
    // resize the buffer?
    if (bytes_read + IO_READ_CHUNK_SIZE + 1 > size) {
      size = bytes_read + IO_READ_CHUNK_SIZE + 1;

      // overflow
      if (size <= bytes_read) {
        free (data);
        ERROR_RETURN (file, IO_READ_ERROR_MEMORY, path);
      }

      tmp = realloc (data, size);
      if (!tmp) {
        free (data);
        ERROR_RETURN (file, IO_READ_ERROR_MEMORY, path);
      }
      data = tmp;
    }

    current_chunk = fread (data + bytes_read, 1, IO_READ_CHUNK_SIZE, fp);
    if (current_chunk == 0) {
      break;
    }

    bytes_read += current_chunk;
  }

  // sanity check
  if (ferror (fp)) {
    ERROR_RETURN (file, IO_READ_ERROR_GENERAL, path, errno);
  }

  // null terminate
  tmp = realloc (data, bytes_read + 1);
  if (!tmp) {
    free (data);
    ERROR_RETURN (file, IO_READ_ERROR_MEMORY, path);
  }
  data = tmp;
  data[bytes_read] = '\0';

  file.is_valid = true;
  file.data = data;
  file.size = bytes_read;

  return file;
}

i32
io_file_write (void *buffer, size_t size, const char *path)
{
  FILE *fp = fopen (path, "wb");
  if (!fp || ferror (fp)) {
    ERROR_RETURN (1, "Cannot write to file %s", path);
  }

  size_t chunks_written = fwrite (buffer, size, 1, fp);
  fclose (fp);

  if (chunks_written != 1) {
    ERROR_RETURN (1, "Error writing to file %s\nExpected 1, got %zu", path, chunks_written);
  }

  return 0;
}