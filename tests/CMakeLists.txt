include(CTest)

find_package(fmt CONFIG REQUIRED)

add_library(TestUtils test_utils.cpp)
target_link_libraries(TestUtils PUBLIC API fmt::fmt)

create_test_sourcelist(Tests
  testdriver.cpp
  test_hello_world.cpp
  test_comments.cpp
  test_io.cpp
  test_stack_reversal.cpp
  test_swap.cpp
  test_variables.cpp)

add_executable(testdriver ${Tests})
target_link_libraries(testdriver PUBLIC TestUtils)

set(TestsToRun ${Tests})
remove(TestsToRun testdriver.cpp)

foreach(test ${TestsToRun})
  get_filename_component(TName ${test} NAME_WE)
  add_test(NAME ${TName} COMMAND testdriver ${TName})
endforeach()