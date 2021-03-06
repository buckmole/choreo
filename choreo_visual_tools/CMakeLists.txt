cmake_minimum_required(VERSION 2.8.3)
project(choreo_visual_tools)

# C++ 11 required for OMPL
add_compile_options(-std=c++11)

# Load catkin and all dependencies required for this package
find_package(catkin REQUIRED COMPONENTS
  moveit_visual_tools
  eigen_conversions
  geometry_msgs
  moveit_core
  roscpp
  tf_conversions
  visualization_msgs
  choreo_msgs
)

find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)

# Catkin
catkin_package(
  LIBRARIES
    ${PROJECT_NAME}
  CATKIN_DEPENDS
    moveit_visual_tools
    moveit_core
    geometry_msgs
    visualization_msgs
    choreo_msgs
  INCLUDE_DIRS include
)

## Build
#include_directories(SYSTEM
#  ${EIGEN3_INCLUDE_DIRS}
#)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
)

# Visualization Tools Library
add_library(${PROJECT_NAME}
  src/${PROJECT_NAME}.cpp
)

#add_dependencies(${PROJECT_NAME} graph_msgs_generate_messages_cpp)

target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES} ${Boost_LIBRARIES}
)

#############
## Install ##
#############

# Install libraries
install(TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})

# Install header files
install(DIRECTORY include/${PROJECT_NAME}/   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})
