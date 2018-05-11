cmake_minimum_required(VERSION 3.5)
project(KFB_Convert)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
link_directories(lib)

set(SRC_FILES src/write_RGB_to_TIFF.cpp)

add_executable(KFB_Convert main.cpp ${SRC_FILES})
target_link_libraries(KFB_Convert ImageOperationLib dl tiff)