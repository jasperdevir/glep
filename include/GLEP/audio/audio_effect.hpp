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

#ifndef AUDIO_EFFECT_HPP
#define AUDIO_EFFECT_HPP

#include <GLEP/core/utility/print.hpp>

#include <string>
#include <vector>

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"
#include "AL/efx.h"

namespace GLEP::Audio{

    enum class AudioEffectType{
        REVERB = 1,
        CHORUS,
        DISTORTION,
        ECHO,
        FLANGER,
        FREQUENCY_SHIFTER,
        VOCAL_MORPHER,
        PITCH_SHIFTER,
        RING_MODULATOR,
        AUTOWAH,
        COMPRESSOR,
        EQUALIZER
    };

    enum class ReverbParams{
        DENSITY = 1,
        DIFFUSION,
        GAIN,
        GAIN_HF,
        DECAY_TIME,
        DECAY_HF_RATIO,
        REFLECTIONS_GAIN,
        REFLECTIONS_DELAY,
        LATE_REVERB_GAIN,
        LATE_REVERB_DELAY,
        AIR_ABSORPTION_GAIN_HF,
        ROOM_ROLLOFF_FACTOR,
        DECAY_HF_LIMIT
    };

    enum class ChorusParams{
        WAVEFORM = 1,
        PHASE,
        RATE,
        DEPTH,
        FEEDBACK,
        DELAY
    };

    enum class DistortionParams{
        EDGE = 1,
        GAIN,
        LOWPASS_CUTOFF,
        EQ_CENTER,
        EQ_BANDWIDTH
    };

    enum class FlangerParams{
        WAVEFORM = 1,
        PHASE,
        RATE,
        DEPTH,
        FEEDBACK,
        DELAY
    };

    enum class FrequencyShifterParams{
        FREQUENCY = 1,
        LEFT_DIRECTION,
        RIGHT_DIRECTION
    };

    enum class VocalMorpherParams{
        PHONEMEA = 1,
        PHONEMEA_COARSE_TUNING,
        PHONEMEB,
        PHONEMEB_COARSE_TUNING,
        WAVEFORM,
        RATE
    };

    enum class PitchShifterParams{
        COARSE_TUNE = 1,
        FINE_TUNE,
    };

    enum class RingModulatorParams{
        FREQUENCY = 1,
        HIGHPASS_CUTOFF,
        WAVEFORM
    };

    enum class AutowahParams{
        ATTACK_TIME = 1,
        RELEASE_TIME,
        RESONANCE,
        PEAK_GAIN
    };

    enum class CompressorParams{
        ON_OFF = 1
    };

    enum class EqualizerParams{
        LOW_GAIN = 1,
        LOW_CUTOFF,
        MID1_GAIN,
        MID1_CENTER,
        MID1_WIDTH,
        MID2_GAIN,
        MID2_CENTER,
        MID2_WIDTH,
        HIGH_GAIN,
        HIGH_CUTOFF
    };

    class AudioEffect{
        protected:
            AudioEffectType _type;
            uint32_t _effect;

        public:

            AudioEffect(AudioEffectType type);
            ~AudioEffect();

            /// @brief Get the type of audio effect.
            /// @return Audio effect type
            AudioEffectType GetType();

            /// @brief Get the effect buffer ID.
            /// @return Buffer ID
            uint32_t GetID();


            /// @brief Set the value of an effect parameter.
            /// @param target Target parameter
            /// @param value Value to set
            void SetValue(int target, int value);

            /// @brief Set the value of an effect parameter.
            /// @param target Target parameter
            /// @param value Value to set
            void SetValue(int target, bool value);

            /// @brief Set the value of an effect parameter.
            /// @param target Target parameter
            /// @param value Value to set
            void SetValue(int target, float value);


            /// @brief Initialize all audio effect interfacing functions.
            /// @param alDevice AL context device
            static void Initialize(ALCdevice* alDevice);

            /* Effect object functions */
            static LPALGENEFFECTS alGenEffects;
            static LPALDELETEEFFECTS alDeleteEffects;
            static LPALISEFFECT alIsEffect;
            static LPALEFFECTI alEffecti;
            static LPALEFFECTIV alEffectiv;
            static LPALEFFECTF alEffectf;
            static LPALEFFECTFV alEffectfv;
            static LPALGETEFFECTI alGetEffecti;
            static LPALGETEFFECTIV alGetEffectiv;
            static LPALGETEFFECTF alGetEffectf;
            static LPALGETEFFECTFV alGetEffectfv;

            /* Auxiliary Effect Slot object functions */
            static LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots;
            static LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots;
            static LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot;
            static LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti;
            static LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv;
            static LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf;
            static LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv;
            static LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti;
            static LPALGETAUXILIARYEFFECTSLOTIV alGetAuxiliaryEffectSlotiv;
            static LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf;
            static LPALGETAUXILIARYEFFECTSLOTFV alGetAuxiliaryEffectSlotfv;

    };
}

#endif //AUDIO_EFFECT_HPP