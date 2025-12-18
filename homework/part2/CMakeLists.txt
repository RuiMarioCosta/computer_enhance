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

add_library(haversine_parser
  haversine_parser.hpp
  haversine_parser.cpp
)
target_link_libraries(haversine_parser
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

add_library(metrics
  metrics.hpp
  metrics.cpp
)

add_executable(profile_haversine_1
  profile_haversine_1.cpp
)
target_link_libraries(profile_haversine_1
  PRIVATE
    haversine_parser
    metrics
)

add_library(profiler
  profiler.hpp
  profiler.cpp
)
target_link_libraries(profiler
  PRIVATE
    metrics
)

add_executable(profile_haversine_2
  profile_haversine_2.cpp
)
target_link_libraries(profile_haversine_2
  PRIVATE
    haversine_parser
    profiler
)

