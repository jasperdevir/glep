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

#include <GLEP/audio/audio_effect.hpp>

namespace GLEP::Audio {

	LPALGENEFFECTS AudioEffect::alGenEffects = nullptr;
	LPALDELETEEFFECTS AudioEffect::alDeleteEffects = nullptr;
	LPALISEFFECT AudioEffect::alIsEffect = nullptr;
	LPALEFFECTI AudioEffect::alEffecti = nullptr;
	LPALEFFECTIV AudioEffect::alEffectiv = nullptr;
	LPALEFFECTF AudioEffect::alEffectf = nullptr;
	LPALEFFECTFV AudioEffect::alEffectfv = nullptr;
	LPALGETEFFECTI AudioEffect::alGetEffecti = nullptr;
	LPALGETEFFECTIV AudioEffect::alGetEffectiv = nullptr;
	LPALGETEFFECTF AudioEffect::alGetEffectf = nullptr;
	LPALGETEFFECTFV AudioEffect::alGetEffectfv = nullptr;

	LPALGENAUXILIARYEFFECTSLOTS AudioEffect::alGenAuxiliaryEffectSlots = nullptr;
	LPALDELETEAUXILIARYEFFECTSLOTS AudioEffect::alDeleteAuxiliaryEffectSlots = nullptr;
	LPALISAUXILIARYEFFECTSLOT AudioEffect::alIsAuxiliaryEffectSlot = nullptr;
	LPALAUXILIARYEFFECTSLOTI AudioEffect::alAuxiliaryEffectSloti = nullptr;
	LPALAUXILIARYEFFECTSLOTIV AudioEffect::alAuxiliaryEffectSlotiv = nullptr;
	LPALAUXILIARYEFFECTSLOTF AudioEffect::alAuxiliaryEffectSlotf = nullptr;
	LPALAUXILIARYEFFECTSLOTFV AudioEffect::alAuxiliaryEffectSlotfv = nullptr;
	LPALGETAUXILIARYEFFECTSLOTI AudioEffect::alGetAuxiliaryEffectSloti = nullptr;
	LPALGETAUXILIARYEFFECTSLOTIV AudioEffect::alGetAuxiliaryEffectSlotiv = nullptr;
	LPALGETAUXILIARYEFFECTSLOTF AudioEffect::alGetAuxiliaryEffectSlotf = nullptr;
	LPALGETAUXILIARYEFFECTSLOTFV AudioEffect::alGetAuxiliaryEffectSlotfv = nullptr;

	void AudioEffect::Initialize(ALCdevice* alDevice){
		alGenEffects = (LPALGENEFFECTS)alcGetProcAddress(alDevice, "alGenEffects");
		alDeleteEffects = (LPALDELETEEFFECTS)alcGetProcAddress(alDevice, "alDeleteEffects");
		alIsEffect = (LPALISEFFECT)alcGetProcAddress(alDevice, "alIsEffect");
		alEffecti = (LPALEFFECTI)alcGetProcAddress(alDevice, "alEffecti");
		alEffectiv = (LPALEFFECTIV)alcGetProcAddress(alDevice, "alEffectiv");
		alEffectf = (LPALEFFECTF)alcGetProcAddress(alDevice, "alEffectf");
		alEffectfv = (LPALEFFECTFV)alcGetProcAddress(alDevice, "alEffectfv");
		alGetEffecti = (LPALGETEFFECTI)alcGetProcAddress(alDevice, "alGetEffecti");
		alGetEffectiv = (LPALGETEFFECTIV)alcGetProcAddress(alDevice, "alGetEffectiv");
		alGetEffectf = (LPALGETEFFECTF)alcGetProcAddress(alDevice, "alGetEffectf");
		alGetEffectfv = (LPALGETEFFECTFV)alcGetProcAddress(alDevice, "alGetEffectfv");

		alGenAuxiliaryEffectSlots = (LPALGENAUXILIARYEFFECTSLOTS)alcGetProcAddress(alDevice, "alGenAuxiliaryEffectSlots");
		alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS)alcGetProcAddress(alDevice, "alDeleteAuxiliaryEffectSlots");
		alIsAuxiliaryEffectSlot = (LPALISAUXILIARYEFFECTSLOT)alcGetProcAddress(alDevice, "alIsAuxiliaryEffectSlot");
		alAuxiliaryEffectSloti = (LPALAUXILIARYEFFECTSLOTI)alcGetProcAddress(alDevice, "alAuxiliaryEffectSloti");
		alAuxiliaryEffectSlotiv = (LPALAUXILIARYEFFECTSLOTIV)alcGetProcAddress(alDevice, "alAuxiliaryEffectSlotiv");
		alAuxiliaryEffectSlotf = (LPALAUXILIARYEFFECTSLOTF)alcGetProcAddress(alDevice, "alAuxiliaryEffectSlotf");
		alAuxiliaryEffectSlotfv = (LPALAUXILIARYEFFECTSLOTFV)alcGetProcAddress(alDevice, "alAuxiliaryEffectSlotfv");
		alGetAuxiliaryEffectSloti = (LPALGETAUXILIARYEFFECTSLOTI)alcGetProcAddress(alDevice, "alGetAuxiliaryEffectSloti");
		alGetAuxiliaryEffectSlotiv = (LPALGETAUXILIARYEFFECTSLOTIV)alcGetProcAddress(alDevice, "alGetAuxiliaryEffectSlotiv");
		alGetAuxiliaryEffectSlotf = (LPALGETAUXILIARYEFFECTSLOTF)alcGetProcAddress(alDevice, "alGetAuxiliaryEffectSlotf");
		alGetAuxiliaryEffectSlotfv = (LPALGETAUXILIARYEFFECTSLOTFV)alcGetProcAddress(alDevice, "alGetAuxiliaryEffectSlotfv");
	}

	AudioEffect::AudioEffect(AudioEffectType type){
		_type = type;

		alGenEffects(1, &_effect);
		alEffecti(_effect, AL_EFFECT_TYPE, (ALint)type);
		if (alGetError() != AL_NO_ERROR) {
			Print(PrintCode::ERROR, "AUDIO_EFFECT", "Unable to load audio effect with type: " + std::to_string((int)type));
			return;
		}
	}

	AudioEffect::~AudioEffect() {
		alDeleteEffects(1, &_effect);
	}

	AudioEffectType AudioEffect::GetType(){ return _type; }
	uint32_t AudioEffect::GetID(){ return _effect; }

	void AudioEffect::SetValue(int target, int value){
		if (alIsEffect(_effect)){
			alEffecti(_effect, target, value);
		} 
	}

	void AudioEffect::SetValue(int target, bool value){
		if(alIsEffect(_effect)){
			alEffecti(_effect, target, (int)value);
		}
	}

	void AudioEffect::SetValue(int target, float value){
		if(alIsEffect(_effect)){
			alEffectf(_effect, target, value);
		}
	}
}