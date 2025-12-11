add_library(prng STATIC rng_wrapper.cpp)

target_include_directories(prng PUBLIC include/)

add_subdirectory(rng)
target_link_libraries(prng PRIVATE rng_c)