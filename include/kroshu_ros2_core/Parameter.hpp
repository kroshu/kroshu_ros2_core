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

#include "kroshu_ros2_core/ParameterBase.hpp"

namespace kroshu_ros2_core
{

template<typename T>
class Parameter : public ParameterBase
{
public:
  Parameter(
    rclcpp_lifecycle::LifecycleNode::SharedPtr node,
    const std::string & name, const T & value, const ParameterSetAccessRights & rights,
    std::function<bool(const kroshu_ros2_core::Parameter<T> &)> on_change_callback)
  : ParameterBase(node, name, rights), on_change_callback_(on_change_callback)
  {
    node_->declare_parameter(name, value);
  }

  bool getValue(T & place_holder) const
  {
    return node_->get_parameter(name_, place_holder);
  }

  bool callCallback()
  {
    return on_change_callback_(*this);
  }

private:
  const std::function<bool(const kroshu_ros2_core::Parameter<T> &)> on_change_callback_;
};

}  // namespace kroshu_ros2_core

#endif  // KROSHU_ROS2_CORE__PARAMETER_HPP_
