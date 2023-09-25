cmake_minimum_required(VERSION 3.15)

project(OrderBook)

set(CMAKE_CXX_STANDARD 17)

add_executable(OrderBook
        src/main.cpp
        src/OrderBook.cpp
        src/OrderBook.h
        src/Order.cpp
        src/Order.h
        src/Limit.cpp
        src/Limit.h)

include_directories(${CMAKE_SOURCE_DIR}/src)