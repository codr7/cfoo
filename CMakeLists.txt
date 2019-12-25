cmake_minimum_required(VERSION 3.5)

project(cfoo)
add_compile_options(-std=c99 -Wall -Werror -O2 -g)

file(GLOB_RECURSE sources source/cfoo/*.c)

add_library(lib_static STATIC ${sources})
set_target_properties(lib_static PROPERTIES OUTPUT_NAME cfoo)
target_include_directories(lib_static PUBLIC source/)
target_link_libraries(lib_static ceque)

add_executable(tests ${sources} source/tests.c)
target_include_directories(tests PUBLIC source/)
target_link_libraries(tests ceque)

file(GLOB headers source/cfoo/*.h)
install(FILES ${headers} DESTINATION include/cfoo)

install(FILES build/libcfoo.a DESTINATION lib)
