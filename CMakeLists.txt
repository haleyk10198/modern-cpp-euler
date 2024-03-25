cmake_minimum_required(VERSION 3.12)
project(modern-cpp-euler)

file(GLOB CONFIGURE_DEPENDS _files *.cpp)

foreach (_file IN ${_files})
    add_executable(_file $file
            problem_18.cpp)
endforeach ()