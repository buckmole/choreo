//
// Created by yijiangh on 4/3/18.
//

#include <choreo_process_planning/choreo_process_planning.h>

#include "generate_motion_plan.h"
#include "path_transitions.h"
#include "common_utils.h"

#include <choreo_msgs/SequencedElement.h>

#include <ros/console.h>

namespace {

const static std::string JOINT_TOPIC_NAME =
    "joint_states"; // ROS topic to subscribe to for current robot state info

} // end anon util namespace

namespace choreo_process_planning
{

bool ProcessPlanningManager::handlePickNPlacePlanning(
    choreo_msgs::ProcessPlanning::Request& req,
    choreo_msgs::ProcessPlanning::Response& res)
{
  // Enable Collision Checks
  hotend_model_->setCheckCollisions(true);

  assert(req.as_pnp.sequenced_elements.size() > 0);

  const static double LINEAR_VEL = 0.5; // (m/s)
  const static double LINEAR_DISC = 0.003; // meters

  // TODO: assuming current robot pose is the home pose, this should be read from ros parameter
  std::vector<double> current_joints = getCurrentJointState(JOINT_TOPIC_NAME);

  // copy & crop up to the required index
  assert(req.index > 0 && req.index < req.as_pnp.sequenced_elements.size());
  choreo_msgs::AssemblySequencePickNPlace as_pnp = req.as_pnp;
  as_pnp.sequenced_elements = std::vector<choreo_msgs::SequencedElement>(
      as_pnp.sequenced_elements.begin(), as_pnp.sequenced_elements.begin() + req.index + 1);

  // TODO: this shouldn't remove collision objs in xacro?
  // clear existing objs from previous planning
  clearAllCollisionObjects(planning_scene_diff_client_);

  if(generateMotionPlan(world_frame_,
                        req.use_saved_graph,
                        req.file_name,
                        req.clt_rrt_unit_process_timeout,
                        req.clt_rrt_timeout,
                        LINEAR_VEL,
                        LINEAR_DISC,
                        hotend_group_name_,
                        current_joints,
                        as_pnp,
                        hotend_model_,
                        moveit_model_,
                        planning_scene_diff_client_,
                        res.plan))
  {
    return true;
  }
  else
  {
    return false;
  }
}

}// end ff_process_planning ns