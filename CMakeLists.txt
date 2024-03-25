cmake_minimum_required(VERSION 3.12)
project(modern-cpp-euler)

file(GLOB CONFIGURE_DEPENDS _files src/*.cpp)

foreach (_file IN ${_files})
    add_executable(_file $file)
endforeach ()