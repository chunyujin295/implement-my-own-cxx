project(my-own-wav-parser)

add_library(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/3rd)

target_link_libraries(${PROJECT_NAME} PUBLIC Logger)

file(GLOB_RECURSE headers CONFIGURE_DEPENDS "*.hpp")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "*.h")
file(GLOB_RECURSE srcs CONFIGURE_DEPENDS "*.cpp")

target_sources(${PROJECT_NAME} PUBLIC ${headers})
target_sources(${PROJECT_NAME} PRIVATE ${srcs})