cmake_minimum_required (VERSION 3.0)
project("udp_client")



add_executable(udp_client udp_client.cpp)
target_compile_features(udp_client PRIVATE cxx_range_for)
target_link_libraries(udp_client m jack)
