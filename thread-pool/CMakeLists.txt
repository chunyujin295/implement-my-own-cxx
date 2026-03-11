project(my-own-thread-pool)

add_library(${PROJECT_NAME} "")

file(GLOB_RECURSE "src" CONFIGURE_DEPENDS "*.cpp" "*.h" "*.hpp")

target_link_libraries(${PROJECT_NAME} PRIVATE Logger)
target_sources(${PROJECT_NAME} PRIVATE ${src})