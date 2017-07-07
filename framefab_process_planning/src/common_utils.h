//
// Created by yijiangh on 7/5/17.
//

#ifndef FRAMEFAB_PROCESS_PLANNING_COMMON_UTILS_H
#define FRAMEFAB_PROCESS_PLANNING_COMMON_UTILS_H

#include <ros/ros.h>

// descartes
#include <descartes_core/trajectory_pt.h>
#include <descartes_core/robot_model.h>

// msgs
#include <sensor_msgs/JointState.h>
#include <moveit_msgs/CollisionObject.h>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/Point.h>

namespace framefab_process_planning
{
typedef std::vector<descartes_core::TrajectoryPtPtr> DescartesTraj;

std::vector<double> getCurrentJointState(const std::string& topic);

bool addCollisionObject(
    ros::ServiceClient& planning_scene, const moveit_msgs::CollisionObject& c_obj);

}

#endif //FRAMEFAB_PROCESS_PLANNING_COMMON_UTILS_H
