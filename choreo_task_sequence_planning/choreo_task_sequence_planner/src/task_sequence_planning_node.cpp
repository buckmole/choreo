//
// Created by yijiangh on 12/21/17.
//

#include <ros/ros.h>

// choreo dependencies
#include "choreo_task_sequence_planner/FiberPrintPARM.h"
#include "choreo_task_sequence_planner/utils/WireFrame.h"
#include "choreo_task_sequence_planner/FiberPrintPlugIn.h"

// srv
#include <choreo_msgs/TaskSequencePlanning.h>

// Globals
const static std::string DEFAULT_TASK_SEQUENCE_PLANNING_SERVICE = "task_sequence_planning";

int main(int argc, char** argv)
{
  ros::init(argc, argv, "choreo_task_sequence_planning");
  ros::NodeHandle nh, pnh("~");

  std::string world_frame, hotend_group, hotend_tcp, hotend_base, robot_model_plugin;
  pnh.param<std::string>("world_frame", world_frame, "");
  pnh.param<std::string>("hotend_group", hotend_group, "");
  pnh.param<std::string>("hotend_tcp", hotend_tcp, "");
  pnh.param<std::string>("hotend_base", hotend_base, "");
  pnh.param<std::string>("robot_model_plugin", robot_model_plugin, "");

  // IK Plugin parameter must be specified
  if (robot_model_plugin.empty())
  {
    ROS_ERROR_STREAM("MUST SPECIFY PARAMETER 'robot_model_plugin' for choreo_process_planning node");
    return -1;
  }

  FiberPrintPlugIn fiber_print_plugin(world_frame, hotend_group, hotend_tcp, world_frame, robot_model_plugin);

  ros::ServiceServer task_sequence_planning_server =
      nh.advertiseService(DEFAULT_TASK_SEQUENCE_PLANNING_SERVICE,
                          &FiberPrintPlugIn::handleTaskSequencePlanning, &fiber_print_plugin);

  // Serve and wait for shutdown
  ROS_INFO_STREAM("[tsp] sequence task planning server online");
  ros::spin();

  return 0;
}