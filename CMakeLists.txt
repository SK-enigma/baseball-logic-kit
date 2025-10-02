cmake_minimum_required(VERSION 3.20)
project(baseball_logic_kit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(baseball
    src/GameState.cpp
    src/Rules.cpp
    src/RNG.cpp
)
target_include_directories(baseball PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(baseball_demo src/main.cpp)
target_link_libraries(baseball_demo PRIVATE baseball)

include(cmake/FetchDoctest.cmake)
add_executable(tests tests/test_rules.cpp)
target_link_libraries(tests PRIVATE baseball doctest_dep)

enable_testing()
add_test(NAME rules COMMAND tests)