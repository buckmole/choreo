//
// Created by yijiangh on 9/4/17.
//

#include <choreo_process_planning/choreo_process_planning.h>
#include <ros/console.h>

// service
#include <choreo_msgs/MoveToTargetPose.h>

// msg
#include <trajectory_msgs/JointTrajectory.h>

#include "path_transitions.h"
#include "common_utils.h"

// for immediate execution
#include <actionlib/client/simple_action_client.h>
#include <control_msgs/FollowJointTrajectoryAction.h>

namespace choreo_process_planning
{

const static std::string JOINT_TOPIC_NAME =
    "joint_states"; // ROS topic to subscribe to for current robot state info

bool ProcessPlanningManager::handleMoveToTargetPosePlanAndExecution(
    choreo_msgs::MoveToTargetPose::Request& req,
    choreo_msgs::MoveToTargetPose::Response& res)
{
  if(req.type == req.JOINT_POSE)
  {
    std::vector<double> current_joints = getCurrentJointState(JOINT_TOPIC_NAME);
    const std::vector<std::string>& joint_names =
        moveit_model_->getJointModelGroup(hotend_group_name_)->getActiveJointModelNames();

    if(current_joints == req.pose)
    {
      // skip planning if required pose = current pose
      return true;
    }

    std::vector<double> target_pose(req.pose.end() - joint_names.size(), req.pose.end());

    trajectory_msgs::JointTrajectory ros_traj = getMoveitPlan(hotend_group_name_,
                                                              current_joints,
                                                              target_pose,
                                                              moveit_model_);

    ros::Duration base_time = ros_traj.points[0].time_from_start;

    double sim_time_scale = 0.6;
    for (int i = 0; i < ros_traj.points.size(); i++)
    {
      ros_traj.points[i].time_from_start -= base_time;

      //sim speed tuning
      ros_traj.points[i].time_from_start *= sim_time_scale;
    }

    fillTrajectoryHeaders(joint_names, ros_traj, world_frame_);

    // step 5: immediate execution (a quick solution for debugging)
    ros::NodeHandle nh;
    actionlib::SimpleActionClient <control_msgs::FollowJointTrajectoryAction> client(nh, "joint_trajectory_action");
    if (!client.waitForServer(ros::Duration(1.0)))
    {
      ROS_WARN("[Reset Exe] Exec timed out");
    }
    else
    {
      ROS_INFO("[Reset Exe] Found action server");
    }

    control_msgs::FollowJointTrajectoryGoal goal;
    goal.trajectory = ros_traj;

    client.sendGoalAndWait(goal);
    return true;
  }
}
}