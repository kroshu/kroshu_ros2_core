// Copyright 2022 Komáromi Sándor
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

#ifndef KROSHU_ROS2_CORE__TRANSITION_STEP_HPP_
#define KROSHU_ROS2_CORE__TRANSITION_STEP_HPP_

#include <functional>

namespace kroshu_ros2_core
{
//template<typename T>
class TransitionStep
{
public:
  TransitionStep(
    std::function<bool(/*const T &*/)> in_forward_transition_callback,
    std::function<bool(/*const T &*/)> in_reversed_transition_callback
  )
  : in_forward_transition_callback_(in_forward_transition_callback),
    in_reversed_transition_callback_(in_reversed_transition_callback)
  {}
  ~TransitionStep() = default;

  bool CallForwardCallback(/*const T & param*/) const;
  // {
  //   return in_forward_transition_callback_(/*param*/);
  // }
  bool CallReversedCallback(/*const T & param*/) const;
  // {
  //   return in_reversed_transition_callback_(/*param*/);
  // }

private:
  std::function<bool(/*const T &*/)> in_forward_transition_callback_;
  std::function<bool(/*const T &*/)> in_reversed_transition_callback_;
};
} // namespace kroshu_ros2_core


#endif KROSHU_ROS2_CORE__TRANSITION_STEP_HPP_
