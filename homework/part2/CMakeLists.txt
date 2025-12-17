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
