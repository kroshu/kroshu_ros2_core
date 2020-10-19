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

#ifndef KROSHU_ROS2_CORE__ROS2BASENODE_HPP_
#define KROSHU_ROS2_CORE__ROS2BASENODE_HPP_

#include <string>
#include <map>
#include <vector>

#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "lifecycle_msgs/msg/state.hpp"

#include "kroshu_ros2_core/Parameter.hpp"

namespace kroshu_ros2_core
{

class ROS2BaseNode : public rclcpp_lifecycle::LifecycleNode
{
public:
  explicit ROS2BaseNode(const std::string & node_name);
  virtual ~ROS2BaseNode();

  virtual rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_configure(const rclcpp_lifecycle::State & state);

  virtual rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_cleanup(const rclcpp_lifecycle::State & state);

  virtual rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_shutdown(const rclcpp_lifecycle::State & state);

  virtual rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_activate(const rclcpp_lifecycle::State & state);

  virtual rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_deactivate(const rclcpp_lifecycle::State & state);

  virtual rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_error(const rclcpp_lifecycle::State & state);

protected:
  rcl_interfaces::msg::SetParametersResult onParamChange(
    const std::vector<rclcpp::Parameter> & parameters);
  bool canSetParameter(const kroshu_ros2_core::Parameter & param);
  bool setParameter(kroshu_ros2_core::Parameter & param, const rclcpp::ParameterValue & value);
  std::map<std::string, kroshu_ros2_core::Parameter> params_;
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn SUCCESS =
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn ERROR =
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::ERROR;
  void declareParameter(
    const std::string & name, const rclcpp::ParameterValue & value,
    const rclcpp::ParameterType & type, const ParameterSetAccessRights & rights,
    const std::function<bool(const kroshu_ros2_core::Parameter &)> & on_change_callback);
};

}  // namespace kroshu_ros2_core

#endif  // KROSHU_ROS2_CORE__ROS2BASENODE_HPP_
