cmake_minimum_required(VERSION 3.10)
project(KoboldKhat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Shared library
add_subdirectory(common)

add_subdirectory(server)

add_subdirectory(client)


