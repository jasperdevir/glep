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

/* GLEP - Example 8: Lighting */

#include <GLEP/core.hpp>

using namespace GLEP;

const glm::vec2 screenResolution = glm::vec2(1200, 800);

int main(){

    /* -Initialise key objects (Window, Camera, Renderer & Scene)- */
    std::shared_ptr<Window> window = std::make_shared<Window>(
        WindowState::WINDOWED, // Window state (Windowed, Windowed Fullscreen, Fullscreen)
        screenResolution, // Window resolution
        "GLEP Example - Core: Lighting" // Window title
    );

    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(
        45.0f, // FOV (Degrees)
        screenResolution.x / screenResolution.y, // Aspect Ratio
        0.01f, // Near Plane
        100.0f // Far Plane
    );
    camera->Position = glm::vec3(0.0f, 1.0f, 5.0f);
    camera->Rotation = glm::quat(glm::lookAt(camera->Position, glm::vec3(0.0f), Camera::UP));

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(window, camera);

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    /* ------------------------------------------------------ */

    /* ---------------------Lit Materials-------------------- */
    std::shared_ptr<Material> textureLambertMaterial = std::make_shared<LambertMaterial>(
        nullptr
    );
    std::shared_ptr<Material> texturePhongMaterial = std::make_shared<PhongMaterial>(
        nullptr, 
        32.0f
    );

    std::shared_ptr<Material> lambertMaterial = std::make_shared<LambertMaterial>(
        Color::WHITE
    );
    std::shared_ptr<Material> phongMaterial = std::make_shared<PhongMaterial>(
        Color::WHITE, 
        32.0f
    );
    /* ------------------------------------------------------ */

    std::shared_ptr<Geometry> floorGeometry = std::make_shared<CubeGeometry>(1.0f, 1.0f, 1.0f);
    std::shared_ptr<Texture> floorTexture = std::make_shared<Texture>(File::GLEP_DEFUALT_TEXTURE);
    std::shared_ptr<Material> floorMaterial = std::make_shared<PhongMaterial>(
        floorTexture,
        32.0f
    );
    
    std::shared_ptr<Model> floor = std::make_shared<Model>(floorGeometry, floorMaterial);
    floor->Position.y = -1.0f;
    floor->Scale = glm::vec3(5.0f, 0.5f, 5.0f);
    scene->Add(floor);

    std::shared_ptr<ImportGeometry> backpackGeometry = std::make_shared<ImportGeometry>(File::DIRECTORY / "examples" / "res" / "models" / "backpack" / "backpack.obj");
    std::shared_ptr<ImportModelTexture> backpackTextures = std::make_shared<ImportModelTexture>(File::DIRECTORY / "examples" / "res" / "models" / "backpack" / "backpack.obj");

    std::shared_ptr<ImportGeometryModel> backpack0 = std::make_shared<ImportGeometryModel>(
        backpackGeometry,
        textureLambertMaterial
    );
    backpack0->ApplyImportTextures(backpackTextures);
    backpack0->Scale = glm::vec3(0.003f);
    backpack0->Position = glm::vec3(-1.0f, 0.0f, 1.0f);
    scene->Add(backpack0);

    std::shared_ptr<ImportGeometryModel> backpack1 = std::make_shared<ImportGeometryModel>(
        backpackGeometry,
        texturePhongMaterial
    );
    backpack1->ApplyImportTextures(backpackTextures);
    backpack1->Scale = glm::vec3(0.003f);
    backpack1->Position = glm::vec3(-1.0f, 0.0f, -1.0f);
    scene->Add(backpack1);

    std::shared_ptr<ImportGeometry> bunnyGeometry = std::make_shared<ImportGeometry>(File::DIRECTORY / "examples" / "res" / "models" / "bunny" / "bunny.ply");

    std::shared_ptr<ImportGeometryModel> bunny0 = std::make_shared<ImportGeometryModel>(
        bunnyGeometry,
        lambertMaterial
    );
    bunny0->Scale = glm::vec3(6.0f);
    bunny0->Position = glm::vec3(1.0f, -1.0f, -1.0f);
    scene->Add(bunny0);

    std::shared_ptr<ImportGeometryModel> bunny1 = std::make_shared<ImportGeometryModel>(
        bunnyGeometry,
        phongMaterial
    );
    bunny1->Scale = glm::vec3(6.0f);
    bunny1->Position = glm::vec3(1.0f, -1.0f, 1.0f);
    scene->Add(bunny1);
    
    /* ----------------Add Lights to the scene--------------- */
    std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>(
        glm::vec3(0.0f, 1.0f, 0.0f),
        Color::BLUE,
        1.0f, 
        1.0f,
        0.09f,
        0.032f
    );
    scene->Add(pointLight);

    std::shared_ptr<SpotLight> spotlight = std::make_shared<SpotLight>(
        glm::vec3(0.0f, 1.0f, 2.0f),
        glm::vec3(0.0f, 0.5f, 1.0f),
        Color::RED,
        2.0f,
        15.0f,
        10.0f,
        1.0f, 
        0.09f,
        0.032f
    );
    scene->Add(spotlight);

    std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>(
        glm::vec3(-0.2f, -1.0f, -0.3f),
        Color::GREEN,
        0.2f
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

        pointLight->Position.z = sin(Time::GetDeltaTimeF()) * 5.0f;

        // Rotate the camera in a radius around the center
        camera->Position.x = 5.0f * cos(Time::GetElapsedTimeF());
        camera->Position.z = 5.0f * sin(Time::GetElapsedTimeF());
        camera->Rotation = glm::quat(glm::lookAt(camera->Position, glm::vec3(0.0f), Camera::UP));

        renderer->Render(scene); // Render 
        renderer->EndFrame(); // End frame, poll events
    }
    /* ------------------------------------------------------ */

    return 0;
}