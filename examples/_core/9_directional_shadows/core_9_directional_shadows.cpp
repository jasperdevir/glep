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

/* GLEP - Example 8: Directional Shadows */

#include <GLEP/core.hpp>

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
    camera->Position = glm::vec3(0.0f, 2.0f, 5.0f);
    camera->Rotation = glm::quat(glm::lookAt(camera->Position, glm::vec3(0.0f), Camera::UP));

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(window, camera);

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    /* ------------------------------------------------------ */

    /* ---------------Enable shadow renderering-------------- */
    renderer->RenderShadows = true;
    renderer->ShadowMapDistance = 2.0f; //Distance from the origin towards the directional light to render
    renderer->GetShadowMapCamera()->SetFarPlane(10.0f); //You can also call the camera used to render the shadow map

    std::shared_ptr<Material> phongMaterial = std::make_shared<PhongMaterial>(
        Color::WHITE, 
        Color::WHITE,
        32.0f
    );
    phongMaterial->CastShadows = true; // Enable this material during shadow map rendering

    std::shared_ptr<Geometry> floorGeometry = std::make_shared<PlaneGeometry>(5.0f, 5.0f);
    std::shared_ptr<Texture> floorTexture = std::make_shared<Texture>(File::GLEP_DEFAULT_TEXTURE);
    std::shared_ptr<Material> floorMaterial = std::make_shared<PhongMaterial>(
        floorTexture,
        Color::WHITE,
        32.0f
    );
    floorMaterial->ReceiveShadows = true; // Enable this material to apply the shadow map result to its lighting
    
    std::shared_ptr<Model> floor = std::make_shared<Model>(floorGeometry, floorMaterial);
    floor->Position.y = -0.5f;
    floor->Rotation = glm::rotate(floor->Rotation, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    scene->Add(floor);

    std::shared_ptr<ImportGeometry> backpackGeometry = std::make_shared<ImportGeometry>(File::DIRECTORY / "examples" / "res" / "models" / "backpack" / "backpack.obj");

    std::shared_ptr<ImportGeometryModel> backpack0 = std::make_shared<ImportGeometryModel>(
        backpackGeometry,
        phongMaterial
    );
    backpack0->CalculateNormals();
    backpack0->Scale = glm::vec3(0.003f);
    scene->Add(backpack0);

    std::shared_ptr<ImportGeometry> bunnyGeometry = std::make_shared<ImportGeometry>(File::DIRECTORY / "examples" / "res" / "models" / "bunny" / "bunny.ply");

    std::shared_ptr<ImportGeometryModel> bunny0 = std::make_shared<ImportGeometryModel>(
        bunnyGeometry,
        phongMaterial
    );
    bunny0->Scale = glm::vec3(6.0f);
    scene->Add(bunny0);
    
    /* ----------------Add Lights to the scene--------------- */
    // Currently a directional light only creates shadows
    std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>(
        glm::vec3(-0.7f, -1.0f, -0.9f),
        Color::WHITE,
        0.5f
    );
    scene->Add(directionalLight);

    std::shared_ptr<AmbientLight> ambientLight = std::make_shared<AmbientLight>(
        Color::WHITE,
        0.4f
    );
    scene->Add(ambientLight);
    /* ------------------------------------------------------ */

    /* ---------------------Render Loop---------------------- */
    while(renderer->IsRunning()){
        Time::Update(); // Update internal clock
        Input::Update(renderer->TargetWindow); // Update user input

        float moveX = sin(Time::GetElapsedTimeF());

        backpack0->Position = glm::vec3(-1.0f * moveX, 1.0f, 0.0f);
        bunny0->Position = glm::vec3(1.0f * moveX, 0.0f, 2.0f);

        renderer->Render(scene); // Render 
        renderer->EndFrame(); // End frame, poll events
    }
    /* ------------------------------------------------------ */

    return 0;
}