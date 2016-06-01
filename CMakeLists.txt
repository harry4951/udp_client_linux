cmake_minimum_required (VERSION 3.0)
project("udp_client")



add_executable(udp_client udp_client.cpp)
target_link_libraries(udp_client m jack)