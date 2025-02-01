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

/* GLEP Audio - Example 0: Playing Files */

#include <GLEP/core.hpp>
#include <GLEP/audio.hpp>

using namespace GLEP;
using namespace GLEP::Audio;

const glm::vec2 screenResolution = glm::vec2(800, 800);

bool triggerAudio = false;

void key_callback(Window* window, KeyCode key, int scancode, InputState state, KeyMod mod){
    if(state == InputState::PRESS){
        if(key == KeyCode::SPACE){
            triggerAudio = true;
        }
    }
}

int main(){

    /* -Initialise key objects (Window & Renderer)- */
    std::shared_ptr<Window> window = std::make_shared<Window>(
        WindowState::WINDOWED, // Window state (Windowed, Windowed Fullscreen, Fullscreen)
        screenResolution, // Window resolution
        "GLEP Audio Example - Playing Files" // Window title
    );

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(window);

    Input::SetKeyCallback(renderer->TargetWindow, key_callback);
    /* ------------------------------------------------------ */

    std::unique_ptr<AudioEngine> audioEngine = std::make_unique<AudioEngine>(
        44100.0f //Sample rate
    ); // The audio engine must be initialized first before any other audio based classes.

    std::shared_ptr<AudioBuffer> audioBuffer = std::make_shared<AudioBuffer>(
        File::DIRECTORY / "examples" / "res" / "audio" / "coin.wav" //Audio file path
    );

    std::shared_ptr<AudioSource> audioSource = std::make_shared<AudioSource>(
        audioBuffer //Active buffer
    );

    audioSource->SetGain(1.5f);

    Print(PrintCode::INFO, "==Audio Example 0: Playing Files==");
    Print(PrintCode::INFO, "Press SPACE to play the sound file");
    Print(PrintCode::INFO, "==================================");

    /* ---------------------Render Loop---------------------- */
    while(renderer->IsRunning()){
        Time::Update(); // Update internal clock
        Input::Update(renderer->TargetWindow); // Update user input

        if(triggerAudio){
            Print(PrintCode::INFO, "Audio triggered.");
            audioSource->Play();
            
            triggerAudio = false;
        }

        renderer->EndFrame(); // End frame, poll events
    }
    /* ------------------------------------------------------ */

    return 0;
}