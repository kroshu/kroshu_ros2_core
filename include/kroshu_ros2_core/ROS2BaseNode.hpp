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

namespace kroshu_ros2_core
{

class ROS2BaseNode : public rclcpp_lifecycle::LifecycleNode
{
public:
  explicit ROS2BaseNode(const std::string & node_name);
  explicit ROS2BaseNode(const std::string & node_name, const rclcpp::NodeOptions & options);
  ~ROS2BaseNode() override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_configure(const rclcpp_lifecycle::State &) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_cleanup(const rclcpp_lifecycle::State &) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_shutdown(const rclcpp_lifecycle::State & state) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_activate(const rclcpp_lifecycle::State &) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_deactivate(const rclcpp_lifecycle::State &) override;

  rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_error(const rclcpp_lifecycle::State &) override;

protected:
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
      const std::string & name, const ParameterSetAccessRights & rights)
    : name_(name), rights_(rights)
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

    virtual bool callCallback(const rclcpp::Parameter &) {return false;}

protected:
    const std::string name_;
    rclcpp::ParameterValue default_value_;

private:
    const ParameterSetAccessRights rights_;
  };

  template<typename T>
  class Parameter : public ParameterBase
  {
public:
    Parameter(
      const std::string & name, const T & value, const ParameterSetAccessRights & rights,
      std::function<bool(rclcpp::Parameter)> on_change_callback,
      ROS2BaseNode & node)
    : ParameterBase(name, rights), on_change_callback_(on_change_callback),
      node_(node)
    {
    	default_value_ = rclcpp::ParameterValue(value);
    }

    ~Parameter() override = default;

    bool getValue(
      T & place_holder) const
    {
      return node_.get_parameter(name_, place_holder);
    }

    bool setValue(const T & new_value) const
    {
      rclcpp::Parameter new_param(name_, new_value);
      return node_.set_parameter(new_param).successful;
    }

    bool callCallback(const rclcpp::Parameter & new_param) override
    {
      return on_change_callback_(new_param);
    }

private:
    const std::function<bool(rclcpp::Parameter)> on_change_callback_;
    ROS2BaseNode & node_;
  };

  rcl_interfaces::msg::SetParametersResult onParamChange(
    const std::vector<rclcpp::Parameter> & parameters);
  bool canSetParameter(const ParameterBase & param);
  void registerParameter(std::shared_ptr<ParameterBase> param_shared_ptr);

  static const rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn SUCCESS =
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
  static const rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn ERROR =
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::ERROR;
  static const rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn FAILURE =
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::FAILURE;

private:
  std::vector<std::shared_ptr<ParameterBase>> params_;
};

}  // namespace kroshu_ros2_core

#endif  // KROSHU_ROS2_CORE__ROS2BASENODE_HPP_
