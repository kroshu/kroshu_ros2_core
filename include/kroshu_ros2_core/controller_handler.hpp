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

#ifndef KROSHU_ROS2_CORE__CONTROLLER_HANDLER_HPP_
#define KROSHU_ROS2_CORE__CONTROLLER_HANDLER_HPP_

#include <string>
#include <vector>
#include <set>

#include "rclcpp/rclcpp.hpp"

namespace kroshu_ros2_core
{
/**
 * @brief This class is responsible for tracking the active controllers
 *  and on control mode change offer the controllers name that
 *  need to be activated and deactivated.
 * The following three public function has to be called in the following order:
 *  - Calculate_activate_deactivate_controllers() or Get_active_controllers_on_deactivation()
 *  - ApproveControllerActivation()
 *  - ApproveControllerDeactivation()
 */
class Controller_handler
{
public:
  /**
   * @brief Enum for identify every type of controllers
   */
  typedef enum   //controller_type
  {
    POSITION_CONTROLLER_TYPE = 0,
    CARTESIAN_POSITION_CONTROLLER_TYPE = 1,
    IMPEDANCE_CONTROLLER_TYPE = 2,
    CARTESIAN_IMPEDANCE_CONTROLLER_TYPE = 3,
    TORQUE_CONTROLLER_TYPE = 4,
    WRENCH_CONTROLLER_TYPE = 5
  } Controller_type;

private:
  /**
   * @brief Enum for identify every control mode
   */
  typedef enum
  {
    UNSPECIFIED_CONTROL_MODE = 0,
    POSITION_CONTROL_MODE = 1,
    CARTESIAN_POSITION_CONTROL_MODE = 2,
    IMPEDANCE_CONTROL_MODE = 3,
    CARTESIAN_IMPEDANCE_CONTROL_MODE = 4,
    TORQUE_CONTROL_MODE = 5,
    WRENCH_CONTROL_MODE = 6
  } control_mode;

  /**
   * @brief Controller names thats have to active at any control mode
   */
  std::vector<std::string> fixed_controllers_;

  /**
   * @brief The currently active controllers that not include the fixed controllers
   * The controllers are stored in an std::set type
   */
  std::set<std::string> active_controllers_;

  /**
   * @brief These controllers will be activated after they get approved
   */
  std::vector<std::string> activate_controllers_;

  /**
   * @brief These controllers will be deactivated after they get approved
   */
  std::vector<std::string> deactivate_controllers_;

  /**
   * @brief Look up table for wich controllers needed for each control mode
   */
  std::map<control_mode, std::vector<std::string>> control_mode_map_;

  /**
   * @brief There are two kinds of control modes with different number of necessary interfaces to be set:
   *  - in standard mode (possition, torque), only the control sgnal to the used interface
   *  - in impedance modes, the setpoint and the parameters describing the behavior
   */
  static constexpr int STANDARD_MODE_CONTROLLERS_SIZE = 1;
  static constexpr int IMPEDANCE_MODE_CONTROLLERS_SIZE = 2;

  /**
   * @brief Controllers possition in the controllers vector
   */
  static constexpr int NORMAL_CONTROLLER_POS = 0;
  static constexpr int IMPEDANCE_CONTROLLER_POS = 1;

  /**
   * @brief Retruns the currently active controllers for the user.
   *
   * @return std::vector<std::string>: The currently active controllers
   */
  std::vector<std::string> get_active_controllers();

public:
  /**
   * @brief Construct a new control mode handler object
   *
   * @param fixed_controllers: Controller names thats have to active at any control mode
   */
  Controller_handler(std::vector<std::string> fixed_controllers);

  /**
   * @brief Destroy the control mode handler object
   */
  ~Controller_handler() = default;

  /**
   * @brief Updates the controllers name for a specific controller type.
   *
   * @param controller_type: The type of the cotroller wich will be updated.
   * @param controller_name: The new controllers name. From now on this controller will be activated on controller acivation.
   * @return True, if update was succesfull.
   * @return False, if update failed.
   */
  bool Update_controller_name(
    const Controller_type controller_type,
    const std::string & controller_name);

  /**
   * @brief Calculates the controllers that have to be activated and deactivated for the control mode change
   *
   * @param new_control_mode: The new control mode. It is based on Controller_handler::control_mode enum.
   * @return std::pair<std::vector<std::string>, std::vector<std::string>>:
   * Two vectors, first has the contrllers to activate, second has the controllers to deactivate
   * @exception std::out_of_range: No control mode for the actual new_control_mode atribute.
   */
  std::pair<std::vector<std::string>,
    std::vector<std::string>> Get_activate_deactivate_controllers(
    int new_control_mode);

  /**
   * @brief Returns all controllers that has active state while deactivation
   *
   * @return std::pair<std::vector<std::string>, std::vector<std::string>>:
   * Two vectors, first has the contrllers to activate, second has the controllers to deactivate
   */
  std::pair<std::vector<std::string>,
    std::vector<std::string>> Get_active_controllers_on_deactivation();

  /**
   * @brief Approves that the controller activation was succesfull
   *
   */
  void ApproveControllerActivation();

  /**
   * @brief Approves that the controller deactivation was succesfull
   *
   */
  void ApproveControllerDeactivation();

  /**
   * @brief Clears the controller switch action
   *
   */
  void RejectControllerSwitch();
};
} // namespace kroshu_ros2_core


#endif  // KROSHU_ROS2_CORE__CONTROLLER_HANDLER_HPP_