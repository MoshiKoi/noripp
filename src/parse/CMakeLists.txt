find_package(fmt CONFIG REQUIRED)

add_library(TokenIO tokenio.cpp)

target_link_libraries(TokenIO PUBLIC fmt::fmt)