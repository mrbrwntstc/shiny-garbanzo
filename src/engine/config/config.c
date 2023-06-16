#include "../config.h"
#include "../global.h"
#include "../input.h"
#include "../io.h"
#include "../util.h"

/// https://www.glfw.org/docs/latest/group__keys.html
/// @brief default WASD controls if a config file is not present
static const char *config_default = "[controls]\n"
                                    "left = 65\n"
                                    "right = 68\n"
                                    "up = 87\n"
                                    "down = 83\n"
                                    "escape = 256\n"
                                    "\n";

static char tmp_buffer[20];

static char *
config_get_value (const char *config_buffer, const char *value)
{
  char *line = strstr (config_buffer, value);
  if (!line)
    ERROR_EXIT ("Could not find config value %s. Try deleting config.ini and try again.\n", value);

  size_t len = strlen (line);
  char *end = line + len;

  char *curr = line;
  char *tmp_ptr = &tmp_buffer[0];
  // move pointer to '='
  while (*curr != '=' && curr != end)
    ++curr;
  // consume '='
  ++curr;
  // consume any spaces
  while (*curr == ' ')
    ++curr;
  // get characters until the end of the line
  while (*curr != '\n' && *curr != '\0' && curr != end)
    *tmp_ptr++ = *curr++;
  // null terminate
  *(tmp_ptr + 1) = '\0';

  return tmp_buffer;
}

static void
load_controls (const char *config_buffer)
{
  config_key_bind (Input_Key_Left, config_get_value (config_buffer, "left"));
  config_key_bind (Input_Key_Right, config_get_value (config_buffer, "right"));
  config_key_bind (Input_Key_Up, config_get_value (config_buffer, "up"));
  config_key_bind (Input_Key_Down, config_get_value (config_buffer, "down"));
  config_key_bind (Input_Key_Escape, config_get_value (config_buffer, "escape"));
}

static u8
config_load (void)
{
  engine_file file_config = io_file_read ("./config.ini");
  if (!file_config.is_valid)
    return 1;

  load_controls (file_config.data);

  free (file_config.data);

  return 0;
}

void
config_init (void)
{
  if (config_load == 0)
    return;

  io_file_write ((void *)config_default, strlen (config_default), "./config.ini");

  if (config_load () != 0)
    ERROR_EXIT ("Failed to create or load config file.\n");
}

void
config_key_bind (input_key key, const char *key_name)
{
  i32 key_value = atoi (key_name);
  u8 scan_code = (u8)glfwGetKeyScancode (key_value);
  if (scan_code == GLFW_KEY_UNKNOWN)
    ERROR_RETURN (, "Unknown key: %s", key_name);

  global.config.keybinds[key] = scan_code;
}