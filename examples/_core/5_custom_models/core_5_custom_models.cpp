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

/* GLEP - Example 5: Custom Models */

#include <GLEP/core.hpp>

using namespace GLEP;

const glm::vec2 screenResolution = glm::vec2(1200, 800);

int main(){

    /* -Initialise key objects (Window, Camera & Renderer)- */
    std::shared_ptr<Window> window = std::make_shared<Window>(
        WindowState::WINDOWED, // Window state (Windowed, Windowed Fullscreen, Fullscreen)
        screenResolution, // Window resolution
        "GLEP Example - Core: Custom Models" // Window title
    );

    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(
        45.0f, // FOV (Degrees)
        screenResolution.x / screenResolution.y, // Aspect Ratio
        0.01f, // Near Plane
        100.0f // Far Plane
    );
    camera->Position = glm::vec3(0.0f, 1.5f, 3.0f);
    camera->Rotation = glm::quat(glm::lookAt(camera->Position, glm::vec3(0.0f), Camera::UP));

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(window, camera);
    /* ------------------------------------------------------ */

    /* -------Initialize the scene and add models to it------ */
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    std::shared_ptr<Material> unlitMaterial = std::make_shared<UnlitMaterial>(
        Color::GLEP_GREEN // Diffuse Color
    );
    unlitMaterial->Wireframe = true;

    std::shared_ptr<ImportGeometry> modelGeometry = std::make_shared<ImportGeometry>(File::DIRECTORY / "examples" / "res" / "models" / "backpack" / "backpack.obj");
    std::shared_ptr<ImportGeometryModel> modelA = std::make_shared<ImportGeometryModel>(
        modelGeometry,
        unlitMaterial
    );
    modelA->Scale = glm::vec3(0.0025f);
    modelA->Position.x = -1.0f;
    scene->Add(modelA);

    std::shared_ptr<ImportModel> modelB = std::make_shared<ImportModel>(
        File::DIRECTORY / "examples" / "res" / "models" / "backpack" / "backpack.obj"
    );
    modelB->Scale = glm::vec3(0.0025f);
    modelB->Position.x = 1.0f;
    scene->Add(modelB);
    /* ------------------------------------------------------ */

    /* ---------------------Render Loop---------------------- */
    while(renderer->IsRunning()){
        Time::Update(); // Update internal clock
        Input::Update(renderer->TargetWindow); // Update user input

        for(int i = 0; i < scene->GetObjects().size(); i++){
            auto object = scene->GetObject(i); //Get objects in the scene
            object->Rotation = glm::rotate(
                object->Rotation, 
                Time::GetDeltaTimeF() * 0.5f, 
                glm::vec3(0.0f, 1.0f, 0.0f)
            ); // Rotate object on the Y axis
        }

        renderer->Render(scene); // Render 
        renderer->EndFrame(); // End frame, poll events
    }
    /* ------------------------------------------------------ */

    return 0;
}