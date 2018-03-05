cmake_minimum_required(VERSION 2.8.8)

set(PROJECT_NAME_STR SFMT-Database)
project(${PROJECT_NAME_STR} C CXX)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(FATAL_ERROR "should use gcc (not clang)")
endif()

set(DEFINE "-DHAVE_SSE2 -DSFMT_MEXP=19937")

if(CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -ansi -Wno-deprecated -std=c++11 -fopenmp -O3 -msse2 ${DEFINE}")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -O3 -msse2 ${DEFINE}")
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp /arch:SSE2 ${DEFINE}")
endif()

set(SFMT_DIR ${PROJECT_SOURCE_DIR}/SFMT)
include_directories(${SFMT_DIR})
add_library(sfmt STATIC
    ${SFMT_DIR}/SFMT.c
)
add_library(utils STATIC
    ${PROJECT_SOURCE_DIR}/utils.cpp
)
add_library(rng_utils STATIC
    ${PROJECT_SOURCE_DIR}/rng_utils.cpp
)

add_executable(preprocess
    ${PROJECT_SOURCE_DIR}/preprocess.cpp
)
add_executable(create_db
    ${PROJECT_SOURCE_DIR}/create_db.cpp
)
add_executable(split_db
    ${PROJECT_SOURCE_DIR}/split_db.cpp
)
add_executable(search
    ${PROJECT_SOURCE_DIR}/search.cpp
)

target_link_libraries(preprocess utils rng_utils sfmt)
target_link_libraries(create_db utils rng_utils sfmt -lstdc++fs)
target_link_libraries(split_db utils rng_utils sfmt -lstdc++fs)
target_link_libraries(search utils rng_utils sfmt)
