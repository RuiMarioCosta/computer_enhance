add_subdirectory(haversine)
add_subdirectory(metrics)
add_subdirectory(profiler)
add_subdirectory(repetition_tester)
add_subdirectory(test_functions)
add_subdirectory(types)

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

add_executable(repetition_tester_readfile_main
  repetition_tester_readfile_main.cpp
)
target_link_libraries(repetition_tester_readfile_main
  PRIVATE
    repetition_tester
    metrics
    test_functions
)

add_executable(repetition_tester_nops_main
  repetition_tester_nops_main.cpp
)
target_link_libraries(repetition_tester_nops_main
  PRIVATE
    repetition_tester
    metrics
    test_functions
)

add_executable(repetition_tester_conditional_nop_loops
  repetition_tester_conditional_nop_loops.cpp
)
target_link_libraries(repetition_tester_conditional_nop_loops
  PRIVATE
    repetition_tester
    metrics
    test_functions
)

add_executable(repetition_tester_jump_alignment
  repetition_tester_jump_alignment.cpp
)
target_link_libraries(repetition_tester_jump_alignment
  PRIVATE
    repetition_tester
    metrics
    test_functions
)

add_executable(repetition_tester_rat
  repetition_tester_rat.cpp
)
target_link_libraries(repetition_tester_rat
  PRIVATE
    repetition_tester
    metrics
    test_functions
)

add_executable(repetition_tester_read_unroll
  repetition_tester_read_unroll.cpp
)
target_link_libraries(repetition_tester_read_unroll
  PRIVATE
    repetition_tester
    metrics
    test_functions
)

