//
// Created by yijiangh on 7/5/17.
//

#include "choreo_process_planning/choreo_process_planning.h"

#include <choreo_planning_capability/choreo_assembly_type_names.h>

#include <moveit/robot_model_loader/robot_model_loader.h>

// service
#include <moveit_msgs/ApplyPlanningScene.h>

const static std::string APPLY_PLANNING_SCENE_SERVICE = "apply_planning_scene";

choreo_process_planning::ProcessPlanningManager::ProcessPlanningManager(
    const std::string& world_frame, const std::string& hotend_group, const std::string& hotend_tcp, const std::string& hotend_base,
    const std::string& robot_model_plugin)
    : plugin_loader_("descartes_core", "descartes_core::RobotModel"),
      hotend_group_name_(hotend_group)
{
  world_frame_ = world_frame;

  // Attempt to load and initialize the printing robot model (hotend)
  hotend_model_ = plugin_loader_.createInstance(robot_model_plugin);
  if (!hotend_model_)
  {
    throw std::runtime_error(std::string("[FF_process_planning] Could not load: ") + robot_model_plugin);
  }

  if (!hotend_model_->initialize("robot_description", hotend_group, hotend_base, hotend_tcp))
  {
    throw std::runtime_error("[FF_process_planning]: Unable to initialize descartes robot model");
  }

  // Load the moveit model
  robot_model_loader::RobotModelLoader robot_model_loader("robot_description");
  moveit_model_ = robot_model_loader.getModel();

  if (moveit_model_.get() == NULL)
  {
    throw std::runtime_error("[FF_process_planning] Could not load moveit robot model");
  }

  planning_scene_diff_client_ =
      nh_.serviceClient<moveit_msgs::ApplyPlanningScene>(APPLY_PLANNING_SCENE_SERVICE);
}

bool choreo_process_planning::ProcessPlanningManager::handleProcessPlanning(
    choreo_msgs::ProcessPlanning::Request& req,
    choreo_msgs::ProcessPlanning::Response& res)
{
  if(choreo::ASSEMBLY_TYPE_SPATIAL_EXTRUSION == req.assembly_type)
  {
    return handlePrintPlanning(req, res);
  }

  if(choreo::ASSEMBLY_TYPE_PICKNPLACE == req.assembly_type)
  {
    return handlePickNPlacePlanning(req, res);
  }

  ROS_ERROR("process planning: unknown / unsupported assembly type.");
  return false;
}
