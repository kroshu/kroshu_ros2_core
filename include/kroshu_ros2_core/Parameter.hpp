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

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "kroshu_ros2_core/ParameterBase.hpp"

namespace kroshu_ros2_core
{

template<typename T>
class Parameter : public ParameterBase
{
public:
  Parameter(
    const std::string & name, const T & value, const ParameterSetAccessRights & rights,
    std::function<bool(const T &)> on_change_callback,
    std::function<void(const std::string &, const T &)> declare_func,
    std::function<bool(const std::string &, T &)> get_param_func,
    std::function<
      bool(const rclcpp::Parameter &)>
    set_param_func)
  : ParameterBase(name, rights), on_change_callback_(on_change_callback),
    get_param_func_(get_param_func), set_param_func_(set_param_func)
  {
    declare_func(name_, value);
  }

  ~Parameter()
  {
    std::cout << "Destructor of " << name_ << std::endl;
  }

  bool getValue(
    T & place_holder) const
  {
    return get_param_func_(name_, place_holder);
  }

  bool setValue(const T & new_value) const
  {
    rclcpp::Parameter new_param(name_, new_value);
    return set_param_func_(new_param);
  }

  bool callCallback(const rclcpp::Parameter & new_param)
  {
    return on_change_callback_(new_param.get_value<T>());
  }

private:
  const std::function<bool(const T &)> on_change_callback_;
  const std::function<bool(const std::string &, T &)> get_param_func_;
  const std::function<bool(
      const rclcpp::Parameter &)> set_param_func_;
};

}  // namespace kroshu_ros2_core

#endif  // KROSHU_ROS2_CORE__PARAMETER_HPP_
