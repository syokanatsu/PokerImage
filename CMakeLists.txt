# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)
set(THIRD_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/include)
set(THIRD_LIB ${CMAKE_CURRENT_SOURCE_DIR}/third_party/lib)
include_directories(${THIRD_INCLUDE})
link_directories(${THIRD_LIB})
project ("PokerImage")

# 包含子项目。
add_subdirectory ("PokerImage")
add_subdirectory ("third_party/include/json_helper")