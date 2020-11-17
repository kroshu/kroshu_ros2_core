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

#include <string>
#include <vector>
#include <memory>

#include "kroshu_ros2_core/ROS2BaseNode.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"

namespace kroshu_ros2_core
{

ROS2BaseNode::ROS2BaseNode(const std::string & node_name)
: rclcpp_lifecycle::LifecycleNode(node_name)
{
}

ROS2BaseNode::~ROS2BaseNode()
{
  ROS2BaseNode::on_shutdown(get_current_state());
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
ROS2BaseNode::on_configure(const rclcpp_lifecycle::State & state)
{
  return SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
ROS2BaseNode::on_cleanup(const rclcpp_lifecycle::State & state)
{
  return SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
ROS2BaseNode::on_shutdown(const rclcpp_lifecycle::State & state)
{
  auto result = SUCCESS;
  switch (state.id()) {
    case lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE:
      result = this->on_deactivate(get_current_state());
      if (result != SUCCESS) {
        break;
      }
      result = this->on_cleanup(get_current_state());
      break;
    case lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE:
      result = this->on_cleanup(get_current_state());
      break;
    case lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED:
      break;
    default:
      break;
  }
  return result;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
ROS2BaseNode::on_activate(const rclcpp_lifecycle::State & state)
{
  return SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
ROS2BaseNode::on_deactivate(const rclcpp_lifecycle::State & state)
{
  return SUCCESS;
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
ROS2BaseNode::on_error(const rclcpp_lifecycle::State & state)
{
  RCLCPP_INFO(get_logger(), "An error occured");
  return SUCCESS;
}

rcl_interfaces::msg::SetParametersResult ROS2BaseNode::onParamChange(
  const std::vector<rclcpp::Parameter> & parameters)
{
  rcl_interfaces::msg::SetParametersResult result;
  result.successful = false;
  for (const rclcpp::Parameter & param : parameters) {
    auto found_param_it = params_.find(param.get_name());
    if (found_param_it == params_.end()) {
      RCLCPP_ERROR(this->get_logger(), "Invalid parameter name");
    } else if (canSetParameter(*(found_param_it->second))) {
      result.successful = found_param_it->second->callCallback(param);
    }
  }
  return result;
}

bool ROS2BaseNode::canSetParameter(const kroshu_ros2_core::ParameterBase & param)
{
  try {
    if (!param.getRights().isSetAllowed(this->get_current_state().id())) {
      RCLCPP_ERROR(
        this->get_logger(),
        "Parameter " + param.getName() +
        " cannot be changed while in state %s",
        this->get_current_state().label().c_str());
      return false;
    }
  } catch (const std::out_of_range & e) {
    RCLCPP_ERROR(
      this->get_logger(),
      "Parameter set access rights for parameter %s couldn't be determined");
    return false;
  }
  return true;
}

}  // namespace kroshu_ros2_core
