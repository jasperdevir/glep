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

#ifndef INTERP_SEQUENCE_HPP
#define INTERP_SEQUENCE_HPP

#include <GLEP/control/automation/interp_clip.hpp>

#include <memory>
#include <vector>

namespace GLEP::Control{
    
    class TypelessInterpSequence{
        protected:
            float _firstUpdateTime = -1.0f;

            bool _loop;
            float _startTime;
            int _currentIndex = 0;
        
        public:
            TypelessInterpSequence(float startTime, bool loop);

            /// @brief Get if the sequence is set to loop.
            /// @return If the sequence will loop
            bool GetLoop();

            /// @brief Get the start time.
            /// @return Start time
            float GetStartTime();


            /// @brief Update the current interpolation clip in the sequence.
            /// @param time Total elapsed time
            /// @param deltaTime Delta time
            /// @return If the sequence is complete
            virtual bool Update(float time, float deltaTime) = 0;
    };

    template <typename T>
    class InterpSequence : public TypelessInterpSequence {
        private:
            std::vector<std::shared_ptr<InterpClip<T>>> _clips;

        public:
            InterpSequence(std::shared_ptr<InterpSequence<T>> sequence)
            : TypelessInterpSequence(sequence->GetStartTime(), sequence->GetLoop()){
                _clips.insert(_clips.end(), sequence->GetClips().begin(), sequence->GetClips.end());
            }
            InterpSequence(float startTime = 0.0f, bool loop = false) : TypelessInterpSequence(startTime, loop) {}
            InterpSequence(std::shared_ptr<InterpClip<T>> clip, float startTime = 0.0f, bool loop = false)
            : TypelessInterpSequence(startTime, loop){
                _clips.push_back(clip);
            }
            InterpSequence(std::vector<std::shared_ptr<InterpClip<T>>> clips, float startTime = 0.0f, bool loop = false)
            : TypelessInterpSequence(startTime, loop){
                _clips = clips;
            }

            /// @brief Get the queued intepolation clips.
            /// @return Queued interpolation clips
            std::vector<std::shared_ptr<InterpClip<T>>>& GetClips(){
                return _clips;
            }


            /// @brief Add an interpolation clip to the end of the sequence.
            /// @param clip Clip to add
            void Add(std::shared_ptr<InterpClip<T>> clip){
                _clips.push_back(clip);
            }

            /// @brief Remove an interpolation clip from the sequence.
            /// @param clip Clip to remove
            void Remove(std::shared_ptr<InterpClip<T>> clip){
                _clips.erase(std::remove(_clips.begin(), _clips.end(), clip), _clips.end());
            }


            /// @brief Check if the sequence is complete.
            /// @return If the sequence is complete
            bool IsComplete(){
                return _currentIndex >= _clips.size();
            }

            /// @brief Update the current interpolation clip in the sequence.
            /// @param time Total elapsed time
            /// @param deltaTime Delta time
            /// @return If the sequence is complete
            bool Update(float time, float deltaTime){
                if(_firstUpdateTime < 0.0f){
                    _firstUpdateTime = time;
                }

                if(_startTime > time - _firstUpdateTime){
                    return false;
                }
                    

                if (_clips.empty()) {
                    return true;
                }

                if (_currentIndex >= _clips.size()) {
                    if (_loop) {
                        _currentIndex = 0;
                    } else {
                        return true;
                    }
                }

                if (_clips[_currentIndex]->Update(deltaTime)) {
                    _currentIndex++;
                }

                return false;
            }

    };
}

#endif //INTERP_SEQUENCE_HPP