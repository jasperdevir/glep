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

/* GLEP Audio - Example 1: Audio Effects */

#include <GLEP/core.hpp>
#include <GLEP/audio.hpp>

using namespace GLEP;
using namespace GLEP::Audio;

const glm::vec2 screenResolution = glm::vec2(800, 800);

bool triggerAudio = false;
bool useEffects = true;

void key_callback(Window* window, KeyCode key, int scancode, InputState state, KeyMod mod){
    if(state == InputState::PRESS){
        if(key == KeyCode::SPACE){
            triggerAudio = true;
        }
        if(key == KeyCode::Q){
            useEffects = !useEffects;
            if(useEffects) Print(PrintCode::INFO, "Audio effects enabled");
            else Print(PrintCode::INFO, "Audio effects disabled");
        }
    }
}

int main(){

    /* -Initialise key objects (Window & Renderer)- */
    std::shared_ptr<Window> window = std::make_shared<Window>(
        WindowState::WINDOWED, // Window state (Windowed, Windowed Fullscreen, Fullscreen)
        screenResolution, // Window resolution
        "GLEP Audio Example - Audio Effects" // Window title
    );

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(window);

    Input::SetKeyCallback(renderer->TargetWindow, key_callback);
    /* ------------------------------------------------------ */

    std::unique_ptr<AudioEngine> audioEngine = std::make_unique<AudioEngine>(
        44100.0f
    );

    std::shared_ptr<AudioBuffer> audioBuffer = std::make_shared<AudioBuffer>(
        File::DIRECTORY / "examples" / "res" / "audio" / "coin.wav"
    );

    std::shared_ptr<AudioSource> dryAudioSource = std::make_shared<AudioSource>(
        audioBuffer
    );
    dryAudioSource->SetGain(0.75f);

    std::shared_ptr<AudioSource> wetAudioSource = std::make_shared<AudioSource>(
        audioBuffer
    );
    wetAudioSource->SetGain(0.75f);

    std::shared_ptr<AudioEffect> reverbEffect = std::make_shared<AudioEffect>(
        AudioEffectType::REVERB //Effect type
    );
    wetAudioSource->AddEffect(reverbEffect);

    reverbEffect->SetValue(
        (int)ReverbParams::DECAY_TIME, //Target parameter
        1000.0f //Value
    );

    Print(PrintCode::INFO, "==Audio Example 0: Playing Files==");
    Print(PrintCode::INFO, "Press SPACE to play the sound file");
    Print(PrintCode::INFO, "Press Q to toggle the effects");
    Print(PrintCode::INFO, "==================================");

    /* ---------------------Render Loop---------------------- */
    while(renderer->IsRunning()){
        Time::Update(); // Update internal clock
        Input::Update(renderer->TargetWindow); // Update user input

        if(triggerAudio){
            if(useEffects){
                Print(PrintCode::INFO, "Audio with effects triggered.");
                wetAudioSource->Play();
            } else {
                Print(PrintCode::INFO, "Audio without effects triggered.");
                dryAudioSource->Play();
            }

            triggerAudio = false;
        }

        renderer->EndFrame(); // End frame, poll events
    }
    /* ------------------------------------------------------ */

    return 0;
}