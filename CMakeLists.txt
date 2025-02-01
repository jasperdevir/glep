cmake_minimum_required(VERSION 3.11)
project(GLEP)

set(CMAKE_CXX_STANDARD 17)

if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)

include(FetchContent)

FetchContent_Declare(
    al
    GIT_REPOSITORY https://github.com/kcat/openal-soft.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)

set(ALSOFT_EXAMPLES OFF)
set(ALSOFT_UTILS OFF)
set(ALSOFT_TESTS OFF)

FetchContent_Declare(
    sndfile
    GIT_REPOSITORY https://github.com/libsndfile/libsndfile.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)

set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_BUILD_SAMPLES OFF)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_NO_EXPORT ON)

if(MSVC)
    set(ASSIMP_INSTALL_PDB OFF)
endif()

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)

set(BUILD_PROGRAMS OFF)
set(BUILD_EXAMPLES OFF)
set(BUILD_TESTING OFF)
set(CMAKE_BUILD_TYPE Release)

FetchContent_MakeAvailable(al)
FetchContent_MakeAvailable(sndfile)
FetchContent_MakeAvailable(assimp)
FetchContent_MakeAvailable(glfw)

set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)

file(GLOB_RECURSE GLEP_SRC  
    ${CMAKE_SOURCE_DIR}/src/*
)

include_directories(
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/include/external
)

link_directories(
    ${CMAKE_SOURCE_DIR}/lib
)

set(CMAKE_BUILD_TYPE Release)

add_library(GLEP STATIC ${GLEP_SRC})

set_target_properties(GLEP PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
)

if(APPLE)
    target_link_libraries(GLEP 
        ${LIB}
        "-framework IOKit"
        "-framework Cocoa"
        "-framework Foundation"
        "-framework OpenGL"
    )
else()
    target_link_libraries(GLEP 
        ${LIB}
    )
endif()

set(GLEP_BUILD_EXAMPLES ON)

if(GLEP_BUILD_EXAMPLES)
   add_subdirectory(${CMAKE_SOURCE_DIR}/examples/_core/)
   add_subdirectory(${CMAKE_SOURCE_DIR}/examples/_audio/)
   add_subdirectory(${CMAKE_SOURCE_DIR}/examples/_control/)
   add_subdirectory(${CMAKE_SOURCE_DIR}/examples/demo/)
endif()
