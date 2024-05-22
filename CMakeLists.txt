cmake_minimum_required(VERSION 3.28)
project(OS)

set(CMAKE_CXX_STANDARD 17)

add_executable(client1 lab-7/1/client.cpp)
add_executable(client2 lab-7/2/client.cpp)

add_executable(server1 lab-7/1/server.cpp)
add_executable(server2 lab-7/2/server.cpp)