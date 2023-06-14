cmake_minimum_required(VERSION 3.24)
project(falconerd)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_path(LINMATH_INCLUDE_DIRS "linmath.h/linmath.h")

set(RENDER_SOURCE_FILES
  "src/engine/render/render_init.c"
  "src/engine/render/render_internal.h"
  "src/engine/render/render.c"
)

set(ENGINE_HEADER_FILES
  "src/engine/global.c"
  "src/engine/global.h"
  "src/engine/render.h"
  "src/engine/types.h"
  "src/engine/util.h"
)

set(SOURCE_FILES
  ${RENDER_SOURCE_FILES}
  ${ENGINE_HEADER_FILES}
  "src/main.c"
)

add_executable(mygame_glfw ${SOURCE_FILES})

target_link_libraries(mygame_glfw
PRIVATE
  glfw
  glad::glad
)

target_include_directories(mygame_glfw 
PRIVATE
  ${LINMATH_INCLUDE_DIRS}
)