add_library(haversine_math
  haversine_math.hpp
  haversine_math.cpp
)

add_executable(haversine_generator
  haversine_generator.cpp
)
target_link_libraries(haversine_generator
  PRIVATE
    haversine_math
)

add_library(profiler
  profiler.hpp
  profiler.cpp
)
target_link_libraries(profiler
  PRIVATE
    metrics
)

add_library(haversine_parser
  haversine_parser.hpp
  haversine_parser.cpp
)
target_link_libraries(haversine_parser
  PUBLIC
    haversine_math
    profiler
)

add_executable(json_parser
  json_parser.cpp
)
target_link_libraries(json_parser
  PRIVATE
    haversine_parser
)

add_library(metrics
  metrics.hpp
  metrics.cpp
)

add_library(repetition_tester
  repetition_tester.hpp
  repetition_tester.cpp
)
target_link_libraries(repetition_tester
  PRIVATE
    metrics
)

add_executable(repetition_tester_example_main
  repetition_tester_example_main.cpp
)
target_link_libraries(repetition_tester_example_main
  PRIVATE
    repetition_tester
    metrics
)

add_executable(profile_haversine_main
  profile_haversine_main.cpp
)
target_link_libraries(profile_haversine_main
  PRIVATE
    haversine_parser
    profiler
)

