#include <glad/glad.h>
#include <stdio.h>

#include "../io.h"
#include "../util.h"
#include "render_internal.h"

u32
render_shader_create (const char *path_vertex_shader, const char *path_fragment_shader)
{
  i32 success;
  char log[512];

  engine_file file_vertex = io_file_read (path_vertex_shader);
  if (!file_vertex.is_valid) {
    free (file_vertex.data);
    ERROR_EXIT ("Error reading vertex shader %s\n", path_vertex_shader);
  }

  u32 shader_vertex = glCreateShader (GL_VERTEX_SHADER);
  glShaderSource (shader_vertex, 1, &file_vertex.data, NULL);
  glCompileShader (shader_vertex);
  glGetShaderiv (shader_vertex, GL_COMPILE_STATUS, &success);
  if (!success) {
    free (file_vertex.data);
    glGetShaderInfoLog (shader_vertex, 512, NULL, log);
    ERROR_EXIT ("Error compiling vertex shader: %s\n", log);
  }

  free (file_vertex.data);

  engine_file file_fragment = io_file_read (path_fragment_shader);
  if (!file_fragment.is_valid) {
    free (file_fragment.data);
    ERROR_EXIT ("Error reading fragment shader %s\n", path_fragment_shader);
  }

  u32 shader_fragment = glCreateShader (GL_FRAGMENT_SHADER);
  glShaderSource (shader_fragment, 1, &file_fragment.data, NULL);
  glCompileShader (shader_fragment);
  glGetShaderiv (shader_fragment, GL_COMPILE_STATUS, &success);
  if (!success) {
    free (file_fragment.data);
    glGetShaderInfoLog (shader_fragment, 512, NULL, log);
    ERROR_EXIT ("Error compiling fragment shader: %s\n", log);
  }

  u32 shader_program = glCreateProgram ();
  glAttachShader (shader_program, shader_vertex);
  glAttachShader (shader_program, shader_fragment);
  glLinkProgram (shader_program);
  glGetProgramiv (shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    free (file_fragment.data);
    glGetProgramInfoLog (shader_program, 512, NULL, log);
    ERROR_EXIT ("Error linking shader program: %s\n", log);
  }

  free (file_fragment.data);

  glDeleteShader (shader_vertex);
  glDeleteShader (shader_fragment);

  return shader_program;
}