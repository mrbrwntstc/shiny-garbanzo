cmake_minimum_required(VERSION 3.24)
project(falconerd)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
    
set(SOURCE_FILES
  "src/main.c"
)

add_executable(mygame_glfw ${SOURCE_FILES})

target_link_libraries(mygame_glfw
PRIVATE
  glfw
  glad::glad
)
