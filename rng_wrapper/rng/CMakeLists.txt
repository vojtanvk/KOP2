add_library(rng_c STATIC xoshiro256plus.cpp rngctrl.cpp)

target_include_directories(rng_c PUBLIC include/)