cmake_minimum_required(VERSION 3.5)
project(KFBSlide_Cpp)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
link_directories(lib)

set(SRC_FILES src/kfbslide.cpp)

add_library(kfbslide SHARED ${SRC_FILES} include/kfbslide.h)

add_executable(KFBSlide_Cpp main.cpp ${SRC_FILES})
target_link_libraries(KFBSlide_Cpp ImageOperationLib dl tiff)