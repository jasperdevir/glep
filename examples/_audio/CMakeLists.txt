cmake_minimum_required(VERSION 3.10)

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

file(GLOB_RECURSE PROJECTS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp)

foreach(project_file ${PROJECTS})
    get_filename_component(project_dir ${project_file} DIRECTORY)
    get_filename_component(project_name ${project_dir} NAME)

    set(output_name GLEPAudio_${project_name})

    add_executable(${output_name} ${project_file})

    if(APPLE)
        # For Apple-specific settings
        file(GLOB_RECURSE LIB "${CMAKE_SOURCE_DIR}/lib/*.a" "${CMAKE_SOURCE_DIR}/lib/*.dylib")
        target_link_libraries(${output_name}
            GLEP
            ${LIB}
            "-framework IOKit"
            "-framework Cocoa"
            "-framework Foundation"
            "-framework OpenGL"
        )
    else()
        # For non-Apple-specific settings
        file(GLOB_RECURSE LIB "${CMAKE_SOURCE_DIR}/lib/*.lib")
        target_link_libraries(${output_name}
            GLEP 
            ${LIB}
        )
    endif()

    # Set output directory for the executables
    set_target_properties(${output_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    )
endforeach()