find_package(fmt CONFIG REQUIRED)

add_subdirectory(parse)
add_subdirectory(vm)

add_library(Compile compile.cpp)

add_library(API api.cpp)
target_link_libraries(API PRIVATE VM Compile TokenIO)

add_executable(nori main.cpp)
target_link_libraries(nori PRIVATE API fmt::fmt)