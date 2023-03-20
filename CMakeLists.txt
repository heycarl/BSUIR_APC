cmake_minimum_required(VERSION 3.24)
project(LABS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

set(Directories_1
        grvt_logger)
set(Sources_1
        LAB_01/main.cpp
        grvt_logger/logger.hpp
        grvt_logger/logger.cpp)

set(Sources_3
        LAB_03/main.cpp)

set(Sources_4
        LAB_04/main.cpp)

set(Sources_5
        LAB_05/main.c)

add_executable(LAB_01
        ${Sources_1})

target_include_directories(LAB_01 PRIVATE
        ${Directories_1})

add_executable(LAB_02
        LAB_02/main.cpp)

target_include_directories(LAB_02 PRIVATE
        "C:\\Users\\Alexandr\\Documents\\CODE\\BSUIR\\_GRADE2\\_SEMESTER4\\APC\\BC\\INCLUDE")

add_executable(LAB_03
        ${Sources_3})

add_executable(LAB_04
        ${Sources_4})

add_executable(LAB_05
        ${Sources_5})

# define build type
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DRELEASE)
endif()