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

/* GLEP - Example 8: Point Shadows */

#include <GLEP/core.hpp>
#include <GLEP/control/camera/first_person.hpp>

using namespace GLEP;

const glm::vec2 screenResolution = glm::vec2(1200, 800);

int main(){

    /* -Initialise key objects (Window, Camera, Renderer & Scene)- */
    std::shared_ptr<Window> window = std::make_shared<Window>(
        WindowState::WINDOWED, // Window state (Windowed, Windowed Fullscreen, Fullscreen)
        screenResolution, // Window resolution
        "GLEP Example - Core: Directional Shadows" // Window title
    );

    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(
        45.0f, // FOV (Degrees)
        screenResolution.x / screenResolution.y, // Aspect Ratio
        0.01f, // Near Plane
        100.0f // Far Plane
    );
    camera->Position = glm::vec3(0.0f, 0.0f, 0.0f);

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(window, camera);

    std::shared_ptr<Control::FirstPersonController> controls = std::make_shared<Control::FirstPersonController>(camera, 1.0f, 0.1f);

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    /* ------------------------------------------------------ */

    std::shared_ptr<Geometry> cubeGeometry = std::make_shared<CubeGeometry>(5.0f, 5.0f, 5.0f);
    std::shared_ptr<Texture> interiorTexture = std::make_shared<Texture>(File::GLEP_DEFAULT_TEXTURE);
    /* ---------------Enable shadow renderering-------------- */
    std::shared_ptr<Material> interiorMaterial = std::make_shared<LambertMaterial>(interiorTexture); 
    interiorMaterial->CullFace = MaterialCull::FRONT;
    interiorMaterial->ReceiveShadows = true;

    std::shared_ptr<Model> interior = std::make_shared<Model>(cubeGeometry, interiorMaterial);
    scene->Add(interior);
    
    /* ----------------Add Lights to the scene--------------- */
    std::shared_ptr<AmbientLight> ambientLight = std::make_shared<AmbientLight>(Color(1.0f), 0.2f);
    scene->Add(ambientLight);

    /* ------------------------------------------------------ */

    /* ---------------------Render Loop---------------------- */
    while(renderer->IsRunning()){
        Time::Update(); // Update internal clock
        Input::Update(renderer->TargetWindow); // Update user input

        float moveX = sin(Time::GetElapsedTimeF());

        controls->Update(renderer->TargetWindow);

        renderer->Render(scene); // Render 
        renderer->EndFrame(); // End frame, poll events
    }
    /* ------------------------------------------------------ */

    return 0;
}