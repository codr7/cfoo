cmake_minimum_required(VERSION 3.5)

project(cfoo)
add_compile_options(-std=c11 -Wall -Werror)

file(GLOB_RECURSE sources source/cfoo/*.c)

add_executable(cfoo ${sources} source/main.c)
target_compile_options(cfoo PUBLIC -O2 -g -fsanitize=undefined)
target_include_directories(cfoo PUBLIC source/)
target_link_libraries(cfoo codr7 pthread ubsan)

add_library(lib_static STATIC ${sources})
set_target_properties(lib_static PROPERTIES OUTPUT_NAME cfoo)
target_include_directories(lib_static PUBLIC source/)
target_link_libraries(lib_static codr7)

add_executable(tests ${sources} source/tests.c)
target_compile_options(tests PUBLIC -O2 -g -fsanitize=undefined)
target_include_directories(tests PUBLIC source/)
target_link_libraries(tests codr7 pthread ubsan)

file(GLOB headers source/cfoo/*.h)
install(FILES ${headers} DESTINATION include/cfoo)

install(FILES build/libcfoo.a DESTINATION lib)
