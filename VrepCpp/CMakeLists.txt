cmake_minimum_required(VERSION 2.8.3)
project(Vrep_cpp)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_BUILD_TYPE Debug)

#find_package(catkin REQUIRED COMPONENTS roscpp)
#catkin_package()

#include_directories(${catkin_INCLUDE_DIRS})
include_directories("/home/enrico-giordano/VREP/programming/include")
include_directories("/home/enrico-giordano/VREP/programming/remoteApi")
set(SOURCE_FILES src/main.cpp)

add_executable(vrep ${INCLUDE_FILES} ${SOURCE_FILES})
target_link_libraries(vrep "/home/enrico-giordano/VREP/programming/remoteApiBindings/lib/lib/64Bit/libremoteApi.so")
