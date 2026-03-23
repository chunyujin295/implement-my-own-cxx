project(my-own-sample)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE my-own-thread-pool)
target_link_libraries(${PROJECT_NAME} PRIVATE my-own-memory-pool)
target_link_libraries(${PROJECT_NAME} PRIVATE my-own-endian-test)

target_link_libraries(${PROJECT_NAME} PRIVATE Logger)