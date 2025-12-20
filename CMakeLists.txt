
cmake_minimum_required(VERSION 3.28)

project(
  annealer_sim
  VERSION 0.1
  LANGUAGES CXX
  )

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic)



add_executable(${PROJECT_NAME} main.cpp)

target_sources(${PROJECT_NAME} PRIVATE 
  main.cpp)

option(DEBUG "Enable debug mode" ON)
if(DEBUG)
  add_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=1)
else()
  add_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG=0)
endif()

add_subdirectory(cnf_parser)
add_subdirectory(cnf_structs)
add_subdirectory(annealer)
add_subdirectory(rng_wrapper)
target_link_libraries(${PROJECT_NAME} PUBLIC
  cnf_structs
  cnf_parser
  annealer
  prng)