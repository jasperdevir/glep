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

#ifndef AUDIO_SOURCE_HPP
#define AUDIO_SOURCE_HPP

#include <GLEP/audio/audio_buffer.hpp>
#include <GLEP/audio/audio_effect.hpp>

#include <memory>

#include <AL/al.h>
#include <glm/glm.hpp>

namespace GLEP::Audio{
    
    enum class AudioSourceState{
        INITIAL = 0x1011,
        PLAYING = 0x1012,
        PAUSED = 0x1013,
        STOPPED = 0x1014
    };

    struct EffectSlot{
        std::shared_ptr<AudioEffect> Effect;
        ALuint AlSlot;
    };

    class AudioSource{
        private:
            ALuint _alSource;
            float _pitch = 1.0f;
            float _gain = 1.0f;
            glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec3 _velocity = glm::vec3(0.0f, 0.0f, 0.0f);
            bool _loop = false;
            std::shared_ptr<AudioBuffer> _buffer;
            std::vector<EffectSlot> _effects;

            void initialize();
            
        public:
            AudioSource();
            AudioSource(std::shared_ptr<AudioBuffer> buffer);
            ~AudioSource();

            /// @brief Get the playback pitch.
            /// @return Pitch
            float GetPitch();

            /// @brief Get the playback gain.
            /// @return Gain
            float GetGain();

            /// @brief Get the playback postion (Currently unused).
            /// @return Position
            glm::vec3 GetPosition();

            /// @brief Get the playback velocity (Currently unused).
            /// @return Velocity
            glm::vec3 GetVelocity();

            /// @brief Get if the playback will loop.
            /// @return If the playback will loop
            bool GetLoop();

            /// @brief Get the currently assigned buffer.
            /// @return Currently assigned buffer
            std::shared_ptr<AudioBuffer> GetBuffer();

            /// @brief Get the current state of this source.
            /// @return Current state
            AudioSourceState GetState();


            /// @brief Set the playback pitch.
            /// @param pitch Pitch to set
            void SetPitch(float pitch);

            /// @brief Set the playback gain.
            /// @param gain Gain to set
            void SetGain(float gain);

            /// @brief Set the playback position (Currently unused).
            /// @param position Position to set
            void SetPosition(glm::vec3 position);

            /// @brief SEt the playback velocity (Currently unused).
            /// @param velocity Velocity to set
            void SetVelocity(glm::vec3 velocity);

            /// @brief Set if the playback will loop.
            /// @param loop Loop to set 
            void SetLoop(bool loop);

            /// @brief Set the currently assigned buffer.
            /// @param buffer Buffer to set
            void SetBuffer(const std::shared_ptr<AudioBuffer>& buffer);
            

            /// @brief Add an audio effect.
            /// @param effect Effect to add
            void AddEffect(const std::shared_ptr<AudioEffect>& effect);


            /// @brief Play the currenly assigned buffer.
            void Play();

            /// @brief Play a buffer.
            /// @param buffer Buffer to play
            void Play(const std::shared_ptr<AudioBuffer>& buffer);

            /// @brief Pause the currently assigned buffer at its current duration.
            void Pause();

            /// @brief Stop the currently assigned buffer and reset its current duration.
            void Stop();

    };
}


#endif //AUDIO_SOURCE_HPP