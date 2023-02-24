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
#ifndef KROSHU_ROS2_CORE__TRANSITION_HANDLER_HPP_
#define KROSHU_ROS2_CORE__TRANSITION_HANDLER_HPP_

#include "TransitionStep.hpp"

#include <vector>

namespace kroshu_ros2_core
{
class TransitionHandler
{
private:
  /*
  *  The vector that contains the registrated steps, and the bool value that contains if the step i already done
  */
  std::vector<std::pair<kroshu_ros2_core::TransitionStep, bool>> transition_steps_;

public:
  TransitionHandler(/* args */);
  ~TransitionHandler();

  bool MakeTransition();
  bool ReverseTransition();
  bool RegisterTransitionStep();
};


} // namespace kroshu_ros2_core


#endif //KROSHU_ROS2_CORE__TRANSITION_HANDLER_HPP_
