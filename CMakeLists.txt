cmake_minimum_required(VERSION 2.8.3)
project(eclipse_video_maker)

# Include depencencies
#list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
#find_package(Eigen3 REQUIRED)
find_package(OpenCV REQUIRED)


# Try to compile with c++11
# http://stackoverflow.com/a/25836953
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()



# Include needed directories
include_directories(
    src
    ${OpenCV_INCLUDE_DIRS}
)

# Make the binary of the main program
add_executable(eclipse_video src/main_eclipse.cpp src/helpers.cpp)
target_link_libraries(eclipse_video ${OpenCV_LIBRARIES})

# Make the binary file of the opencv example
add_executable(example_contours src/example_contours.cpp)
target_link_libraries(example_contours ${OpenCV_LIBRARIES})


