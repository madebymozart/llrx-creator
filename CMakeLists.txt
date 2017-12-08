cmake_minimum_required(VERSION 3.9)
project(LlrxCreator)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(LlrxCreator src/main.cxx)