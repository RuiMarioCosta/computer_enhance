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

add_library(multinop_loops_obj OBJECT
  multinop_loops.asm
)
set_source_files_properties(multinop_loops.asm PROPERTIES LANGUAGE ASM_NASM)
add_library(multinop_loops STATIC
  $<TARGET_OBJECTS:multinop_loops_obj>
)
set_target_properties(multinop_loops PROPERTIES
  LINKER_LANGUAGE CXX
)

add_library(conditional_nop_loops_obj OBJECT
  conditional_nop_loops.asm
)
set_source_files_properties(conditional_nop_loops.asm PROPERTIES LANGUAGE ASM_NASM)
add_library(conditional_nop_loops STATIC
  $<TARGET_OBJECTS:conditional_nop_loops_obj>
)
set_target_properties(conditional_nop_loops PROPERTIES
  LINKER_LANGUAGE CXX
)

add_library(jump_alignment_obj OBJECT
  jump_alignment.asm
)
set_source_files_properties(jump_alignment.asm PROPERTIES LANGUAGE ASM_NASM)
add_library(jump_alignment STATIC
  $<TARGET_OBJECTS:jump_alignment_obj>
)
set_target_properties(jump_alignment PROPERTIES
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
    multinop_loops
    conditional_nop_loops
    jump_alignment
)
target_include_directories(test_functions
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
