// Copyright 2023 Komáromi Sándor
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "kroshu_ros2_core/ControllerHandler.hpp"

namespace kroshu_ros2_core
{
ControllerHandler::ControllerHandler(std::vector<std::string> fixed_controllers)
: fixed_controllers_(fixed_controllers.begin(), fixed_controllers.end())
{}

bool ControllerHandler::UpdateControllerName(
  const ControllerHandler::ControllerType controller_type,
  const std::string & controller_name)
{
  switch (controller_type) {
    case ControllerType::JOINT_POSITION_CONTROLLER_TYPE:
      control_mode_map_[ControlMode::JOINT_POSITION_CONTROL_MODE].standard_controller =
        controller_name;
      control_mode_map_[ControlMode::JOINT_IMPEDANCE_CONTROL_MODE].standard_controller =
        controller_name;
      break;
    case ControllerType::CARTESIAN_POSITION_CONTROLLER_TYPE:
      control_mode_map_[ControlMode::CARTESIAN_POSITION_CONTROL_MODE].standard_controller =
        controller_name;
      control_mode_map_[ControlMode::CARTESIAN_IMPEDANCE_CONTROL_MODE].standard_controller =
        controller_name;
      break;
    case ControllerType::JOINT_IMPEDANCE_CONTROLLER_TYPE:
      control_mode_map_[ControlMode::JOINT_IMPEDANCE_CONTROL_MODE].impedance_cotroller =
        controller_name;
      break;
    case ControllerType::CARTESIAN_IMPEDANCE_CONTROLLER_TYPE:
      control_mode_map_[ControlMode::CARTESIAN_IMPEDANCE_CONTROL_MODE].impedance_cotroller =
        controller_name;
      break;
    case ControllerType::TORQUE_CONTROLLER_TYPE:
      control_mode_map_[ControlMode::TORQUE_CONTROL_MODE].standard_controller =
        controller_name;
      break;
    case ControllerType::WRENCH_CONTROLLER_TYPE:
      control_mode_map_[ControlMode::WRENCH_CONTROL_MODE].standard_controller =
        controller_name;
      break;
    default:
      RCLCPP_INFO(rclcpp::get_logger("ControllerHandler"), "Invalid Controller type");
      return false;
      break;
  }
  return true;
}

std::pair<std::vector<std::string>, std::vector<std::string>>
ControllerHandler::GetControllersForSwitch(ControllerHandler::ControlMode new_control_mode)
{
  if (control_mode_map_.find(ControlMode(new_control_mode)) == control_mode_map_.end()) {
    // Not valid control mode, through error
    throw std::out_of_range("Attribute new_control_mode is out of range");
  }

  if (ControlMode(new_control_mode) == ControlMode::UNSPECIFIED_CONTROL_MODE) {
    throw std::logic_error("UNSPECIFIED_CONTROL_MODE is not valid control mode");
  }

  // Set controllers wich should be activated and deactivated
  activate_controllers_.clear();
  auto control_mode_controllers = control_mode_map_.at(ControlMode(new_control_mode));
  activate_controllers_.insert(control_mode_controllers.standard_controller);
  if (!control_mode_controllers.impedance_cotroller.empty()) {
    activate_controllers_.insert(control_mode_controllers.impedance_cotroller);
  }

  activate_controllers_.merge(fixed_controllers_);

  deactivate_controllers_ = active_controllers_;


  // Goes through every controllers that should be activated
  for (auto activate_controllers_it = activate_controllers_.begin();
    activate_controllers_it != activate_controllers_.end(); ++activate_controllers_it)
  {
    // Finds the controller in the deactivate controllers
    if (deactivate_controllers_.find(*activate_controllers_it) != deactivate_controllers_.end()) {
      // Delete those controllers wich not need to be activated or deactivated.
      activate_controllers_.erase(activate_controllers_it);
      deactivate_controllers_.erase(activate_controllers_it);
      // Decrement iterators so it will not lose track;
      --activate_controllers_it;
    }
  }

  return std::make_pair(
    std::vector<std::string>(
      activate_controllers_.begin(),
      activate_controllers_.end()),
    std::vector<std::string>(
      deactivate_controllers_.begin(),
      deactivate_controllers_.end()));
}

std::vector<std::string> ControllerHandler::GetControllersForDeactivation()
{
  deactivate_controllers_ = active_controllers_;
  return std::vector<std::string>(deactivate_controllers_.begin(), deactivate_controllers_.end());
}

void ControllerHandler::ApproveControllerActivation()
{
  if (!activate_controllers_.empty()) {
    active_controllers_.merge(activate_controllers_);
    activate_controllers_.clear();
  }
}

void ControllerHandler::ApproveControllerDeactivation()
{
  if (!deactivate_controllers_.empty()) {
    active_controllers_.erase(deactivate_controllers_.begin(), deactivate_controllers_.end());
    deactivate_controllers_.clear();
  }
}
}   // namespace kroshu_ros2_core
