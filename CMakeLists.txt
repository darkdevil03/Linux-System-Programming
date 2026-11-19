cmake_minimum_required(VERSION 3.20)
project(Linux_System_Programming C CXX)

# Generate compile_commands.json for IDE code completion and autocomplete support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the C standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# 1. FIND THE SYSTEM THREADING LIBRARY (Crucial for Linux pthreads)
find_package(Threads REQUIRED)

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
    get_filename_component(NAME_WE ${FILE} NAME_WE)

    # Sanitize the target name: replace any invalid CMake target characters with underscores
    # CMake target names must match [A-Za-z0-9_+-] and must not start with a digit, dot, or dash
    string(REGEX REPLACE "[^A-Za-z0-9_+-]" "_" TARGET_NAME "${NAME_WE}")

    # If the name starts with a digit, dot, or dash, prefix it with an underscore
    string(REGEX MATCH "^[0-9.+-]" STARTS_INVALID "${TARGET_NAME}")
    if(STARTS_INVALID)
        set(TARGET_NAME "_${TARGET_NAME}")
    endif()

    # Ensure target names are unique if duplicate file names exist across different subfolders
    if(TARGET ${TARGET_NAME})
        get_filename_component(PARENT_DIR ${FILE} DIRECTORY)
        get_filename_component(DIR_NAME ${PARENT_DIR} NAME)
        # Sanitize directory name too
        string(REGEX REPLACE "[^A-Za-z0-9_+-]" "_" DIR_NAME "${DIR_NAME}")
        set(TARGET_NAME "${DIR_NAME}_${TARGET_NAME}")
    endif()

    # If still duplicate (e.g., same dir), use the full relative path as target name
    if(TARGET ${TARGET_NAME})
        file(RELATIVE_PATH REL_PATH "${CMAKE_SOURCE_DIR}" ${FILE})
        string(REGEX REPLACE "[^A-Za-z0-9_+-]" "_" TARGET_NAME "${REL_PATH}")
        string(REGEX MATCH "^[0-9.+-]" STARTS_INVALID "${TARGET_NAME}")
        if(STARTS_INVALID)
            set(TARGET_NAME "_${TARGET_NAME}")
        endif()
    endif()

    # Create the individual executable target for the C file
    add_executable(${TARGET_NAME} ${FILE})

    # 2. LINK THE THREAD LIBRARY TO EVERY EXECUTABLE
    target_link_libraries(${TARGET_NAME} PRIVATE Threads::Threads)

    # 3. LINK ATOMICS (Sometimes required on specific Linux distros for stdatomic)
    target_link_libraries(${TARGET_NAME} PRIVATE atomic)

    # Direct output executable binary to the main build directory
    set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

    # Track this target
    list(APPEND ALL_TARGETS ${TARGET_NAME})
endforeach()