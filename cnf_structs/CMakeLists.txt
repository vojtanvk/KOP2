add_library(cnf_structs STATIC clause.cpp formula.cpp literal.cpp)

target_include_directories(cnf_structs
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)