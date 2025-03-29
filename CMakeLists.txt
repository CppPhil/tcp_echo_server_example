cmake_minimum_required(VERSION 3.10)

project(tcp_echo_server_project)

add_executable(
  tcp_echo_server
  library_initializer.cpp
  library_initializer.hpp
  main.cpp
  run_echo_server.cpp
  run_echo_server.hpp
  socket.cpp
  socket.hpp)

if(WIN32)
  target_link_libraries(tcp_echo_server PRIVATE ws2_32)
endif()
