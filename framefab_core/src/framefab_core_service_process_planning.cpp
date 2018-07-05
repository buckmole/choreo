//
// Created by yijiangh on 7/5/17.
//
#include "framefab_core/framefab_core_service.h"

#include <choreo_planning_capability/choreo_assembly_type_names.h>

// services
#include <framefab_msgs/ProcessPlanning.h>
#include <framefab_msgs/MoveToTargetPose.h>
#include <framefab_msgs/TaskSequencePlanning.h>

bool FrameFabCoreService::generateMotionLibrary(
    const int selected_path_index, framefab_core_service::TrajectoryLibrary& traj_lib)
{
  ProcessPlanResult plan = generateProcessPlan(selected_path_index);

  traj_lib.get().clear();
  for (std::size_t k = 0; k < plan.plans.size(); ++k)
  {
    traj_lib.get()[std::to_string(k)] = plan.plans[k];
  }

  return plan.plans.size() > 0;
}

ProcessPlanResult FrameFabCoreService::generateProcessPlan(const int& selected_path_index)
{
  ProcessPlanResult result;

  bool fetch_co_success = false;
  std::vector<framefab_msgs::UnitProcessPlan> process_plan;

  // get task sequence file name as ladder graph file name
  std::string saved_graph_file_name = task_sequence_input_params_.file_path;
  std::replace(saved_graph_file_name.begin(), saved_graph_file_name.end(), '/', '_');

  // construct sequenced collision objects
  framefab_msgs::TaskSequencePlanning ts_srv;
  ts_srv.request.action = ts_srv.request.READ_WIREFRAME;
  ts_srv.request.model_params = model_input_params_;
  ts_srv.request.task_sequence_params = task_sequence_input_params_;

  // call process_processing srv
  framefab_msgs::ProcessPlanning srv;
  srv.request.index = selected_path_index;
  srv.request.use_saved_graph = use_saved_graph_;
  srv.request.file_name = saved_graph_file_name;
  srv.request.clt_rrt_unit_process_timeout = model_input_params_.clt_rrt_unit_process_timeout;
  srv.request.clt_rrt_timeout = model_input_params_.clt_rrt_timeout;

  if(choreo::ASSEMBLY_TYPE_PICKNPLACE == assembly_type_)
  {
    assert(as_pnp_.sequenced_elements.size() > 0);

    srv.request.assembly_type = choreo::ASSEMBLY_TYPE_PICKNPLACE;
    srv.request.as_pnp = as_pnp_;

    // no need to call seq planner for collision objs
    fetch_co_success = true;
  }

  if(choreo::ASSEMBLY_TYPE_SPATIAL_EXTRUSION == assembly_type_)
  {
    srv.request.assembly_type = choreo::ASSEMBLY_TYPE_SPATIAL_EXTRUSION;

    // first call sequence planner to request collision object (because it has model configuration ability...)
    if(!task_sequence_planning_srv_client_.call(ts_srv))
    {
      ROS_ERROR_STREAM("[Core] task sequence planning service read wireframe failed.");
      fetch_co_success = false;
    }
    else
    {
      ts_srv.request.action = ts_srv.request.REQUEST_COLLISION_OBJS;
      ts_srv.request.element_array = std::vector<framefab_msgs::ElementCandidatePoses>(
          task_sequence_.begin(), task_sequence_.begin() + selected_path_index + 1);

      if (!task_sequence_planning_srv_client_.call(ts_srv))
      {
        ROS_WARN_STREAM("[Core] task sequence planning service construct collision objects failed.");
        fetch_co_success = false;
      }
      else
      {
        srv.request.wf_collision_objs = ts_srv.response.wf_collision_objs;
        fetch_co_success = true;
      }
    }

    srv.request.task_sequence = task_sequence_;
    srv.request.env_collision_objs = env_objs_;
  }

  assert(fetch_co_success);

  if(fetch_co_success)
  {
    if (process_planning_client_.call(srv))
    {
      process_plan = srv.response.plan;

      for (auto v : process_plan)
      {
        result.plans.push_back(v);
      }
    }
    else
    {
      ROS_ERROR_STREAM("[Core] Failed to plan until path #" << selected_path_index << ", planning failed.");
    }
  }

  return result;
}

bool FrameFabCoreService::moveToTargetJointPose(std::vector<double> joint_pose)
{
  framefab_msgs::MoveToTargetPose srv;

  srv.request.type = srv.request.JOINT_POSE;
  srv.request.pose = joint_pose;

  for(auto j : joint_pose)
  {
    ROS_INFO_STREAM("reset joint: " << j);
  }

  return move_to_pose_client_.call(srv);
}
