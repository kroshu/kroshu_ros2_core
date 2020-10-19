// Copyright 2020 Gergely Kovács
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

#ifndef KROSHU_ROS2_CORE__PARAMETER_HPP_
#define KROSHU_ROS2_CORE__PARAMETER_HPP_

#include <string>
#include <functional>

#include "rclcpp/rclcpp.hpp"
#include "lifecycle_msgs/msg/state.hpp"

namespace kroshu_ros2_core
{

struct ParameterSetAccessRights
{
  bool unconfigured;
  bool inactive;
  bool active;
  bool finalized;
  bool isSetAllowed(std::uint8_t current_state) const
  {
    switch (current_state) {
      case lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED:
        return unconfigured;
      case lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE:
        return inactive;
      case lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE:
        return active;
      case lifecycle_msgs::msg::State::PRIMARY_STATE_FINALIZED:
        return finalized;
      default:
        return false;
    }
  }
};

class Parameter
{
public:
  Parameter(
    const std::string & name, const rclcpp::ParameterValue & value,
    const rclcpp::ParameterType & type, const ParameterSetAccessRights & rights,
    const std::function<bool(const kroshu_ros2_core::Parameter &)> & on_change_callback);
  const std::string & getName() const;
  const rclcpp::ParameterValue & getValue() const;
  const rclcpp::ParameterType getType() const;
  const ParameterSetAccessRights & getRights() const;
  const std::function<bool(const kroshu_ros2_core::Parameter &)> & getCallback() const;
  bool setValue(const rclcpp::ParameterValue &);

private:
  const std::string name_;
  rclcpp::ParameterValue value_;
  const rclcpp::ParameterType type_;
  const ParameterSetAccessRights rights_;
  const std::function<bool(const kroshu_ros2_core::Parameter &)> on_change_callback_;
};

}  // namespace kroshu_ros2_core

#endif  // KROSHU_ROS2_CORE__PARAMETER_HPP_
