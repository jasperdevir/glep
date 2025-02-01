/* GLEP - OpenGL Engine Platform
 * Copyright (C) 2025 Jasper Devir <jasperdevir.jd@gmail.com>
 *
 * GLEP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * GLEP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GLEP.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef INTERP_MANAGER_HPP
#define INTERP_MANAGER_HPP

#include <GLEP/control/automation/interp_sequence.hpp>
#include <GLEP/core/time.hpp>

#include <vector>
#include <iostream>

namespace GLEP::Control{

    class InterpManager{
        private:
            static std::vector<std::shared_ptr<TypelessInterpSequence>> queue;

        public:
            /// @brief Add an interpolation clip to the queue.
            /// @tparam T Interpolation sequence type
            /// @param sequence Sequence to add
            template <typename T>
            static void Queue(std::shared_ptr<InterpSequence<T>> sequence){
                if(auto typeless = std::dynamic_pointer_cast<TypelessInterpSequence>(sequence)) 
                    queue.push_back(typeless);                
            }

            /// @brief Create and add an interpolation clip to the queue.
            /// @tparam T Target type
            /// @param value The target value to interpolate
            /// @param startValue The start value
            /// @param endValue The end value
            /// @param duration The duration of the interpolation
            /// @param func The interpolation function
            template <typename T>
            static void Queue(T* value, T startValue, T endValue, float duration, InterpFunc func = InterpFunc::LINEAR){
                std::shared_ptr<InterpClip<T>> clip = std::make_shared<InterpClip<T>>(value, startValue, endValue, duration, func);
                std::shared_ptr<InterpSequence<T>> sequence = std::make_shared<InterpSequence<T>>(clip);
                queue.push_back(std::dynamic_pointer_cast<TypelessInterpSequence>(sequence));
            }

            /// @brief Remove an interpolation sequence from the queue.
            /// @param sequence Sequence to remove
            static void RemoveSequenceFromQueue(std::shared_ptr<TypelessInterpSequence> sequence);
            

            /// @brief Activate all valid queued sequences and update all active sequences (Ideally, to be called every frame).
            static void Update();
    };

}

#endif //INTERP_MANAGER_HPP