cmake_minimum_required(VERSION 3.10)
project(Unity_ZBar)
if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory and run CMake from there. You may need to remove CMakeCache.txt." )
endif()

# set the project name and version
set(PROJECT_NAME Unity_ZBar)
set(VERSION_NUM 0.1)
project(${PROJECT_NAME} VERSION ${VERSION_NUM})

message(STATUS "${PROJECT_NAME} v${VERSION_NUM} building on ${CMAKE_SYSTEM}")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#include cmake dir for includes
add_definitions(-DENABLE_BITCODE)


#Support #include<SocketPool> by adding SocketPool dir to include directories
include_directories(libzbar/include)

#Build the SocketPool Static Library to link
add_subdirectory(libzbar)

set(SOURCES
    wrapper.cpp
)
set(HEADERS
    wrapper.i
    wrapper.h
)


FIND_PACKAGE(SWIG REQUIRED)

message(STATUS ${SWIG_DIR})
message(STATUS ${SWIG_EXECUTABLE})

include(UseSWIG)


swig_add_library(Unity_ZBar TYPE SHARED LANGUAGE csharp OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/products/bindings SOURCES ${SOURCES} ${HEADERS})
if(APPLE)
    target_link_libraries(Unity_ZBar zbar "-fembed-bitcode")
else()
    target_link_libraries(Unity_ZBar zbar)
endif()
target_compile_features(Unity_ZBar PUBLIC cxx_std_17)
