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

#ifndef INTERP_CLIP_HPP
#define INTERP_CLIP_HPP

#include <GLEP/core/utility/math.hpp>
#include <GLEP/core/time.hpp>

#include <memory>

namespace GLEP::Control{

    template <typename T>
    class InterpClip{
        private:
            float _duration;
            bool _isPlaying = false;
            float _startTime = 0.0f;
            float _currentTime = 0.0f;
            InterpFunc _func;

            T* _value;
            T _startValue;
            T _endValue;
        
        public:
            InterpClip(T* value, const std::shared_ptr<InterpClip<T>>& clip){
                _value = value;
                _startValue = clip->GetStartValue();
                _endValue = clip->GetEndValue();
                _duration = clip->GetDuration();
                _func = clip->GetInterpFunc();
            }

            InterpClip(T* value, T startValue, T endValue, float duration, InterpFunc func = InterpFunc::LINEAR){
                _value = value;
                _startValue = startValue;
                _endValue = endValue;
                _duration = duration;
                _func = func;
            }

            /// @brief Get the start value.
            /// @return Start value
            T GetStartValue(){
                return _startValue;
            }

            /// @brief Get the end value. 
            /// @return End value
            T GetEndValue(){
                return _endValue;
            }

            /// @brief Get the total duration.
            /// @return Total duration
            float GetDuration(){
                return _duration;
            }

            /// @brief Get the interpolation function.
            /// @return Interpolation function
            InterpFunc GetInterpFunc(){
                return _func;
            }


            /// @brief Reset the target to the start value and reset its current duration.
            void Reset(){
                *_value = _startValue;
                _currentTime = 0.0f;
            }

            /// @brief Check if the clip has reached the end of its duration.
            /// @return If the clip is complete
            bool IsComplete(){
                return _currentTime >= _duration;
            }

            /// @brief Update the interpolation based on the delta time.
            /// @param delta Delta time
            /// @return If the clip is complete
            bool Update(float delta){
                if(!_value) return false;

                if(!_isPlaying){
                    Reset();
                    _isPlaying = true;
                }

                _currentTime += delta;

                if(IsComplete()){
                    *_value = _endValue;
                    _isPlaying = false;
                    return true;
                } 

                float t = _currentTime / _duration;

                *_value = Math::Interp(_func, _startValue, _endValue, t);
                return false;
            }
    };

    template <typename T>
    InterpClip(T,T,T) -> InterpClip<T>;

}

#endif //INTERP_CLIP_HPP