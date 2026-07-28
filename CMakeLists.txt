cmake_minimum_required(VERSION 3.20)
project(Linux_System_Programming C)

# Set the C standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Keeps your build directory organized by putting output executables in the root build folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# 1. Find all .c source files recursively and auto-configure when files are added/deleted/renamed
file(GLOB_RECURSE C_FILES CONFIGURE_DEPENDS "*.c")

# 2. Filter out any C files created automatically inside CMake build folders
list(FILTER C_FILES EXCLUDE REGEX "cmake-build-")

# Keep track of all generated targets in this configuration run
set(ALL_TARGETS "")

# 3. Loop through every C source file and generate a standalone executable target
foreach(FILE ${C_FILES})
    # Extract the base file name without extension
    get_filename_component(TARGET_NAME ${FILE} NAME_WE)

    # Ensure target names are unique if duplicate file names exist across different subfolders
    if(TARGET ${TARGET_NAME})
        get_filename_component(PARENT_DIR ${FILE} DIRECTORY)
        get_filename_component(DIR_NAME ${PARENT_DIR} NAME)
        set(TARGET_NAME "${DIR_NAME}_${TARGET_NAME}")
    endif()

    # Create the individual executable target for the C file
    add_executable(${TARGET_NAME} ${FILE})

    # Direct output executable binary to the main build directory
    set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

    # Track this target
    list(APPEND ALL_TARGETS ${TARGET_NAME})
endforeach()