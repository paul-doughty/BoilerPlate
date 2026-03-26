cmake_minimum_required(VERSION 3.16)
project(hello_sdl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not supported. Run cmake -S . -B build instead.")
endif ()

add_executable(hello_sdl src/main.cpp)

find_package(SDL2 CONFIG QUIET)

if (TARGET SDL2::SDL2 OR TARGET SDL2::SDL2-static)
    target_link_libraries(hello_sdl PRIVATE $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)
else ()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
    target_link_libraries(hello_sdl PRIVATE PkgConfig::SDL2)
endif ()
