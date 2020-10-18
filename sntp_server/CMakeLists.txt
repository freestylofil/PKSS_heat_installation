cmake_minimum_required(VERSION 3.16)
project(sntp_server)

set(CMAKE_CXX_STANDARD 17)

add_executable(sntp_server main.cpp)
include_directories(include)

find_package(Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

find_package(Threads REQUIRED)
if(Threads_FOUND)
    target_link_libraries(sntp_server PRIVATE Threads::Threads)
endif()

include_directories(lib/structopt/include/)