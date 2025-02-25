cmake_minimum_required(VERSION 3.15.0)
project(msplot)

set(CMAKE_CXX_STANDARD 20)

add_executable(msplot_demo msplot_demo.cpp)

# Specify the installation directory
install(TARGETS msplot_demo DESTINATION bin)
