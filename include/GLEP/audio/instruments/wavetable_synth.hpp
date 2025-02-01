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

#ifndef SYNTH_HPP
#define SYNTH_HPP

#include <GLEP/audio/audio_engine.hpp>
#include <GLEP/audio/audio_source.hpp>
#include <GLEP/core/input.hpp>

namespace GLEP::Audio{

    #define NOTE_C0 16.35f
    #define NOTE_C_0 17.32f
    #define NOTE_D0 18.35f
    #define NOTE_D_0 19.45f
    #define NOTE_E0 20.60f
    #define NOTE_F0 21.83f
    #define NOTE_F_0 23.12f
    #define NOTE_G0 24.50f
    #define NOTE_G_0 25.96f
    #define NOTE_A0 27.50f
    #define NOTE_A_0 29.14f
    #define NOTE_B0 30.87f

    class WavetableSynth{
        private:
            const int MAX_OCTAVE = 7;
            const float BASE_FREQ = 440.0f;

            float _baseAmp;
            int _octave;

            std::shared_ptr<AudioBuffer> _sine;
            std::shared_ptr<AudioBuffer> _saw;
            std::shared_ptr<AudioBuffer> _rect;

            std::shared_ptr<AudioBuffer> _currentWaveBuffer;
            std::shared_ptr<AudioSource> _source;

        public:
            WavetableSynth(float sampleRate, WaveType waveType, float baseAmp = 1.0f);

            /// @brief Get the source used to playback the synthesised audio.
            /// @return Audio source
            std::shared_ptr<AudioSource> GetSource();


            /// @brief Set the pitch of the synthesised note.
            /// @param freq Note frequency
            void SetPitch(float freq);

            /// @brief Set the pitch of the synthesised note multiplied by an octave.
            /// @param baseFreq Base note frequency
            /// @param octave Octave multiplier
            void SetPitch(float baseFreq, int octave);


            /// @brief Start synthesising audio.
            void Start();

            /// @brief Stop synthesising audio.
            void Stop();

            /// @brief Increase the current octave by 1.
            void IncreaseOctave();

            /// @brief Decrease the current octave by 1.
            void DecreaseOctave();


            /// @brief Process user input through the input callback.
            /// @param key The key that has changed state
            /// @param scancode The scan code of the key
            /// @param state The current state of the key
            /// @param mod Key modifier currently enabled
            void ProcessInput(KeyCode key, int scancode, InputState state, KeyMod mod);
    };
    
}


#endif //SYNTH_HPP