cmake_minimum_required(VERSION 3.10)

project(Hamming)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HAMMING_SOURCES hamming.cpp)

set(HAMMING_HEADERS hamming.h)

add_library(HammingStatic STATIC ${HAMMING_SOURCES} ${HAMMING_HEADERS})

add_library(HammingShared SHARED ${HAMMING_SOURCES} ${HAMMING_HEADERS})

add_executable(main main.cpp)

# target_link_libraries(main PRIVATE HammingStatic)
target_link_libraries(main PRIVATE HammingShared)