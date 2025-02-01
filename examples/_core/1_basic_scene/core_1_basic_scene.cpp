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

/* GLEP - Example 1: Basic Scene */

#include <GLEP/core.hpp>

using namespace GLEP;

const glm::vec2 screenResolution = glm::vec2(800, 800);

int main(){

    /* -Initialise key objects (Window, Camera & Renderer)- */
    std::shared_ptr<Window> window = std::make_shared<Window>(
        WindowState::WINDOWED, // Window state (Windowed, Windowed Fullscreen, Fullscreen)
        screenResolution, // Window resolution
        "GLEP Example - Core: Basic Scene" // Window title
    );

    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(
        45.0f, // FOV (Degrees)
        screenResolution.x / screenResolution.y, // Aspect Ratio
        0.01f, // Near Plane
        100.0f // Far Plane
    );
    camera->Position = glm::vec3(-3.0f, 3.0f, 3.0f);
    camera->Rotation = glm::quat(glm::lookAt(camera->Position, glm::vec3(0.0f), Camera::UP));

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(window, camera);
    /* ------------------------------------------------------ */

    /* -----Initialize the scene and add an object to it----- */
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    std::shared_ptr<Geometry> geometry = std::make_shared<CubeGeometry>(
        1.0f, // Width
        1.0f, // Height
        1.0f // Depth
    );
    std::shared_ptr<Material> material = std::make_shared<UnlitMaterial>(
        Color::GLEP_GREEN// Diffuse color
    );
    material->Wireframe = true;
    std::shared_ptr<Model> cube = std::make_shared<Model>(
        geometry, 
        material
    );

    scene->Add(cube);
    /* ------------------------------------------------------ */

    /* ---------------------Render Loop---------------------- */
    while(renderer->IsRunning()){
        Time::Update(); // Update internal clock
        Input::Update(renderer->TargetWindow); // Update user input

        cube->Rotation = glm::rotate(
            cube->Rotation, 
            Time::GetDeltaTimeF() * 0.5f, 
            glm::vec3(0.0f, 1.0f, 0.0f)
        ); // Rotate cube on the Y axis

        renderer->Render(scene); // Render 
        renderer->EndFrame(); // End frame, poll events
    }
    /* ------------------------------------------------------ */

    return 0;
}