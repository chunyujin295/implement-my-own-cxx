project(my-own-math)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE srcs CONFIGURE_DEPENDS "*.hpp")

target_sources(${PROJECT_NAME} PRIVATE ${srcs})