cmake_minimum_required(VERSION 2.8.12)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(SOURCES
    "src/pause_runtime.cpp"
    "src/interface.cpp"
)

set(HEADERS
    "include/pause_runtime.h"
)

#set compiler flags
if((${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang") OR (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU"))
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wreturn-type -Wpedantic ")
endif()

include_directories(include)
add_library(pause_runtime MODULE ${SOURCES} ${HEADERS})
target_link_libraries(pause_runtime PRIVATE ${CONAN_LIBS})
