# CLion Helper to enable autocompletion
# Cannot actually build

cmake_minimum_required(VERSION 3.8)
project(OS)

set(CMAKE_C_STANDARD 99)

file(GLOB HW4_src "WS04.start/src/*.c")
file(GLOB HW4_head "WS04.start/include/*.h")
set (INCLUDE "")
foreach (_headerFile ${HW4_head})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND INCLUDE ${_dir})
endforeach()
list(REMOVE_DUPLICATES INCLUDE)

add_executable(Execute ${HW4_src})
target_include_directories(Execute PRIVATE ${INCLUDE})