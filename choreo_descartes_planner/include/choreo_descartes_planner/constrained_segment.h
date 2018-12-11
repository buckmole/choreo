//
// Created by yijiangh on 4/10/18.
//

#ifndef FRAMEFAB_MPP_CONSTRAINED_SEGMENT_H
#define FRAMEFAB_MPP_CONSTRAINED_SEGMENT_H

#include <moveit/planning_scene/planning_scene.h>
#include <Eigen/Geometry>
#include <vector>

namespace descartes_planner
{

struct ConstrainedSegment
{
  enum PROCESS_TYPE
  {
    CREATE,
    CONNECT,
    SUPPORT
  };

  Eigen::Vector3d start, end; /** Start and end of the linear segment in 3-space */
  std::vector<Eigen::Matrix3d> orientations; /** All of the allowable orientations of the tool for this path */

  double linear_disc; /** The distance between sampled points in linear space (I'd like to do this automatically) */
  double linear_vel; /** Linear velocity between each disc point */

  double retract_dist;
  int retract_disc;
  double retract_vel;

  // TODO: spatial extrusion specific
  double z_axis_disc; /** The distance between angular steps about z for each orientation */
  PROCESS_TYPE process_type; /** create type, one node is floating; connect type, both of the nodes have been printed */
};

struct ConstrainedSegmentPickNPlace
{
  std::vector<std::vector<Eigen::Vector3d>> path_pts;
  std::vector<std::vector<Eigen::Matrix3d>> orientations;
  std::vector<planning_scene::PlanningScenePtr> planning_scenes;

  double linear_disc; /** The distance between sampled points in linear space (I'd like to do this automatically) */
  double linear_vel; /** Linear velocity between each disc point */
};

}

#endif //FRAMEFAB_MPP_CONSTRAINED_SEGMENT_H
