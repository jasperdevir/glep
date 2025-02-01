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

/* GLEP Control - Example 3: Automation */

#include <GLEP/core.hpp>
#include <GLEP/control/automation.hpp>

#include <vector>

using namespace GLEP;
using namespace GLEP::Control;

const glm::vec2 screenResolution = glm::vec2(1200, 800);

int main(){

    /* -Initialise key objects (Window, Camera & Renderer)- */
    std::shared_ptr<Window> window = std::make_shared<Window>(
        WindowState::WINDOWED, // Window state (Windowed, Windowed Fullscreen, Fullscreen)
        screenResolution, // Window resolution
        "GLEP Example - Control: Automation" // Window title
    );

    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(
        45.0f, // FOV (Degrees)
        screenResolution.x / screenResolution.y, // Aspect Ratio
        0.01f, // Near Plane
        100.0f // Far Plane
    );
    camera->Position = glm::vec3(4.0f, 4.0f, 4.0f);
    camera->Rotation = glm::quat(glm::lookAt(camera->Position, glm::vec3(0.0f), Camera::UP));

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(window, camera);
    /* ------------------------------------------------------ */

    
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    std::shared_ptr<Geometry> geometry = std::make_shared<CubeGeometry>(0.5f, 0.5f, 0.5f);

    std::shared_ptr<Texture> texture = std::make_shared<Texture>(File::GLEP_DEFUALT_TEXTURE, TextureType::DIFFUSE);
    std::shared_ptr<Material> material = std::make_shared<UnlitMaterial>(texture);

    for(int z = 0; z < 4; z++){
        for(int x = 0; x < 4; x++){
            std::shared_ptr<Model> model = std::make_shared<Model>(geometry, material);
            float xPos = x - 1.5f;
            float zPos = z - 1.5f;
            model->Position = glm::vec3(xPos, 0.0f, zPos);
            scene->Add(model);

            std::shared_ptr<InterpClip<glm::vec3>> clipA = std::make_shared<InterpClip<glm::vec3>>(
                &model->Position, // Target
                model->Position, // Start value
                glm::vec3(xPos, 1.5f, zPos), // End value
                2.5f, // Duration
                InterpFunc::EASE_IN_SINE //Interpolation function
            );
            std::shared_ptr<InterpClip<glm::vec3>> clipB = std::make_shared<InterpClip<glm::vec3>>(
                &model->Position,
                glm::vec3(xPos, 1.5f, zPos),
                model->Position,
                2.5f,
                InterpFunc::EASE_OUT_SINE
            );

            float startTime = x * 0.85f;
            std::shared_ptr<InterpSequence<glm::vec3>> sequence = std::make_shared<InterpSequence<glm::vec3>>(
                startTime, // Sequence start time
                true // Loop
            );
            sequence->Add(clipA);
            sequence->Add(clipB);

            InterpManager::Queue(sequence);
        }
    }
        
    /* ---------------------Render Loop---------------------- */
    while(renderer->IsRunning()){
        Time::Update(); // Update internal clock
        Input::Update(renderer->TargetWindow); // Update user input

        InterpManager::Update(); // Update each frame
        
        renderer->Render(scene); // Render will set each uniform added to a material and set a MVP uniform as well as a GLEPInfo uniform
        renderer->EndFrame(); // End frame, poll events
    }
    /* ------------------------------------------------------ */

    return 0;
}