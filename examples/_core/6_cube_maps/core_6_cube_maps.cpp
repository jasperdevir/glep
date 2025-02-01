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

/* GLEP - Example 6: Cube Maps */

#include <GLEP/core.hpp>

using namespace GLEP;

const glm::vec2 screenResolution = glm::vec2(1200, 800);

int main(){

    /* -Initialise key objects (Window, Camera, Renderer & Scene)- */
    std::shared_ptr<Window> window = std::make_shared<Window>(
        WindowState::WINDOWED, // Window state (Windowed, Windowed Fullscreen, Fullscreen)
        screenResolution, // Window resolution
        "GLEP Example - Core: Cube Maps" // Window title
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

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    /* ------------------------------------------------------ */

    std::shared_ptr<Geometry> cubeGeometry = std::make_shared<CubeGeometry>(1.0f, 1.0f, 1.0f);

    /* ---------Add a Cube Map skybox to the scene----------- */
    std::shared_ptr<TextureCubeMap> skyboxCubeMap = std::make_shared<TextureCubeMap>(
        File::GLEP_DEFAULT_CUBE_MAP // .cubemap File path
    );

    /*
    This format is also valid, just ensure the it follows the correct order of directions
    std::vector<std::filesystem::path> cubeMapPaths = {
        File::TEXTURE / "cubemap_right.jpg",  //RIGHT
        File::TEXTURE / "cubemap_left.jpg",   //LEFT
        File::TEXTURE / "cubemap_top.jpg",    //TOP
        File::TEXTURE / "cubemap_bottom.jpg", //BOTTOM
        File::TEXTURE / "cubemap_front.jpg",  //FRONT
        File::TEXTURE / "cubemap_back.jpg"    //BACK
    };

    std::shared_ptr<TextureCubeMap> skyboxCubeMap = std::make_shared<TextureCubeMap>(
        cubeMapPaths // std::vector of 6 image paths
    );
    
    */

    std::shared_ptr<Material> skyboxMaterial = std::make_shared<SkyboxMaterial>(
        skyboxCubeMap // CubeMap
    );

    //This mesh will be drawn behind all other objects
    scene->Skybox = std::make_shared<Mesh>(cubeGeometry, skyboxMaterial); 
    /* ------------------------------------------------------ */

    /* ------Use a Cube Map for reflections/refractions------ */
    std::shared_ptr<Material> reflectionMaterial = std::make_shared<ReflectionMaterial>(
        skyboxCubeMap, // CubeMap
        Color(1.0f) // Tint
    );

    std::shared_ptr<Material> refractionMaterial = std::make_shared<RefractionMaterial>(
        skyboxCubeMap, // CubeMap
        1.5f, // Refractive Index
        Color(1.0f) // Tint
    );

    std::shared_ptr<ImportGeometry> modelGeometry = std::make_shared<ImportGeometry>(File::DIRECTORY / "examples" / "res" / "models" / "bunny" / "bunny.ply");

    std::shared_ptr<ImportGeometryModel> reflectionModel = std::make_shared<ImportGeometryModel>(
        modelGeometry,
        reflectionMaterial
    );
    reflectionModel->Position = glm::vec3(-0.5f, -0.5f, 0.0f);
    reflectionModel->Scale = glm::vec3(5.0f);
    scene->Add(reflectionModel);

    std::shared_ptr<ImportGeometryModel> refractionModel = std::make_shared<ImportGeometryModel>(
        modelGeometry,
        refractionMaterial
    );
    refractionModel->Position = glm::vec3(0.5f, -0.5f, 0.0f);
    refractionModel->Scale = glm::vec3(5.0f);
    scene->Add(refractionModel);
    /* ------------------------------------------------------ */

    /* ---------------------Render Loop---------------------- */
    while(renderer->IsRunning()){
        Time::Update(); // Update internal clock
        Input::Update(renderer->TargetWindow); // Update user input

        // Rotate the camera in a radius around the center
        camera->Position.x = 3.0f * cos(Time::GetElapsedTimeF());
        camera->Position.z = 3.0f * sin(Time::GetElapsedTimeF());
        camera->Rotation = glm::quat(glm::lookAt(camera->Position, glm::vec3(0.0f), Camera::UP));

        renderer->Render(scene); // Render 
        renderer->EndFrame(); // End frame, poll events
    }
    /* ------------------------------------------------------ */

    return 0;
}