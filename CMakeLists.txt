cmake_minimum_required(VERSION 3.24)
project(falconerd)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_path(LINMATH_INCLUDE_DIRS "linmath.h/linmath.h")

set(ANIMATION_SRC_FILES
  "src/engine/animation/animation.c"
)

set(ARRAY_LIST_SRC_FILES
  "src/engine/array_list/array_list.c"
)

set(CONFIG_SRC_FILES
  "src/engine/config/config.c"
)

set(ENTITY_SRC_FILES
  "src/engine/entity/entity.c"
)

set(IO_SOURCE_FILES
  "src/engine/io/io.c"
)

set(PHYSICS_SRC_FILES
  "src/engine/physics/physics_internal.h"
  "src/engine/physics/physics.c"
)

set(RENDER_SOURCE_FILES
  "src/engine/render/render_init.c"
  "src/engine/render/render_internal.h"
  "src/engine/render/render_util.c"
  "src/engine/render/render.c"
)

set(TIME_SRC_FILES
  "src/engine/time/time.c"
)

set(ENGINE_HEADER_FILES
  "src/engine/animation.h"
  "src/engine/array_list.h"
  "src/engine/config.h"
  "src/engine/entity.h"
  "src/engine/global.c"
  "src/engine/global.h"
  "src/engine/input.h"
  "src/engine/io.h"
  "src/engine/physics.h"
  "src/engine/time.h"
  "src/engine/render.h"
  "src/engine/stb_image.h"
  "src/engine/types.h"
  "src/engine/util.h"
)

set(SOURCE_FILES
  ${ANIMATION_SRC_FILES}
  ${ARRAY_LIST_SRC_FILES}
  ${CONFIG_SRC_FILES}
  ${ENTITY_SRC_FILES}
  ${IO_SOURCE_FILES}
  ${PHYSICS_SRC_FILES}
  ${RENDER_SOURCE_FILES}
  ${TIME_SRC_FILES}
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

include("cmake/clang-cxx-dev-tools.cmake")