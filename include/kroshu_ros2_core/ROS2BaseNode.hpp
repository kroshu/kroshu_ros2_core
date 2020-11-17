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
#include <memory>

#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "lifecycle_msgs/msg/state.hpp"

#include "kroshu_ros2_core/Parameter.hpp"
#include "kroshu_ros2_core/ParameterBase.hpp"

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

  template<typename T>
  void declareParameter(
    const std::string & name, const T & value,
    const ParameterSetAccessRights & rights,
    std::function<bool(const T &)> on_change_callback)
  {
    auto found_iter = params_.find(name);
    if (found_iter != params_.end()) {
      RCLCPP_ERROR(
        this->get_logger(),
        "Parameter %s already declared",
        name.c_str());
    } else {
      auto declare_param_lambda = [this](const std::string & param_name,
          const T & param_value) {
          this->declare_parameter(param_name, param_value);
        };

      auto get_param_lambda = [this](const std::string & param_name, T & place_holder) {
          return this->get_parameter(param_name, place_holder);
        };

      auto set_param_lambda = [this](const rclcpp::Parameter & new_param) {
          auto result = this->set_parameter(new_param);
          return result.successful;
        };

      auto param_shared_ptr = std::make_shared<kroshu_ros2_core::Parameter<T>>(
        name, value,
        rights, on_change_callback,
        declare_param_lambda,
        get_param_lambda,
        set_param_lambda);
      params_.emplace(name, param_shared_ptr);
    }
  }

  bool canSetParameter(const kroshu_ros2_core::ParameterBase & param);

  std::map<std::string, std::shared_ptr<kroshu_ros2_core::ParameterBase>> params_;
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn SUCCESS =
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn ERROR =
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::ERROR;
};

}  // namespace kroshu_ros2_core

#endif  // KROSHU_ROS2_CORE__ROS2BASENODE_HPP_
