#ifndef __ENGINE_CONFIG_H__
#define __ENGINE_CONFIG_H__

// allow custom key binds

#include "input.h"
#include "types.h"

/// @brief Store the scan code of each input key. Scan code are platform-specific;
// when searching for scan codes, make sure to specify the operating system.
typedef struct config_state_t {
  u8 keybinds[5];
} config_state;

void config_init (void);
void config_key_bind (input_key key, const char *key_name);

#endif // __ENGINE_CONFIG_H__