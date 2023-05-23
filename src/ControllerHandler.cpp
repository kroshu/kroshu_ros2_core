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
Controller_handler::Controller_handler(std::vector<std::string> fixed_controllers)
: fixed_controllers_(fixed_controllers)
{
  control_mode_map_.emplace(
    std::make_pair(
      control_mode::POSITION_CONTROL_MODE,
      std::vector<std::string>(STANDARD_MODE_CONTROLLERS_SIZE)));
  control_mode_map_.emplace(
    std::make_pair(
      control_mode::CARTESIAN_POSITION_CONTROL_MODE,
      std::vector<std::string>(STANDARD_MODE_CONTROLLERS_SIZE)));
  control_mode_map_.emplace(
    std::make_pair(
      control_mode::IMPEDANCE_CONTROL_MODE,
      std::vector<std::string>(IMPEDANCE_MODE_CONTROLLERS_SIZE)));
  control_mode_map_.emplace(
    std::make_pair(
      control_mode::CARTESIAN_IMPEDANCE_CONTROL_MODE,
      std::vector<std::string>(IMPEDANCE_MODE_CONTROLLERS_SIZE)));
  control_mode_map_.emplace(
    std::make_pair(
      control_mode::TORQUE_CONTROL_MODE,
      std::vector<std::string>(STANDARD_MODE_CONTROLLERS_SIZE)));
  control_mode_map_.emplace(
    std::make_pair(
      control_mode::WRENCH_CONTROL_MODE,
      std::vector<std::string>(STANDARD_MODE_CONTROLLERS_SIZE)));
}

bool Controller_handler::Update_controller_name(
  const Controller_handler::Controller_type controller_type,
  const std::string & controller_name)
{
  switch (controller_type) {
    case Controller_type::POSITION_CONTROLLER_TYPE:
      control_mode_map_.at(control_mode::POSITION_CONTROL_MODE).at(NORMAL_CONTROLLER_POS) =
        controller_name;
      control_mode_map_.at(control_mode::IMPEDANCE_CONTROL_MODE).at(NORMAL_CONTROLLER_POS) =
        controller_name;
      break;
    case Controller_type::CARTESIAN_POSITION_CONTROLLER_TYPE:
      control_mode_map_.at(control_mode::CARTESIAN_POSITION_CONTROL_MODE).at(NORMAL_CONTROLLER_POS)
        =
        controller_name;
      control_mode_map_.at(control_mode::CARTESIAN_IMPEDANCE_CONTROL_MODE).at(NORMAL_CONTROLLER_POS)
        =
        controller_name;
      break;
    case Controller_type::IMPEDANCE_CONTROLLER_TYPE:
      control_mode_map_.at(control_mode::IMPEDANCE_CONTROL_MODE).at(IMPEDANCE_CONTROLLER_POS) =
        controller_name;
      break;
    case Controller_type::CARTESIAN_IMPEDANCE_CONTROLLER_TYPE:
      control_mode_map_.at(control_mode::CARTESIAN_IMPEDANCE_CONTROL_MODE).at(
        IMPEDANCE_CONTROLLER_POS) = controller_name;
      break;
    case Controller_type::TORQUE_CONTROLLER_TYPE:
      control_mode_map_.at(control_mode::TORQUE_CONTROL_MODE).at(NORMAL_CONTROLLER_POS) =
        controller_name;
      break;
    case Controller_type::WRENCH_CONTROLLER_TYPE:
      control_mode_map_.at(control_mode::WRENCH_CONTROL_MODE).at(NORMAL_CONTROLLER_POS) =
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
Controller_handler::Get_activate_deactivate_controllers(int new_control_mode)
{
  if (control_mode_map_.find(control_mode(new_control_mode)) == control_mode_map_.end()) {
    // Not valid control mode, threw error
    throw std::out_of_range("Atribute new_control_mode is out of range");
  }

  // TODO (komaromi): Implement case for UNSPECIFIED CONTROL MODE
  if (control_mode(new_control_mode) == control_mode::UNSPECIFIED_CONTROL_MODE) {
    throw std::logic_error("Not implemented exepton");
  }

  // Set controllers wich should be activated and deactivated
  activate_controllers_ = control_mode_map_.at(control_mode(new_control_mode));
  deactivate_controllers_ = get_active_controllers();

  // Goes through every controllers that should be deactivated
  for (auto deactivate_controllers_it = deactivate_controllers_.begin();
    deactivate_controllers_it != deactivate_controllers_.end(); ++deactivate_controllers_it)
  {
    // Goes through every controllers that should be activated
    for (auto activate_controllers_it = activate_controllers_.begin();
      activate_controllers_it != activate_controllers_.end(); ++activate_controllers_it)
    {
      if (*activate_controllers_it == *deactivate_controllers_it) {
        // Delete those controllers wich not need to be activated or deactivated.
        activate_controllers_.erase(activate_controllers_it);
        deactivate_controllers_.erase(deactivate_controllers_it);
        // Decrement iterators so it will not lose track;
        --activate_controllers_it;
        --deactivate_controllers_it;
        break;
      }
    }
  }
  // TODO (komaromi): Somewhere the active_controllers have to be updated, unless it wont work.
  return std::make_pair(activate_controllers_, deactivate_controllers_);
}

std::pair<std::vector<std::string>,
  std::vector<std::string>> Controller_handler::Get_active_controllers_on_deactivation()
{
  activate_controllers_ = {};
  deactivate_controllers_ = get_active_controllers();
  return std::make_pair(activate_controllers_, deactivate_controllers_);
}

std::vector<std::string> Controller_handler::get_active_controllers()
{
  return std::vector<std::string>(active_controllers_.begin(), active_controllers_.end());
}

void Controller_handler::ApproveControllerActivation()
{
  if (activate_controllers_.size() > 0) {
    std::copy(
      activate_controllers_.begin(), activate_controllers_.end(),
      std::inserter(active_controllers_, active_controllers_.end()));
  }
}

void Controller_handler::ApproveControllerDeactivation()
{
  if (deactivate_controllers_.size() > 0) {
    for (auto && controller : deactivate_controllers_) {
      active_controllers_.erase(active_controllers_.find(controller));
    }
  }
}
}   // namespace kroshu_ros2_core
