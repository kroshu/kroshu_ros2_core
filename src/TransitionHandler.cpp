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

#include "TransitionHandler.hpp"

#include <boost/range/adaptor/reversed.hpp>

namespace kroshu_ros2_core
{
TransitionHandler::TransitionHandler()
{
}

bool TransitionHandler::MakeTransition()
{
  // Check if there is any transition step registerd
  if (!transition_steps_.empty()) {

    // Loop throught the transition steps
    for (auto && step : transition_steps_) {

      // Checks the transition if the step is already done returns with warning
      if (step.second == true) {
        //TODO (Komaromi): (write warning) already or partly done transition
        return true;
      }

      // Call and check the return value on the forward callback
      if (step.first.CallForwardCallback()) {
        // Register if the step was successful
        step.second = true;
      } else {
        // Call the Reverse transition funcion to reverse every step and return the function
        ReverseTransition();
        return false;
      }
    }

  } else {
    //TODO (Komaromi): (Write warning) transition steps empty create log message
  }
  return true;
}

bool TransitionHandler::ReverseTransition()
{
  if (!transition_steps_.empty()) {

    for (auto && step : boost::adaptors::reverse(transition_steps_)) {
      if (step.second == true) {

        if (step.first.CallReversedCallback()) {
          step.second = false;
        } else {
          throw "error";
        }
      }
    }
  } else {
    //TODO (Komaromi): (Write warning) transition steps empty create log message
  }
  return true;
}

bool TransitionHandler::RegisterTransitionStep(
  std::function<bool()> in_forward_transition_callback,
  std::function<bool()> in_reversed_transition_callback)
{
  transition_steps_.emplace_back(
    std::make_pair<kroshu_ros2_core::TransitionStep, bool>(
      kroshu_ros2_core::TransitionStep(
        in_forward_transition_callback,
        in_reversed_transition_callback), false));
}

} // namespace kroshu_ros2_core
