#include <choreo_gui/params_submenu.h>
#include <ui_params_submenu.h>

choreo_gui::ParamsSubmenu::ParamsSubmenu(QWidget* parent) : QWidget(parent)
{
  ui_ = new Ui::ParamsSubmenu();
  ui_->setupUi(this);

  // Set up option menus
  // ModelInput
  model_input_widget_ = new ModelInputConfigWidget(choreo_msgs::ModelInputParameters());
  connect(ui_->pushbutton_model_input, SIGNAL(clicked()), model_input_widget_, SLOT(show()));

  // Path Input
  task_sequence_input_widget_ = new TaskSequenceInputConfigWidget(choreo_msgs::TaskSequenceInputParameters());
  connect(ui_->pushbutton_task_sequence_input, SIGNAL(clicked()), task_sequence_input_widget_, SLOT(show()));

  // Robot Input
  robot_input_widget_ = new RobotInputConfigWidget(choreo_msgs::RobotInputParameters());
  connect(ui_->pushbutton_robot_input, SIGNAL(clicked()), robot_input_widget_, SLOT(show()));

  // Output Path Input
  output_save_dir_input_widget_ = new OutputSaveDirInputConfigWidget(choreo_msgs::OutputSaveDirInputParameters());
  connect(ui_->pushbutton_output_save_dir_input, SIGNAL(clicked()), output_save_dir_input_widget_, SLOT(show()));

  // Save Request Connection
  connect(model_input_widget_, SIGNAL(parameters_save_requested()), this, SIGNAL(saveRequested()));
  connect(task_sequence_input_widget_, SIGNAL(parameters_save_requested()), this, SIGNAL(saveRequested()));
  connect(robot_input_widget_, SIGNAL(parameters_save_requested()), this, SIGNAL(saveRequested()));
  connect(output_save_dir_input_widget_, SIGNAL(parameters_save_requested()), this, SIGNAL(saveRequested()));

  // Accept Request Connection
  connect(model_input_widget_, SIGNAL(parameters_changed()), this, SIGNAL(acceptRequested()));
  connect(task_sequence_input_widget_, SIGNAL(parameters_changed()), this, SIGNAL(acceptRequested()));
  connect(robot_input_widget_, SIGNAL(parameters_changed()), this, SIGNAL(acceptRequested()));
  connect(output_save_dir_input_widget_, SIGNAL(parameters_changed()), this, SIGNAL(acceptRequested()));
}

void choreo_gui::ParamsSubmenu::showOutputSaveDirInputConfigWidget(bool enable)
{
  if(enable)
  {
    output_save_dir_input_widget_->show();
  }
  else
  {
    output_save_dir_input_widget_->hide();
  }
}

const choreo_msgs::ModelInputParameters& choreo_gui::ParamsSubmenu::modelInputParams() const
{
  return model_input_widget_->params();
}

void choreo_gui::ParamsSubmenu::setModelInputParams(const choreo_msgs::ModelInputParameters& params)
{
  model_input_widget_->params() = params;
  model_input_widget_->update_display_fields();
}

const choreo_msgs::TaskSequenceInputParameters& choreo_gui::ParamsSubmenu::taskSequenceInputParams() const
{
  return task_sequence_input_widget_->params();
}

void choreo_gui::ParamsSubmenu::setTaskSequenceInputParams(const choreo_msgs::TaskSequenceInputParameters& params)
{
  task_sequence_input_widget_->params() = params;
  task_sequence_input_widget_->update_display_fields();
}

const choreo_msgs::RobotInputParameters& choreo_gui::ParamsSubmenu::robotInputParams() const
{
  return robot_input_widget_->params();
}

void choreo_gui::ParamsSubmenu::setRobotInputParams(const choreo_msgs::RobotInputParameters& params)
{
  robot_input_widget_->params() = params;
  robot_input_widget_->update_display_fields();
}

const choreo_msgs::OutputSaveDirInputParameters& choreo_gui::ParamsSubmenu::outputSaveDirInputParams() const
{
  return output_save_dir_input_widget_->params();
}

void choreo_gui::ParamsSubmenu::setOutputSaveDirInputParams(const choreo_msgs::OutputSaveDirInputParameters& params)
{
  output_save_dir_input_widget_->params() = params;
  output_save_dir_input_widget_->update_display_fields();
}