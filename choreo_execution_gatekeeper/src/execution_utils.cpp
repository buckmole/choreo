//
// Created by yijiangh on 7/10/17.
//

#include "execution_utils.h"

void choreo_execution_gatekeeper::appendTrajectory(trajectory_msgs::JointTrajectory& original,
                                               const trajectory_msgs::JointTrajectory& next)
{
  ros::Duration last_t =
      original.points.empty() ? ros::Duration(0.0) : original.points.back().time_from_start;
  for (std::size_t i = 0; i < next.points.size(); ++i)
  {
    trajectory_msgs::JointTrajectoryPoint pt = next.points[i];
    pt.time_from_start += last_t;

    original.points.push_back(pt);
  }
}