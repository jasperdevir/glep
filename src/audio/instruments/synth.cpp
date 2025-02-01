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

#include <GLEP/audio/instruments/wavetable_synth.hpp>

namespace GLEP::Audio{

    WavetableSynth::WavetableSynth(float sampleRate, WaveType waveType, float baseAmp){
        _baseAmp = baseAmp;
        _octave = 3;

        uint32_t sineWave = AudioEngine::GenerateWave(sampleRate, WaveType::SINE, BASE_FREQ, baseAmp);
        _sine = std::make_shared<AudioBuffer>(sineWave);
        uint32_t sawWave = AudioEngine::GenerateWave(sampleRate, WaveType::SAW, BASE_FREQ, baseAmp);
        _saw = std::make_shared<AudioBuffer>(sawWave);
        uint32_t rectWave = AudioEngine::GenerateWave(sampleRate, WaveType::RECT, BASE_FREQ, baseAmp);
        _rect = std::make_shared<AudioBuffer>(rectWave);

        switch(waveType){
            case WaveType::SINE:
                _currentWaveBuffer = _sine;
                break;

            case WaveType::SAW:
                _currentWaveBuffer = _saw;
                break;

            case WaveType::RECT:
                _currentWaveBuffer = _rect;
                break;
        }

        _source = std::make_shared<AudioSource>(_currentWaveBuffer);
        _source->SetLoop(true);

        SetPitch(NOTE_C0, _octave);
    }

    std::shared_ptr<AudioSource> WavetableSynth::GetSource(){ return _source; }

    void WavetableSynth::SetPitch(float freq){
        _source->SetPitch(freq / BASE_FREQ);
    }

    void WavetableSynth::SetPitch(float freq, int octave){
        _source->SetPitch(freq * (float)pow(2, octave) / BASE_FREQ);
    }

    void WavetableSynth::Start(){
        _source->Play();
    }

    void WavetableSynth::Stop(){
        _source->Stop();
    }

    void WavetableSynth::IncreaseOctave(){
        _octave++;
        if(_octave > MAX_OCTAVE) _octave = MAX_OCTAVE;
    }

    void WavetableSynth::DecreaseOctave(){
        _octave--;
        if(_octave < 1) _octave = 1;
    }

    void WavetableSynth::ProcessInput(KeyCode key, int scancode, InputState state, KeyMod mod){
        if(state == InputState::PRESS){
            if(key == KeyCode::A) SetPitch(NOTE_C0, _octave);      
            if(key == KeyCode::W) SetPitch(NOTE_C_0, _octave);
            if(key == KeyCode::S) SetPitch(NOTE_D0, _octave);
            if(key == KeyCode::E) SetPitch(NOTE_D_0, _octave);
            if(key == KeyCode::D) SetPitch(NOTE_E0, _octave);
            if(key == KeyCode::F) SetPitch(NOTE_F0, _octave);
            if(key == KeyCode::T) SetPitch(NOTE_F_0, _octave);
            if(key == KeyCode::G) SetPitch(NOTE_G0, _octave);
            if(key == KeyCode::Y) SetPitch(NOTE_G_0, _octave);
            if(key == KeyCode::H) SetPitch(NOTE_A0, _octave);
            if(key == KeyCode::U) SetPitch(NOTE_A_0, _octave);
            if(key == KeyCode::J) SetPitch(NOTE_B0, _octave);
            if(key == KeyCode::K) SetPitch(NOTE_C0, _octave + 1);
            if(key == KeyCode::O) SetPitch(NOTE_C_0, _octave + 1);
            if(key == KeyCode::L) SetPitch(NOTE_D0, _octave + 1);

            if(key == KeyCode::Z) DecreaseOctave();
            if(key == KeyCode::X) IncreaseOctave();

            if(key == KeyCode::KEY_1) {
                _source->SetBuffer(_sine);
                _source->Play();
            }
            if(key == KeyCode::KEY_2){
                _source->SetBuffer(_saw);
                _source->Play();
            }
            if(key == KeyCode::KEY_3){
                _source->SetBuffer(_rect);
                _source->Play();
            }
        }
    }
}