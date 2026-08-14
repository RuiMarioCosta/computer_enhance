function(add_asm_static_library target_name source_file)
  add_library(${target_name}_obj OBJECT
    ${source_file}
  )
  set_source_files_properties(${source_file} PROPERTIES LANGUAGE ASM_NASM)
  add_library(${target_name} STATIC
    $<TARGET_OBJECTS:${target_name}_obj>
  )
  set_target_properties(${target_name} PROPERTIES
    LINKER_LANGUAGE CXX
  )
endfunction()

add_asm_static_library(nop_loop nop_loop.asm)
add_asm_static_library(multinop_loops multinop_loops.asm)
add_asm_static_library(conditional_nop_loops conditional_nop_loops.asm)
add_asm_static_library(jump_alignment jump_alignment.asm)
add_asm_static_library(rat rat.asm)
add_asm_static_library(read_unroll read_unroll.asm)
add_asm_static_library(read_widths read_widths.asm)

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
    rat
    read_unroll
    read_widths
)
target_include_directories(test_functions
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
