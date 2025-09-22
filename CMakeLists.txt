cmake_minimum_required(VERSION 3.16)
project(simulator)

set(CMAKE_CXX_STANDARD 20)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(simulator src/main.cpp)
target_link_libraries(simulator sfml-graphics sfml-window sfml-system sfml-audio)
