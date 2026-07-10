add_library(nop_loop_obj OBJECT
  nop_loop.asm
)
set_source_files_properties(nop_loop.asm PROPERTIES LANGUAGE ASM_NASM)

add_library(nop_loop STATIC
  $<TARGET_OBJECTS:nop_loop_obj>
)
set_target_properties(nop_loop PROPERTIES
  LINKER_LANGUAGE CXX
)

add_library(test_functions
  test_functions.hpp
  write_to_all_bytes.cpp
  read_functions.cpp
  front_end_functions.cpp
)
target_link_libraries(test_functions
  PUBLIC
    repetition_tester
    nop_loop
)
target_include_directories(test_functions
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
