cmake_minimum_required(VERSION 3.5)
PROJECT(finder)
add_compile_options(-std=c++11)
INCLUDE_DIRECTORIES(
include
)
find_package(OpenCV REQUIRED)
find_package(Threads)
aux_source_directory(src DIR_SRCS)
SET(TEST_MATH
${DIR_SRCS}
)
add_executable(finder ${TEST_MATH})
target_link_libraries(finder ${OpenCV_LIBS})
target_link_libraries(finder ${CMAKE_THREAD_LIBS_INIT})
