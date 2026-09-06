add_subdirectory(haversine)
add_subdirectory(metrics)
add_subdirectory(profiler)
add_subdirectory(repetition_tester)
add_subdirectory(test_functions)
add_subdirectory(types)

add_executable(cpu_info cpu_info.cpp)

add_executable(001_haversine_generator
  001_haversine_generator.cpp
)
target_link_libraries(001_haversine_generator
  PRIVATE
    haversine_math
)

add_executable(002_json_parser
  002_json_parser.cpp
)
target_link_libraries(002_json_parser
  PRIVATE
    haversine_parser
)

add_executable(003_profile_haversine_main
  003_profile_haversine_main.cpp
)
target_link_libraries(003_profile_haversine_main
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

add_repetition_tester_executable(004_repetition_tester_readfile_main 004_repetition_tester_readfile_main.cpp)
add_repetition_tester_executable(005_repetition_tester_nops_main 005_repetition_tester_nops_main.cpp)
add_repetition_tester_executable(006_repetition_tester_conditional_nop_loops 006_repetition_tester_conditional_nop_loops.cpp)
add_repetition_tester_executable(007_repetition_tester_jump_alignment 007_repetition_tester_jump_alignment.cpp)
add_repetition_tester_executable(008_repetition_tester_rat 008_repetition_tester_rat.cpp)
add_repetition_tester_executable(009_repetition_tester_read_unroll 009_repetition_tester_read_unroll.cpp)
add_repetition_tester_executable(010_repetition_tester_read_widths 010_repetition_tester_read_widths.cpp)
add_repetition_tester_executable(011_repetition_tester_cache_test 011_repetition_tester_cache_test.cpp)

