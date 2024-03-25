cmake_minimum_required(VERSION 3.12)
project(modern-cpp-euler)

set(CMAKE_CXX_STANDARD 23)

file(GLOB _files CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )

foreach (_file ${_files})
    MESSAGE(STATUS "adding executable for ${_file}")
    get_filename_component(file_name ${_file} NAME)
    add_executable(${file_name} ${_file})
endforeach ()