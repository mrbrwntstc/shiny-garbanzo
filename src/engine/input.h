#ifndef __ENGINE_INPUT_H__
#define __ENGINE_INPUT_H__

// library-agnostic way to manage user input

// clang-format off

/// @brief all possible input actions
typedef enum input_key_t { 
  Input_Key_Up,
  Input_Key_Down,
  Input_Key_Left,
  Input_Key_Right,
  Input_Key_Escape
} input_key;

/// @brief some libraries offer more options, but it is not necessary. Key_State_Released must come first
typedef enum key_state_t {
  Key_State_Released,
  Key_State_Pressed
} key_state;

// clang-format on

/// @brief track which actions to perform
typedef struct input_state_t {
  key_state left;
  key_state right;
  key_state up;
  key_state down;
  key_state escape;
} input_state;

#endif // __ENGINE_INPUT_H__