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
#include <functional>
#include <utility>

#include "rclcpp/rclcpp.hpp"
#include "kroshu_ros2_core/Parameter.hpp"

namespace kroshu_ros2_core
{
Parameter::Parameter(
  const std::string & name, const rclcpp::ParameterValue & value,
  const rclcpp::ParameterType & type, const ParameterSetAccessRights & rights,
  const std::function<bool(const kroshu_ros2_core::Parameter &)> & on_change_callback)
: name_(name), value_(value), type_(type), rights_(rights),
  on_change_callback_(on_change_callback)
{
}

const std::string & Parameter::getName() const {return name_;}
const rclcpp::ParameterValue & Parameter::getValue() const {return value_;}
const rclcpp::ParameterType Parameter::getType() const {return type_;}
const ParameterSetAccessRights & Parameter::getRights() const {return rights_;}
const std::function<bool(const kroshu_ros2_core::Parameter &)> & Parameter::getCallback() const
{
  return on_change_callback_;
}
bool Parameter::setValue(const rclcpp::ParameterValue & other_value)
{
  value_ = other_value;
}

}  // namespace kroshu_ros2_core
