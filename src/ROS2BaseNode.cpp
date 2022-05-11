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

#include <rclcpp_lifecycle/lifecycle_node.hpp>

#include <string>
#include <vector>
#include <memory>

#include "kroshu_ros2_core/ROS2BaseNode.hpp"


namespace kroshu_ros2_core
{

template<typename T>
void ROS2BaseNode::registerParameter(
  const std::string & name, const T & value,
  std::function<bool(const T &)> on_change_callback)
{
  auto param_shared_ptr = std::make_shared<Parameter<T>>(
    name, value, ParameterSetAccessRights(),
    on_change_callback, this->get_node_parameters_interface());
  param_handler_->registerParameter(param_shared_ptr);
}

}  // namespace kroshu_ros2_core
