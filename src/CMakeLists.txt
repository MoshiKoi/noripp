add_subdirectory(parse)
add_subdirectory(vm)

add_library(Compile compile.cpp)

add_executable(nori main.cpp)
target_link_libraries(nori PRIVATE VM Compile TokenIO)