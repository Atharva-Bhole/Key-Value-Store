cmake_minimum_required(VERSION 3.31)
project(KeyValueStore)

set(CMAKE_CXX_STANDARD 20)

add_executable(KeyValueStore KeyValueStore.cpp
        include/kv_store.h
        include/persistence.h
        persistence.cpp
        include/user_manager.h
        user_manager.cpp
        main.cpp
        main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/crow/include)
include_directories(${CMAKE_SOURCE_DIR}/asio/asio/include) # Add this
target_link_libraries(KeyValueStore PRIVATE Ws2_32 mswsock)
