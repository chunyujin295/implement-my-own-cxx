cmake_minimum_required(VERSION 3.15)
project(implement-my-own-cxx)
set(CMAKE_CXX_STANDARD 17)

add_compile_options("/utf-8")     # 给编译器加编译选项 /utf-8，让源文件按 UTF-8 编码解析
add_compile_definitions("-DUNICODE -D_UNICODE")

include(${CMAKE_SOURCE_DIR}/cmake/3rd.cmake)

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# MinGW下静态链接通用依赖库
if (MINGW)
    add_link_options(
            -static-libgcc
            -static-libstdc++
            -Wl,-Bstatic
            -lwinpthread
            -Wl,-Bdynamic
    )
endif ()

add_subdirectory(thread-pool)
add_subdirectory(memory-pool)
add_subdirectory(sample)