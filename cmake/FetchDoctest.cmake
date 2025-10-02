include(FetchContent)

FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/doctest/doctest.git
  GIT_TAG v2.4.11
)
FetchContent_MakeAvailable(doctest)

# Create a safe interface target that maps to whichever doctest target exists.
if (TARGET doctest::doctest)
  add_library(doctest_dep INTERFACE)
  target_link_libraries(doctest_dep INTERFACE doctest::doctest)
elseif (TARGET doctest)
  add_library(doctest_dep INTERFACE)
  target_link_libraries(doctest_dep INTERFACE doctest)
else()
  add_library(doctest_dep INTERFACE)
  target_include_directories(doctest_dep INTERFACE ${doctest_SOURCE_DIR})
endif()