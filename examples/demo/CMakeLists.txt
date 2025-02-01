cmake_minimum_required(VERSION 3.10)
project(GLEP_Demo)

set(CMAKE_CXX_STANDARD 17)

if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/include/external
)

link_directories(
    ${CMAKE_SOURCE_DIR}/lib
)

add_executable(GLEP_Demo
    GLEP_demo.cpp
)

if(APPLE)
    file(GLOB_RECURSE LIB "${CMAKE_SOURCE_DIR}/lib/*.a" "${CMAKE_SOURCE_DIR}/lib/*.dylib")
else()
    file(GLOB_RECURSE LIB "${CMAKE_SOURCE_DIR}/lib/*.lib")
endif()

if(APPLE)
    target_link_libraries(GLEP_Demo 
        ${LIB}
        "-framework IOKit"
        "-framework Cocoa"
        "-framework Foundation"
        "-framework OpenGL"
    )
else()
    target_link_libraries(GLEP_Demo 
        ${LIB}
    )
endif()

set_target_properties(GLEP_Demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)