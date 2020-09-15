cmake_minimum_required(VERSION 3.10)
set(this clustering)
project(${this})
set(CMAKE_CXX_STANDARD 17)

SET( EIGEN3_INCLUDE_DIR "/usr/include/eigen3" )
include_directories( "${EIGEN3_INCLUDE_DIR}" )


#include_directories(  ${catkin_INCLUDE_DIRS}  ${PCL_INCLUDE_DIRS} )


find_package(PCL 1.8 REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

#set(ENV{FLANN_ROOT} "/usr/include/flann")
#find_package(FLANN REQUIRED)
#add_definitions(${FLANN_CFLAGS})




include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

FILE(GLOB HEADER_FILES include/*.hpp)
FILE(GLOB SOURCE_FILES src/*.cpp)

add_library(mylib ${HEADER_FILES} ${SOURCE_FILES})
add_executable(${this} main.cpp ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(${this} ${PCL_LIBRARIES})