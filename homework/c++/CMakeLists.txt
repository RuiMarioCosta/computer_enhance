add_subdirectory(haversine)
add_subdirectory(metrics)
add_subdirectory(profiler)
add_subdirectory(repetition_tester)
add_subdirectory(test_functions)
add_subdirectory(types)

add_executable(cpu_info cpu_info.cpp)

add_executable(haversine_generator
  haversine_generator.cpp
)
target_link_libraries(haversine_generator
  PRIVATE
    haversine_math
)

add_executable(json_parser
  json_parser.cpp
)
target_link_libraries(json_parser
  PRIVATE
    haversine_parser
)

add_executable(profile_haversine_main
  profile_haversine_main.cpp
)
target_link_libraries(profile_haversine_main
  PRIVATE
    haversine_parser
    profiler
)

function(add_repetition_tester_executable target_name source_file)
  add_executable(${target_name}
    ${source_file}
  )
  target_link_libraries(${target_name}
    PRIVATE
      repetition_tester
      metrics
      test_functions
  )
endfunction()

add_repetition_tester_executable(repetition_tester_readfile_main repetition_tester_readfile_main.cpp)
add_repetition_tester_executable(repetition_tester_nops_main repetition_tester_nops_main.cpp)
add_repetition_tester_executable(repetition_tester_conditional_nop_loops repetition_tester_conditional_nop_loops.cpp)
add_repetition_tester_executable(repetition_tester_jump_alignment repetition_tester_jump_alignment.cpp)
add_repetition_tester_executable(repetition_tester_rat repetition_tester_rat.cpp)
add_repetition_tester_executable(repetition_tester_read_unroll repetition_tester_read_unroll.cpp)
add_repetition_tester_executable(repetition_tester_read_widths repetition_tester_read_widths.cpp)

