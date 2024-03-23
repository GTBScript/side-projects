cmake_minimum_required(VERSION 3.27)
project(MazeSolver)

set(CMAKE_CXX_STANDARD 23)

add_executable(MazeSolver main.cpp)

if(WIN32)
    target_compile_definitions(MazeSolver PRIVATE WIN_PLATFORM)
    if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_link_options(MazeSolver PRIVATE -static-libgcc -static-libstdc++ -static)
    endif()
endif()

# Conditional compilation for Linux
if(UNIX AND NOT APPLE)
    target_compile_definitions(MazeSolver PRIVATE LINUX_PLATFORM)
endif()

# Conditional compilation for macOS
if(APPLE)
    target_compile_definitions(MazeSolver PRIVATE MACOS_PLATFORM)
endif()

