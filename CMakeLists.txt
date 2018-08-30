cmake_minimum_required(VERSION 3.5)
project(KFB_Convert_TIFF)

set(CMAKE_CXX_STANDARD 11)

set(LINKER_LIBS "")

# add GFLAGS 
find_package(GFlags REQUIRED)
include_directories(${GFLAGS_INCLUDE_DIRS})
list(APPEND LINKER_LIBS ${GFLAGS_LIBRARIES})

include_directories(include)
link_directories(lib)

set(SRC_FILES src/read_RGB_from_KFB.cpp src/utils.cpp src/write_RGB_to_TIFF.cpp)

add_executable(read_rgb_from_kfb main.cpp ${SRC_FILES})
target_link_libraries(read_rgb_from_kfb ImageOperationLib dl tiff ${LINKER_LIBS})
