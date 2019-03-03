cmake_minimum_required(VERSION 3.1.3)
project(SVMBlendLibrary)

include_directories(include)

find_package(ZLIB REQUIRED)
add_library(tools STATIC cieLab.cpp rgb.cpp modelReader.cpp cieLCh.cpp
                         modelContainer.cpp svm.cpp modelDecoder.cpp)
set_property(TARGET tools PROPERTY POSITION_INDEPENDENT_CODE ON)
add_library(svmBlend SHARED svmBlend.cpp)
target_link_libraries(svmBlend tools ZLIB::ZLIB)

add_executable(blendTest main.cpp)
target_link_libraries(blendTest svmBlend)
