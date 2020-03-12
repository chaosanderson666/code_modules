cmake_minimum_required(VERSION 3.2)

project("sample")
set(libs json-c)
set(targets sample)
add_definitions("-Wall -g")
add_executable(sample jsonc_example.c)
target_link_libraries(sample ${libs})
