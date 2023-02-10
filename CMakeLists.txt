cmake_minimum_required(VERSION 3.15)

project (  concurrent-cpp20-all )

set ( PRJ_COMPILE_FEATURES )

list ( APPEND PRJ_COMPILE_FEATURES cxx_std_20 )

add_subdirectory(demo1)
add_subdirectory(demo2)