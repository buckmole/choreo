/*
* ==========================================================================
*		This file is part of the implementation of
*
*		<FrameFab: Robotic Fabrication of Frame Shapes>
*		Yijiang Huang, Juyong Zhang, Xin Hu, Guoxian Song, Zhongyuan Liu, Lei Yu, Ligang Liu
*		In ACM Transactions on Graphics (Proc. SIGGRAPH Asia 2016)
----------------------------------------------------------------------------
*		class:	SequenceAnalyzer
*
*		Description:  perform tool path searching algorithm to generate
*				a collision-free, structurally stable path.
*
*		Version: 2.0
*		Created: Oct/10/2015
*		Updated: Aug/24/2016
*
*		Author:  Xin Hu, Yijiang Huang, Guoxian Song
*		Company:  GCL@USTC
*		Successor:			FFAnalyzer - FrameFab sequence analyzer
*							BFAnalyzer - Brute Force sequence analyzer

----------------------------------------------------------------------------
*		Copyright (C) 2016  Yijiang Huang, Xin Hu, Guoxian Song, Juyong Zhang
*		and Ligang Liu.
*
*		FrameFab is free software: you can redistribute it and/or modify
*		it under the terms of the GNU General Public License as published by
*		the Free Software Foundation, either version 3 of the License, or
*		(at your option) any later version.
*
*		FrameFab is distributed in the hope that it will be useful,
*		but WITHOUT ANY WARRANTY; without even the implied warranty of
*		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*		GNU General Public License for more details.
*
*		You should have received a copy of the GNU General Public License
*		along with FrameFab.  If not, see <http://www.gnu.org/licenses/>.
* ==========================================================================
*/

#pragma once
#include <cmath>

#include "choreo_task_sequence_planner/utils/GCommon.h"
#include "choreo_task_sequence_planner/utils/Timer.h"
#include "choreo_task_sequence_planner/utils/WireFrame.h"
#include "choreo_task_sequence_planner/utils/DualGraph.h"
#include "choreo_task_sequence_planner/utils/Stiffness.h"
#include "choreo_task_sequence_planner/utils/WireFrame.h"

#include "choreo_task_sequence_planner/FiberPrintPARM.h"

#include "choreo_task_sequence_planner/utils/QuadricCollision.h"
#include "choreo_task_sequence_planner/utils/ResolveAngle.h"

// msgs
#include <moveit_msgs/CollisionObject.h>
#include <choreo_msgs/ElementCandidatePoses.h>
#include <choreo_msgs/WireFrameCollisionObject.h>

// robot model
#include <descartes_core/robot_model.h>
#include <pluginlib/class_loader.h>

struct SingleTaskPlanningResult
{
  WF_edge* e_;
  std::vector<Eigen::Vector3d> eef_directions_;
};

class SeqAnalyzer
{
 public:
  typedef Eigen::MatrixXd MX;
  typedef Eigen::Matrix3d M3;
  typedef Eigen::VectorXd VX;
  typedef Eigen::Vector3d V3;
  typedef std::array<bool, DIR_SPHERE_DIVISION> EEDirArray;

 public:
  explicit SeqAnalyzer(
      DualGraph			*ptr_dualgraph,
      QuadricCollision	*ptr_collision,
      Stiffness			*ptr_stiffness,
      FiberPrintPARM	*ptr_parm,
      char				*ptr_path,
      bool				terminal_output,
      bool				keep_timing,
      descartes_core::RobotModelPtr hotend_model,
      moveit::core::RobotModelConstPtr moveit_model,
      std::string hotend_group_name
  ) noexcept;

  virtual ~SeqAnalyzer();

  void setFrameMsgs(const std::vector<choreo_msgs::ElementCandidatePoses>& frame_msg){ frame_msgs_ = frame_msg; };

 public:
  virtual bool SeqPrint();
  virtual bool SeqPrintLayer(std::vector<int> layer_id);

  virtual void PrintOutTimer();

 public:
  bool InputPrintOrder(const std::vector<int>& print_queue);
  bool ConstructCollisionObjsInQueue(const std::vector<int>& print_queue_edge_ids,
                                     std::vector<choreo_msgs::WireFrameCollisionObject>& collision_objs);

  void OutputPrintOrder(std::vector<WF_edge*>& print_queue);
  void OutputTaskSequencePlanningResult(std::vector<SingleTaskPlanningResult>& planning_result);

 protected:
  void Init();

  void UpdateStructure(WF_edge *e, bool update_collision = false);
  void RecoverStructure(WF_edge *e, bool update_collision = false);

  void UpdateStateMap(const WF_edge *e, std::vector<EEDirArray> &state_map);
  void RecoverStateMap(const WF_edge *e, const std::vector<EEDirArray>& state_map);

  bool TestifyStiffness(WF_edge *e);
  bool TestRobotKinematics(WF_edge* e, const EEDirArray& cmap);

  WF_edge* RouteEdgeDirection(const WF_edge* prev_e, WF_edge* e);

 private:
  // robot kinematics related
  // the collision obj's update and recover are called
  // inside UpdateStructure and RecoverStructure.
  std::vector<moveit_msgs::CollisionObject> UpdateCollisionObjects(WF_edge* e, bool shrink = false);
  std::vector<moveit_msgs::CollisionObject> RecoverCollisionObjects(WF_edge* e, bool shrink = false);

 public:
  WireFrame* ptr_frame_;
  DualGraph* ptr_dualgraph_;
  Stiffness* ptr_stiffness_;
  QuadricCollision* ptr_collision_;
  char* ptr_path_;

  std::vector<choreo_msgs::ElementCandidatePoses> frame_msgs_;

 protected:
  /* maintaining for sequence */
  // number of unique edges in wireframe = number of vertices in dual graph
  int Nd_;

  // entire dual graph copy
  DualGraph* ptr_wholegraph_;

  std::vector<WF_edge*> print_queue_;

  std::vector<std::array<bool, DIR_SPHERE_DIVISION>> ee_dir_states_;

  VX D0_;

  /* parameters */
  double gamma_; // gamma_	: amplifier factor for adjacency cost
  double D_tol_; // Dt_tol	: tolerance of offset in stiffness
  double Wp_;	 // Wp_		: stablity weight for printing cost
  double Wa_;	 // Wa_		: adjacent weight for printing cost
  double Wi_;	 // Wl_		: influence weight for printing cost

  /* for debuging */
  bool terminal_output_;
  bool keep_timing_;

  bool update_collision_;

  Timer	test_stiff_;
  Timer test_kin_;
  Timer test_collision_;

  Timer upd_frame_;
  Timer retr_frame_;

  Timer upd_dir_map_;
  Timer retr_dir_map_;

  Timer upd_collision_;
  Timer retr_collision_;

  descartes_core::RobotModelPtr hotend_model_;
  moveit::core::RobotModelConstPtr moveit_model_;
  std::string hotend_group_name_;

  planning_scene::PlanningScenePtr planning_scene_;

  int num_p_assign_visited_;
  int num_backtrack_;

  int search_rerun_;
};
