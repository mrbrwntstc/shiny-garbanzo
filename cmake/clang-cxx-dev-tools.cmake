# https://www.labri.fr/perso/fleury/posts/programming/using-clang-tidy-and-clang-format.html

# Additional targets to perform clang-format
# Get all project files
file(GLOB_RECURSE
  ALL_CXX_SOURCE_FILES
  *.[ch]
)

# Adding clang-format 
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
  add_custom_target(clang-format
    COMMAND ${CLANG_FORMAT}
    -i
    -style=file
    ${ALL_CXX_SOURCE_FILES}
  )
endif(CLANG_FORMAT)