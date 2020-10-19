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
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn result = SUCCESS;
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

const rclcpp::ParameterValue getValueHelper(const rclcpp::Parameter & param)
{
  switch (param.get_type()) {
    case rclcpp::ParameterType::PARAMETER_BOOL:
      return rclcpp::ParameterValue(param.as_bool());
    case rclcpp::ParameterType::PARAMETER_INTEGER:
      return rclcpp::ParameterValue(param.as_int());
    case rclcpp::ParameterType::PARAMETER_DOUBLE:
      return rclcpp::ParameterValue(param.as_double());
    case rclcpp::ParameterType::PARAMETER_STRING:
      return rclcpp::ParameterValue(param.as_string());
    case rclcpp::ParameterType::PARAMETER_BYTE_ARRAY:
      return rclcpp::ParameterValue(param.as_byte_array());
    case rclcpp::ParameterType::PARAMETER_BOOL_ARRAY:
      return rclcpp::ParameterValue(param.as_bool_array());
    case rclcpp::ParameterType::PARAMETER_INTEGER_ARRAY:
      return rclcpp::ParameterValue(param.as_integer_array());
    case rclcpp::ParameterType::PARAMETER_DOUBLE_ARRAY:
      return rclcpp::ParameterValue(param.as_double_array());
    case rclcpp::ParameterType::PARAMETER_STRING_ARRAY:
      return rclcpp::ParameterValue(param.as_string_array());
    default:
      return rclcpp::ParameterValue();
  }
}

rcl_interfaces::msg::SetParametersResult ROS2BaseNode::onParamChange(
  const std::vector<rclcpp::Parameter> & parameters)
{
  rcl_interfaces::msg::SetParametersResult result;
  result.successful = false;
  for (const rclcpp::Parameter & param : parameters) {
    auto found_param_it = params_.find(param.get_name());
    if (found_param_it == params_.end()) {
      RCLCPP_ERROR(this->get_logger(), "Invalid parameter name %s",
        param.get_name().c_str());
    } else if (param.get_type() != found_param_it->second.getType()) {
      RCLCPP_ERROR(this->get_logger(), "Invalid parameter type for parameter %s",
        param.get_name().c_str());
    } else {
      result.successful = setParameter(found_param_it->second, getValueHelper(param));
    }
  }
  return result;
}

bool ROS2BaseNode::canSetParameter(const kroshu_ros2_core::Parameter & param)
{
  try {
    if (!param.getRights().isSetAllowed(this->get_current_state().id())) {
      RCLCPP_ERROR(this->get_logger(), "Parameter " + param.getName() +
        " cannot be changed while in state %s",
        this->get_current_state().label().c_str());
      return false;
    }
  } catch (const std::out_of_range & e) {
    RCLCPP_ERROR(this->get_logger(),
      "Parameter set access rights for parameter %s couldn't be determined",
      param.getName().c_str());
    return false;
  }
  return true;
}

bool ROS2BaseNode::setParameter(
  kroshu_ros2_core::Parameter & param,
  const rclcpp::ParameterValue & value)
{
  if (!canSetParameter(param)) {
    return false;
  }

  param.setValue(value);
  auto return_success = param.getCallback()(param);
  return return_success;
}

void ROS2BaseNode::declareParameter(
  const std::string & name, const rclcpp::ParameterValue & value,
  const rclcpp::ParameterType & type, const ParameterSetAccessRights & rights,
  const std::function<bool(const kroshu_ros2_core::Parameter &)> & on_change_callback)
{
  auto found_iter = params_.find(name);
  if (found_iter != params_.end()) {
    RCLCPP_ERROR(this->get_logger(),
      "Parameter %s already declared",
      name.c_str());
  } else {
    kroshu_ros2_core::Parameter new_param(name, value, type,
      rights, on_change_callback);
    params_.emplace(name, new_param);
    this->declare_parameter(name, value);
  }
}

}  // namespace kroshu_ros2_core
