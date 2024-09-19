cmake_minimum_required(VERSION 3.30.2)

project(CMAKE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify the path to the OpenCVConfig.cmake
set(OpenCV_DIR "C:/Users/The user/Desktop/פראקטיקום אמבדד/opencv/build")
# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# Add source files and create executable
add_executable(CMAKE main.cpp HashTable.cpp matchFeatures.cpp matchFeatures.h HashTable.h)

# Link OpenCV libraries
target_link_libraries(CMAKE ${OpenCV_LIBS})

# Set installation path for the executable
install(TARGETS CMAKE DESTINATION bin)



# cmake_minimum_required(VERSION 3.14)

# project(CMAKE)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Set the path to OpenCVConfig.cmake
# set(OpenCV_DIR "C:/Users/The user/Desktop/פראקטיקום אמבדד/opencv/build")

# # Find OpenCV package
# find_package(OpenCV REQUIRED)

# # Include OpenCV headers
# include_directories(${OpenCV_INCLUDE_DIRS})

# # Add source files and create executable
# add_executable(CMAKE main.cpp HashTable.cpp matchFeatures.cpp matchFeatures.h HashTable.h)

# # Link against the combined OpenCV library
# target_link_libraries(CMAKE "${OpenCV_LIB_DIR}/opencv_world4100.lib")

# # Install executable
# install(TARGETS CMAKE DESTINATION bin)
