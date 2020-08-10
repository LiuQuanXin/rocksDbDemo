cmake_minimum_required(VERSION 3.13)
project(rocksDbDemo)

set(CMAKE_CXX_STANDARD 11)

# 静态库位置
include_directories(
        /usr/local/include
)

# 动态库位置
link_directories(
    /usr/local/lib
)

add_executable(rocksDbDemo main.cpp)

set(LIBS rocksdb)
target_link_libraries(rocksDbDemo ${LIBS})