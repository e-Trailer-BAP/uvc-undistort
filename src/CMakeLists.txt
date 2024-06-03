cmake_minimum_required(VERSION 3.10.0)

# Set the project name
project(uvc-undistort)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR})

# Source files
set(SOURCE_FILES 
    main.cpp
    FisheyeCameraModel.cpp
)

# Header files
set(HEADER_FILES
    FisheyeCameraModel.hpp
)

# Specify the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Find packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(opencv REQUIRED IMPORTED_TARGET opencv4)
pkg_search_module(gstreamer REQUIRED IMPORTED_TARGET gstreamer-1.0)
pkg_search_module(glib REQUIRED IMPORTED_TARGET glib-2.0)
pkg_search_module(yaml-cpp REQUIRED IMPORTED_TARGET yaml-cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    PkgConfig::opencv
    PkgConfig::gstreamer
    PkgConfig::glib
    PkgConfig::yaml-cpp
)