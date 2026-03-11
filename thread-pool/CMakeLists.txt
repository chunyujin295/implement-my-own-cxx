cmake_minimum_required(VERSION 3.15)
project(my-own-thread-pool)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/3rd.cmake)

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

add_executable(${PROJECT_NAME} "")

file(GLOB_RECURSE "src" CONFIGURE_DEPENDS "*.cpp" "*.h" "*.hpp")

target_link_libraries(${PROJECT_NAME} PRIVATE Logger)
target_sources(${PROJECT_NAME} PRIVATE ${src})