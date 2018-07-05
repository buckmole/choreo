#ifndef CHOREO_VISUAL_TOOL_H
#define CHOREO_VISUAL_TOOL_H

// C++
#include <string>
#include <vector>

// ROS
#include <ros/ros.h>

#include <Eigen/Core>
#include <Eigen/Geometry>

// For visualizing things in rviz
#include <moveit_visual_tools/moveit_visual_tools.h>

#include <choreo_msgs/ElementCandidatePoses.h>
#include <choreo_msgs/AssemblySequencePickNPlace.h>

namespace choreo_visual_tools
{
// TODO: not used in picknplace
enum UNIT_PATH_TYPE
{
  SUPPORT=2,
  CREATE=1,
  CONNECT=0
};

struct VisualUnitProcessPath
{
  Eigen::Vector3d start_pt;
  Eigen::Vector3d end_pt;

  std::vector<Eigen::Vector3d> oriented_st_pts;
  Eigen::Vector3d avr_orient_vec;

  UNIT_PATH_TYPE type;
  double diameter;
  int layer_id;
};

class ChoreoVisualTools
{
  typedef std::vector<choreo_msgs::ElementCandidatePoses> PathArray;
  typedef std::vector<choreo_visual_tools::VisualUnitProcessPath> VisualPathArray;

 public:
  ChoreoVisualTools(){}
  virtual ~ChoreoVisualTools(){}

  // initiator: TODO: should merge into the constructor
  // framename: common base for all visualization markers, usually "/world" or "/base_link"
  // marker_topic - rostopic to publish markers to - your Rviz display should match
  // robot_model: load robot model pointer so that we don't have do re-parse it here
  void init(std::string frame_name,
            std::string marker_topic,
            robot_model::RobotModelConstPtr robot_model = robot_model::RobotModelConstPtr());

  // set visual wire frames
  // visualize layer decomposition and wire frames
  // when no assembly seq (with grasps) is inputted
  void setVisualWireFrame(const PathArray& path_array)
  {
    path_array_ = path_array;
    convertWireFrameVisual(path_array_, visual_path_array_);
  }

  // set visual assembly seq for spatial extrusion
  void setProcessPath(const PathArray& path_array)
  {
    path_array_ = path_array;
    convertPathVisual(path_array_, visual_path_array_);
  }

  int getPathArraySize() { return visual_path_array_.size(); }

  // VISUALIZE SEQUENCED EXTRUSIONS
  //
  // visualize all extrusion sequence at once, mark different types
  // of process: blue: support, yellow: create, red: connect
  void visualizeAllPaths();

  // visualize extrusion sequence until the given index
  // all elements are painted grey except element No. index
  void visualizePathUntil(int index);

  // visualize feasible orientations using green lines
  void visualizeFeasibleOrientations(int index, bool solid);
  //
  // VISUALIZE SEQUENCED EXTRUSIONS END

  //
  void visualizeAllWireFrame();

  //TODO: not implemented now
  void visualizeWireFrameUntilLayer(int index);

  // PICKNPLACE
  //
  void setAssemblySequencePickNPlace(const choreo_msgs::AssemblySequencePickNPlace& as_pnp) { as_pnp_ = as_pnp; }

  void visualizeSequencePickNPlaceUntil(int index);

  void visualizeGraspPickNPlace(int index, int grasp_id, bool visualize_ee);

  void visualizeAllSequencePickNPlace();

  void visualizeSupportSurfaces();
  void visualizeSupportSurfaces(const std::vector<std::string>& pick_contact_surf_names,
                                const std::vector<std::string>& place_contact_surf_names);
  //
  // PICKNPLACE END

  void cleanUpAllPaths();

 protected:
  void convertPathVisual(const PathArray& path_array, VisualPathArray& visual_path_array);
  void convertWireFrameVisual(const PathArray& path_array, VisualPathArray& visual_path_array);

 private:
  ros::NodeHandle nh_;

  moveit_visual_tools::MoveItVisualToolsPtr visual_tools_;

  PathArray path_array_;
  VisualPathArray visual_path_array_;

  // TODO: should try to make two assembly type's function unites under one scheme
  choreo_msgs::AssemblySequencePickNPlace as_pnp_;

  // Pointer to the robot model
  moveit::core::RobotModelConstPtr robot_model_;
};
}

#endif
