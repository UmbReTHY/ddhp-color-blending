cmake_minimum_required(VERSION 2.8)
project(SVMBlendLibrary)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}" /Ox")

include_directories(include ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(zlib)
add_library(tools STATIC cieLab.cpp rgb.cpp modelReader.cpp cieLCh.cpp
                         modelContainer.cpp svm.cpp modelDecoder.cpp)
add_library(svmBlend SHARED svmBlend.cpp)
target_link_libraries(svmBlend tools zlib)

add_executable(blendTest main.cpp)
target_link_libraries(blendTest svmBlend)

add_executable(zlibTest zlibTest.cpp)
target_link_libraries(zlibTest zlib)
