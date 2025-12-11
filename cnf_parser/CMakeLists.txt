add_library(cnf_parser STATIC cnf_parser.cpp)

target_include_directories(cnf_parser
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(cnf_parser PUBLIC cnf_structs)