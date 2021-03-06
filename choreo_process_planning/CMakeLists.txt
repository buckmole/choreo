cmake_minimum_required(VERSION 2.8.12)
project(choreo_process_planning)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
	descartes_core
	descartes_moveit
	descartes_trajectory
	descartes_parser
	descartes_planner
	choreo_descartes_planner
	choreo_msgs
	moveit_msgs
	geometric_shapes
	moveit_core
	moveit_ros_planning_interface
	roscpp
	rviz_visual_tools
	choreo_planning_capability
	choreo_param_helpers
	choreo_geometry_conversion_helpers
	eigen_conversions
	)

find_package(Boost REQUIRED COMPONENTS filesystem)

catkin_package(
	INCLUDE_DIRS include
	#
	CATKIN_DEPENDS
	descartes_core
	descartes_moveit
	descartes_planner
	choreo_descartes_planner
	descartes_trajectory
	descartes_parser
	choreo_msgs
	moveit_msgs
	geometric_shapes
	moveit_core
	moveit_ros_planning_interface
	roscpp
	rviz_visual_tools
	choreo_planning_capability
	choreo_param_helpers
	choreo_geometry_conversion_helpers
	eigen_conversions
)

###########
## Build ##
###########

include_directories(
	include
	${catkin_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
)

## Declare a cpp executable
add_executable(choreo_process_planning_node
	src/choreo_process_planning.cpp
	src/choreo_process_planning_node.cpp
	#
	src/print_process_planning.cpp
	src/move_to_target_pose_planning.cpp
	src/picknplace_planning.cpp
	#
	src/trajectory_utils.cpp
	src/path_transitions.cpp
	src/common_utils.cpp
	#
	src/generate_motion_plan.cpp
	src/construct_planning_scene.cpp
	src/semi_constrained_cartesian_planning.cpp
	)

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
add_dependencies(choreo_process_planning_node choreo_msgs_generate_messages_cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(choreo_process_planning_node
	${catkin_LIBRARIES}
	)

#############
## Install ##
#############

# Mark executables and/or libraries for installation
install(TARGETS choreo_process_planning_node
	ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
	RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
	)

## Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
	DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
	FILES_MATCHING PATTERN "*.h"
	PATTERN ".svn" EXCLUDE
	)

## Mark other files for installation (e.g. launch and bag files, etc.)
install(DIRECTORY launch
	DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
	)
