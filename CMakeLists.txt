cmake_minimum_required(VERSION 3.10)
project(Mymuduo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置库文件的输出路径
set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

# 指定 include 目录，供编译器查找头文件
include_directories(${PROJECT_SOURCE_DIR}/include)

# 查找 src 目录下的所有 .cpp 源文件
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)

# 生成共享库 Mymuduo
add_library(Mymuduo SHARED ${SRC_LIST})

# 指定编译输出的动态库路径（适用于某些 CMake 版本）
set_target_properties(Mymuduo PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_DIRECTORY}
)
