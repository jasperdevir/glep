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

#include <GLEP/audio/audio_engine.hpp>

namespace GLEP::Audio{

    AudioEngine::AudioEngine(float sampleRate){
        _sampleRate = sampleRate;

        initialize();
    }

    AudioEngine::~AudioEngine(){        
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(_alContext);
        alcCloseDevice(_alDevice); 
        _alDevice = nullptr;
        _alContext = nullptr;

        Print(PrintCode::INFO, "AUDIO_ENGINE", "Audio Engine successfully shutdown");
    }

    void AudioEngine::initialize(){
        _alDevice = alcOpenDevice(nullptr);
        if (!_alDevice || alcGetError(_alDevice) != AL_NO_ERROR) {
            Print(PrintCode::CRITICAL, "AUDIO_ENGINE", "Failed to open audio device.");
            return;
        }

        _alContext = alcCreateContext(_alDevice, nullptr);
        if (!_alContext) {
            Print(PrintCode::CRITICAL, "AUDIO_ENGINE", "Failed to create OpenAL context.");
            return;
        }
        
        if(!alcMakeContextCurrent(_alContext)){
            Print(PrintCode::CRITICAL, "AUDIO_ENGINE", "Failed to make OpenAL context current.");
            return;
        }

        const ALCchar* name = nullptr;
        if(alcIsExtensionPresent(_alDevice, "ALC_ENUMERATE_ALL_EXT")){
            name = alcGetString(_alDevice, ALC_ALL_DEVICES_SPECIFIER);
        }
        if(!name || alcGetError(_alDevice) != AL_NO_ERROR){
            name = alcGetString(_alDevice, ALC_DEVICE_SPECIFIER);
        }
        
        Print(PrintCode::INFO, "AUDIO_ENGINE", "Audio Engine successfully initialized");
        Print(PrintCode::INFO, "AUDIO_ENGINE", "Opened " + std::string(name));

        AudioEffect::Initialize(_alDevice);
    }

    float AudioEngine::GetSampleRate(){ return _sampleRate; }

    uint32_t AudioEngine::GenerateWave(float sampleRate, WaveType type, float frequency, float amplitude) {
        std::vector<short> membuf;

        size_t totalSamples = static_cast<size_t>(sampleRate);
        membuf.resize(totalSamples);

        float period = 1 / frequency;

        for (size_t i = 0; i < totalSamples; ++i) {
            float time = i / sampleRate;
            float phase = fmodf(time, period); 

            float sample;
            switch(type){
                case WaveType::SINE:
                    sample = (float)sin(2 * (float)Math::PI * frequency * time);
                    break;
                
                case WaveType::SAW:
                    sample = (phase / period) * 2.0f - 1.0f;
                    break;

                case WaveType::RECT:
                    sample = (phase < period / 2.0f) ? -1.0f : 1.0f;
                    break;
            }

            sample *= amplitude;
            
            // Convert to 16-bit PCM format (int16_t)
            membuf[i] = static_cast<short>(sample * 32767); // 16-bit PCM format range is -32768 to 32767
        }

        uint32_t buffer = 0;
        alGenBuffers(1, &buffer);
        alBufferData(buffer, AL_FORMAT_MONO16, membuf.data(), (ALsizei)totalSamples * sizeof(short), (ALsizei)sampleRate);
        
        if(alGetError() != AL_NO_ERROR){
            Print(PrintCode::ERROR, "AUDIO_ENGINE", "Failed to create wave buffer.");
            if(buffer && alIsBuffer(buffer)) alDeleteBuffers(1, &buffer);
            return 0;
        }

        return buffer;
    }

}