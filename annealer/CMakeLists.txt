add_library(annealer STATIC annealer.cpp)

target_include_directories(annealer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(annealer PUBLIC prng)