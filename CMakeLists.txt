cmake_minimum_required(VERSION 3.10)

project(gRPC)
#生成调试版本
set(CMAKE_BUILD_TYPE "Debug")

# 设置可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 设置库文件输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 添加头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/example)
include_directories(${PROJECT_SOURCE_DIR}/muduo1/build/release-install-cpp11/include)
# 添加库文件搜索路径
link_directories(${PROJECT_SOURCE_DIR}/lib)
link_directories(${PROJECT_SOURCE_DIR}/muduo1/build/release-install-cpp11/lib)

# 添加子目录
# src包含了mprpc框架的核心代码
add_subdirectory(src)
# example包含示例代码
add_subdirectory(example)
