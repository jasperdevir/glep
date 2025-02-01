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

#ifndef AUDIO_ENGINE_HPP
#define AUDIO_ENGINE_HPP

#include <GLEP/core/utility/math.hpp>
#include <GLEP/core/utility/print.hpp>

#include <GLEP/audio/audio_buffer.hpp>
#include <GLEP/audio/audio_effect.hpp>

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"
#include "AL/efx.h"

namespace GLEP::Audio{

    enum class WaveType{
        SINE,
        SAW,
        RECT
    };

    class AudioEngine{
        private:
            float _sampleRate;
            ALCdevice* _alDevice;
            ALCcontext* _alContext;

            void initialize();

        public:
            AudioEngine(float sampleRate = 44100.0f);
            ~AudioEngine();

            /// @brief Get the sample rate.
            /// @return Sample rate
            float GetSampleRate();

            /// @brief Synthesise a wave buffer based on parameters.
            /// @param sampleRate Sample rate of the buffer
            /// @param type Wave type to generate
            /// @param frequency Frequency of the wave
            /// @param amplitude Amplitude of the wave
            /// @return Synthesised buffer ID
            static uint32_t GenerateWave(float sampleRate, WaveType type, float frequency, float amplitude = 1.0f);
                        
    };
}

#endif //AUDIO_ENGINE_HPP