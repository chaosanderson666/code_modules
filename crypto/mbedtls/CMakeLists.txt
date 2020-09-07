cmake_minimum_required(VERSION 3.2)

project("sample")

set(libs mbedtls mbedcrypto mbedx509)

set(targets sample)

add_definitions("-Wall -g")
add_executable(sample encrypt.c main.c)
target_link_libraries(sample ${libs})
