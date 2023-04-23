find_package(fmt CONFIG REQUIRED)

add_library(VM vm.cpp)

target_link_libraries(VM PUBLIC fmt::fmt)