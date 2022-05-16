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

#ifndef KROSHU_ROS2_CORE__PARAMETERHANDLER_HPP_
#define KROSHU_ROS2_CORE__PARAMETERHANDLER_HPP_

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "rclcpp/node_interfaces/node_parameters_interface.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
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

class ParameterBase
{
public:
  ParameterBase(
    const std::string & name, const ParameterSetAccessRights & rights,
    rclcpp::node_interfaces::NodeParametersInterface::SharedPtr param_IF)
  : name_(name), rights_(rights), paramIF_(param_IF)
  {
  }

  virtual ~ParameterBase() = default;

  const std::string & getName() const
  {
    return name_;
  }

  const ParameterSetAccessRights & getRights() const
  {
    return rights_;
  }

  const rclcpp::ParameterValue & getDefaultValue() const
  {
    return default_value_;
  }

  rclcpp::node_interfaces::NodeParametersInterface::SharedPtr getParameterInterface() const
  {
    return paramIF_;
  }

  virtual bool callCallback(const rclcpp::Parameter &) const {return false;}

protected:
  void setDefaultValue(rclcpp::ParameterValue && value)
  {
    default_value_ = value;
  }
  const std::string name_;

private:
  const ParameterSetAccessRights rights_;
  rclcpp::node_interfaces::NodeParametersInterface::SharedPtr paramIF_;
  rclcpp::ParameterValue default_value_;
};

template<typename T>
class Parameter : public ParameterBase
{
public:
  Parameter(
    const std::string & name, const T & value, const ParameterSetAccessRights & rights,
    std::function<bool(const T &)> on_change_callback,
    std::shared_ptr<rclcpp::node_interfaces::NodeParametersInterface> paramIF)
  : ParameterBase(name, rights, paramIF), on_change_callback_(on_change_callback)
  {
    setDefaultValue(rclcpp::ParameterValue(value));
  }

  ~Parameter() override = default;

  bool getValue(
    T & place_holder) const
  {
    return paramIF_->get_parameter(name_, place_holder);
  }

  bool callCallback(const rclcpp::Parameter & new_param) const override
  {
    try {
      return on_change_callback_(new_param.get_value<T>());
    } catch (const rclcpp::exceptions::InvalidParameterTypeException & e) {
      printf("%s", e.what());
      return false;
    }
  }

private:
  const std::function<bool(const T &)> on_change_callback_;
};

class ParameterHandler
{
public:
  explicit ParameterHandler(rclcpp_lifecycle::LifecycleNode * node = nullptr);

  rcl_interfaces::msg::SetParametersResult onParamChange(
    const std::vector<rclcpp::Parameter> & parameters) const;
  bool canSetParameter(const ParameterBase & param) const;
  void registerParameter(std::shared_ptr<ParameterBase> param_shared_ptr);

private:
  std::vector<std::shared_ptr<ParameterBase>> params_;
  rclcpp_lifecycle::LifecycleNode * node_;
};
}  // namespace kroshu_ros2_core


#endif  // KROSHU_ROS2_CORE__PARAMETERHANDLER_HPP_
