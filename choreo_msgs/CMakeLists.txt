cmake_minimum_required(VERSION 2.8.3)
project(choreo_msgs)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
	actionlib
	actionlib_msgs
	genmsg
	geometry_msgs
	message_generation
	sensor_msgs
	trajectory_msgs
	visualization_msgs
	moveit_msgs
	)

#######################################
## Declare ROS messages and services ##
#######################################

## Generate messages in the 'action' folder
add_action_files(
	DIRECTORY
	action
	FILES
	TaskSequenceProcessing.action
	TaskSequencePlanning.action
	ProcessPlanning.action
	SimulateMotionPlan.action
	ProcessExecution.action
)

## Generate messages in the 'msg' folder
add_message_files(
	DIRECTORY
	msg
	FILES
	ModelInputParameters.msg
	TaskSequenceInputParameters.msg
	RobotInputParameters.msg
	OutputSaveDirInputParameters.msg
	UnitProcessPlan.msg
	SubProcess.msg
	ElementCandidatePoses.msg
	CollisionObjectList.msg
	WireFrameCollisionObject.msg
	Grasp.msg
	SequencedElement.msg
	AssemblySequencePickNPlace.msg
)

## Generate services in the 'srv' folder
add_service_files(
	FILES
	ChoreoParameters.srv
	TaskSequenceProcessing.srv
	TaskSequencePlanning.srv
	ElementNumberRequest.srv
	VisualizeSelectedPath.srv
	ProcessPlanning.srv
	TrajectoryExecution.srv
	MoveToTargetPose.srv
	GetAvailableProcessPlans.srv
	OutputProcessPlans.srv
	OutputProcessing.srv
	QueryComputationRecord.srv
)

## Generate added messages and services with any dependencies listed here
generate_messages(DEPENDENCIES
	actionlib_msgs
	geometry_msgs
	sensor_msgs
	visualization_msgs
	trajectory_msgs
	moveit_msgs
	)

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
	CATKIN_DEPENDS
	actionlib
	actionlib_msgs
	message_runtime
	geometry_msgs
	sensor_msgs
	moveit_msgs
	genmsg
)
