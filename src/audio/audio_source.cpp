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

#include <GLEP/audio/audio_source.hpp>

namespace GLEP::Audio{
    AudioSource::AudioSource(){
        initialize();
    }

    AudioSource::AudioSource(std::shared_ptr<AudioBuffer> buffer){
        _buffer = buffer;
        initialize();
    }

    void AudioSource::initialize(){
        alGenSources(1, &_alSource);
        alSourcef(_alSource, AL_PITCH, _pitch);
        alSourcef(_alSource, AL_GAIN, _gain);
        alSource3f(_alSource, AL_POSITION, _position.x, _position.y, _position.z);
        alSource3f(_alSource, AL_VELOCITY, _velocity.x, _velocity.y, _velocity.z);
        alSourcei(_alSource, AL_LOOPING, _loop);

        uint32_t bufferData = 0;
        if(_buffer) bufferData = _buffer->GetID();
        alSourcei(_alSource, AL_BUFFER, bufferData);
    }

    AudioSource::~AudioSource(){
        alDeleteSources(1, &_alSource);
        for(EffectSlot slot : _effects){
            AudioEffect::alDeleteAuxiliaryEffectSlots(1, &slot.AlSlot);
        }
    }

    float AudioSource::GetPitch(){ return _pitch; }
    float AudioSource::GetGain(){ return _gain; }
    glm::vec3 AudioSource::GetPosition(){ return _position; }
    glm::vec3 AudioSource::GetVelocity(){ return _velocity; }
    bool AudioSource::GetLoop(){ return _loop; }
    std::shared_ptr<AudioBuffer> AudioSource::GetBuffer(){ return _buffer; }

    void AudioSource::SetPitch(float pitch){
        _pitch = pitch;
        alSourcef(_alSource, AL_PITCH, _pitch);
    }

    void AudioSource::SetGain(float gain){
        _gain = gain;
        alSourcef(_alSource, AL_GAIN, _gain);
    }

    void AudioSource::SetPosition(glm::vec3 position){
        _position = position;
        alSource3f(_alSource, AL_POSITION, _position.x, _position.y, _position.z);
    }

    void AudioSource::SetVelocity(glm::vec3 velocity){
        _velocity = velocity;
        alSource3f(_alSource, AL_VELOCITY, _velocity.x, _velocity.y, _velocity.z);
    }

    void AudioSource::SetLoop(bool loop){
        _loop = loop;
        alSourcei(_alSource, AL_LOOPING, _loop);
    }

    void AudioSource::SetBuffer(const std::shared_ptr<AudioBuffer>& buffer){
        if(GetState() != AudioSourceState::STOPPED) 
            Stop();

        _buffer = buffer;
        alSourcei(_alSource, AL_BUFFER, _buffer->GetID());
    }

    void AudioSource::AddEffect(const std::shared_ptr<AudioEffect>& effect){
        EffectSlot slot;
        slot.Effect = effect;
        
        ALuint alSlot;
        AudioEffect::alGenAuxiliaryEffectSlots(1, &alSlot);
        if (alGetError() != AL_NO_ERROR) {
            Print(PrintCode::ERROR, "AUDIO_SOURCE", "Unable to generate auxiliary effects slot");
            return;
        }
        slot.AlSlot = alSlot;


        AudioEffect::alAuxiliaryEffectSloti(slot.AlSlot, AL_EFFECTSLOT_EFFECT, slot.Effect->GetID());
        if (alGetError() != AL_NO_ERROR) {
            Print(PrintCode::ERROR, "AUDIO_SOURCE", "Unable to attach effect to auxiliary effects slot");
            return;
        }

        alSource3i(_alSource, AL_AUXILIARY_SEND_FILTER, slot.AlSlot, 0, AL_FILTER_NULL);
        _effects.push_back(slot);
    }

    void AudioSource::Play(){
        if(!_buffer) 
            return;
        
        alSourcePlay(_alSource);
    }

    void AudioSource::Play(const std::shared_ptr<AudioBuffer>& buffer){
        SetBuffer(buffer);
        alSourcePlay(_alSource);
    }

    AudioSourceState AudioSource::GetState(){
        ALint state;
        alGetSourcei(_alSource, AL_SOURCE_STATE, &state); 
        if (alGetError() != AL_NO_ERROR) {
            Print(PrintCode::ERROR, "AUDIO_SOURCE", "Failed to query source state.");
            return AudioSourceState::INITIAL;
        }
        return (AudioSourceState)state;
    }

    void AudioSource::Pause(){
        if(!_buffer) 
            return;

        alSourcePause(_alSource);
    }

    void AudioSource::Stop(){
        if(!_buffer) 
            return;
        
        alSourceStop(_alSource);
    }
}