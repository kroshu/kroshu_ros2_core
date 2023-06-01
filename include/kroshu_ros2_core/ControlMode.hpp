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


#ifndef KROSHU_ROS2_CORE__CONTROLMODE_HPP_
#define KROSHU_ROS2_CORE__CONTROLMODE_HPP_

namespace kroshu_ros2_core
{
/**
  * @brief Enum to identify every control mode
  */
enum class ControlMode : std::uint8_t
{
  UNSPECIFIED_CONTROL_MODE = 0,
  JOINT_POSITION_CONTROL_MODE = 1,
  CARTESIAN_POSITION_CONTROL_MODE = 2,
  JOINT_IMPEDANCE_CONTROL_MODE = 3,
  CARTESIAN_IMPEDANCE_CONTROL_MODE = 4,
  TORQUE_CONTROL_MODE = 5,
  WRENCH_CONTROL_MODE = 6
};

/**
   * @brief Enum for identify every type of controllers
   */
enum class ControllerType : std::uint8_t
{
  JOINT_POSITION_CONTROLLER_TYPE = 0,
  CARTESIAN_POSITION_CONTROLLER_TYPE = 1,
  JOINT_IMPEDANCE_CONTROLLER_TYPE = 2,
  CARTESIAN_IMPEDANCE_CONTROLLER_TYPE = 3,
  TORQUE_CONTROLLER_TYPE = 4,
  WRENCH_CONTROLLER_TYPE = 5
};
}  // namespace kroshu_ros2_core

#endif  // KROSHU_ROS2_CORE__CONTROLMODE_HPP_
