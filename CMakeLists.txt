# CLion Helper to enable autocompletion
# Cannot actually build

cmake_minimum_required(VERSION 3.8)
project(OS)

set(CMAKE_C_STANDARD 99)

set (INCLUDE "")
file(GLOB src "WS04.start/src/*.c" "Lab04/src/*.c" "WS05.start/src/*.c")
file(GLOB head "WS04.start/include/*.h" "Lab04/include/*.h" "WS05.start/include/*.h")
foreach (_headerFile ${head})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND INCLUDE ${_dir})
endforeach()

add_executable(Execute ${src})
target_include_directories(Execute PRIVATE ${INCLUDE})