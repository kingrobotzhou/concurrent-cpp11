add_compile_options(-std=c++11)

file(GLOB_RECURSE SOURCES
    ./*.cpp    
)
set(SRC_LIST main.cpp)
add_executable(demo14 ${SRC_LIST})