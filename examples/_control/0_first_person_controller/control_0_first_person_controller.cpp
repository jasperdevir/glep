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

/* GLEP Control - Example 0: First Person Controller */

#include <GLEP/core.hpp>
#include <GLEP/control/camera/first_person.hpp>

using namespace GLEP;
using namespace GLEP::Control;

const glm::vec2 screenResolution = glm::vec2(1200, 800);

int main(){

    std::shared_ptr<Window> window = std::make_shared<Window>(
        WindowState::WINDOWED, // Window state (Windowed, Windowed Fullscreen, Fullscreen)
        screenResolution, // Window resolution
        "GLEP Example - Core: Baked Cube Maps" // Window title
    );

    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(
        45.0f, // FOV (Degrees)
        screenResolution.x / screenResolution.y, // Aspect Ratio
        0.01f, // Near Plane
        100.0f // Far Plane
    );
    camera->Position = glm::vec3(0.0f, 0.0f, 3.0f);
    camera->Rotation = glm::quat(glm::lookAt(camera->Position, glm::vec3(0.0f), Camera::UP));

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(window, camera);

    /* ----------------First Person Controller--------------- */
    std::shared_ptr<FirstPersonController> firstPersonController = std::make_shared<FirstPersonController>(
        camera, //Target camera
        2.0f, // Move speed
        0.1f, // Look speed
        false, // Invert X Axis
        false // Invert Y Axis
    );
    /* ------------------------------------------------------ */

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    std::shared_ptr<Geometry> cubeGeometry = std::make_shared<CubeGeometry>(1.0f, 1.0f, 1.0f);

    std::shared_ptr<TextureCubeMap> skyboxCubeMap = std::make_shared<TextureCubeMap>(
        File::GLEP_DEFAULT_CUBE_MAP // .cubemap File path
    );

    std::shared_ptr<Material> skyboxMaterial = std::make_shared<SkyboxMaterial>(
        skyboxCubeMap // CubeMap
    );

    scene->Skybox = std::make_shared<Mesh>(cubeGeometry, skyboxMaterial); 

    std::shared_ptr<Texture> cubeTexture = std::make_shared<Texture>(File::GLEP_DEFAULT_TEXTURE);
    std::shared_ptr<Material> cubeMaterial = std::make_shared<UnlitMaterial>(cubeTexture);

    std::shared_ptr<Model> cube0 = std::make_shared<Model>(cubeGeometry, cubeMaterial);
    cube0->Position.x = -1.5f;
    cube0->Scale = glm::vec3(0.5f);
    scene->Add(cube0);
    std::shared_ptr<Model> cube1 = std::make_shared<Model>(cubeGeometry, cubeMaterial);
    cube1->Position.x = 1.5f;
    cube1->Scale = glm::vec3(0.5f);
    scene->Add(cube1);
    std::shared_ptr<Model> cube2 = std::make_shared<Model>(cubeGeometry, cubeMaterial);
    cube2->Position.z = -1.5f;
    cube2->Scale = glm::vec3(0.5f);
    scene->Add(cube2);
    std::shared_ptr<Model> cube3 = std::make_shared<Model>(cubeGeometry, cubeMaterial);
    cube3->Position.z = 1.5f;
    cube3->Scale = glm::vec3(0.5f);
    scene->Add(cube3);
    std::shared_ptr<Model> cube4 = std::make_shared<Model>(cubeGeometry, cubeMaterial);
    cube4->Position.y = -1.0f;
    cube4->Scale = glm::vec3(5.0f, 0.5f, 5.0f);
    scene->Add(cube4);

    std::shared_ptr<Material> reflectionMaterial = std::make_shared<ReflectionMaterial>(
        nullptr, // CubeMap
        Color(1.0f) // Tint
    );
    reflectionMaterial->BakeRequired = true;

    std::shared_ptr<Material> refractionMaterial = std::make_shared<RefractionMaterial>(
        nullptr, // CubeMap
        1.5f, // Refractive Index
        Color(1.0f) // Tint
    );
    reflectionMaterial->BakeRequired = true;

    std::shared_ptr<ImportGeometry> bunnyGeometry = std::make_shared<ImportGeometry>(File::DIRECTORY / "examples" / "res" / "models" / "bunny" / "bunny.ply");

    std::shared_ptr<ImportGeometryModel> reflectionModel = std::make_shared<ImportGeometryModel>(
        bunnyGeometry,
        reflectionMaterial
    );
    reflectionModel->Position = glm::vec3(-0.5f, -0.5f, 0.0f);
    reflectionModel->Scale = glm::vec3(5.0f);
    scene->Add(reflectionModel);

    std::shared_ptr<ImportGeometryModel> refractionModel = std::make_shared<ImportGeometryModel>(
        bunnyGeometry,
        refractionMaterial
    );
    refractionModel->Position = glm::vec3(0.5f, -0.5f, 0.0f);
    refractionModel->Scale = glm::vec3(5.0f);
    scene->Add(refractionModel);

    std::shared_ptr<BakedCubeMap> bakedA = std::make_shared<BakedCubeMap>(
        reflectionModel->Position,
        1024
    );
    std::shared_ptr<BakedCubeMap> bakedB = std::make_shared<BakedCubeMap>(
        refractionModel->Position,
        1024
    );
    scene->Add(bakedA);
    scene->Add(bakedB);

    renderer->Bake(scene);
    /* ------------------------------------------------------ */


    /* ---------------------Render Loop---------------------- */
    while(renderer->IsRunning()){
        Time::Update(); // Update internal clock
        Input::Update(renderer->TargetWindow); // Update user input

        firstPersonController->Update(renderer->TargetWindow); //Update each frame

        renderer->Render(scene); // Render 
        renderer->EndFrame(); // End frame, poll events
    }
    /* ------------------------------------------------------ */

    return 0;
}