#include <GLFW/glfw3.h>
#include <Windows.h>

#include "../global.h"
#include "../time.h"

void
time_init (u32 frame_rate)
{
  global.time.frame_rate = frame_rate;
  global.time.frame_delay = 1.0f / frame_rate;
}

void
time_update (void)
{
  global.time.now = (f32)glfwGetTime ();
  global.time.dt = global.time.now - global.time.last;
  global.time.last = global.time.now;
  global.time.frame_count += 1;

  if ((global.time.now - global.time.frame_last) >= 1.0f) {
    global.time.frame_rate = global.time.frame_count;
    global.time.frame_count = 0;
    global.time.frame_last = global.time.now;
  }
}

void
time_update_late (void)
{
  global.time.frame_time = (f32)glfwGetTime () - global.time.now;

  if (global.time.frame_delay > global.time.frame_time) {
    Sleep ((global.time.frame_delay - global.time.frame_time) * 1000);
  }
}
