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
    GL_MAJ_VERSION = 4;
    GL_MIN_VERSION = 1;

    /* -Initialise key objects (Window, Camera, Renderer & Scene)- */
    std::shared_ptr<Window> window = std::make_shared<Window>(
        WindowState::WINDOWED, // Window state (Windowed, Windowed Fullscreen, Fullscreen)
        screenResolution, // Window resolution
        "GLEP Example - Core: Point Shadows" // Window title
    );

    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(
        45.0f, // FOV (Degrees)
        screenResolution.x / screenResolution.y, // Aspect Ratio
        0.01f, // Near Plane
        100.0f // Far Plane
    );
    camera->Position = glm::vec3(-7.0f, 0.0f, -6.0f);
    

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(window, camera);
    renderer->DebugRenderMode = true;
    renderer->RenderShadows = true;

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    /* ------------------------------------------------------ */

    std::shared_ptr<Geometry> planeGeometry = std::make_shared<PlaneGeometry>(10.0f, 10.0f);
    std::shared_ptr<Geometry> cubeGeometry = std::make_shared<CubeGeometry>(1.0f, 1.0f, 1.0f);
    std::shared_ptr<Texture> interiorTexture = std::make_shared<Texture>(File::GLEP_DEFAULT_TEXTURE);
    /* ---------------Enable shadow renderering-------------- */
    std::shared_ptr<Material> interiorMaterial = std::make_shared<BlinnPhongMaterial>(interiorTexture, Color::WHITE, 16.0f); 
    interiorMaterial->ReceiveShadows = true;

    std::shared_ptr<Model> frontWall = std::make_shared<Model>(planeGeometry, interiorMaterial);
    frontWall->Position.z = -5.0f;
    scene->Add(frontWall);

    std::shared_ptr<Model> backWall = std::make_shared<Model>(planeGeometry, interiorMaterial);
    backWall->Position.z = 5.0f;
    backWall->Rotation = glm::rotate(backWall->Rotation, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    scene->Add(backWall);

    std::shared_ptr<Model> leftWall = std::make_shared<Model>(planeGeometry, interiorMaterial);
    leftWall->Position.x = -5.0f;
    leftWall->Rotation = glm::rotate(leftWall->Rotation, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    scene->Add(leftWall);

    std::shared_ptr<Model> rightWall = std::make_shared<Model>(planeGeometry, interiorMaterial);
    rightWall->Position.x = 5.0f;
    rightWall->Rotation = glm::rotate(rightWall->Rotation, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    scene->Add(rightWall);

    std::shared_ptr<Model> topWall = std::make_shared<Model>(planeGeometry, interiorMaterial);
    topWall->Position.y = -5.0f;
    topWall->Rotation = glm::rotate(topWall->Rotation, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    scene->Add(topWall);

    std::shared_ptr<Model> botWall = std::make_shared<Model>(planeGeometry, interiorMaterial);
    botWall->Position.y = 5.0f;
    botWall->Rotation = glm::rotate(botWall->Rotation, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    scene->Add(botWall);

    std::shared_ptr<Material> castMaterial = std::make_shared<PhongMaterial>(Color::GLEP_GREEN, Color::WHITE, 16.0f);
    castMaterial->CastShadows = true;

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, -2.0f, -2.5f),
        glm::vec3(3.0f, 2.0f, 3.0),
        glm::vec3(-1.0f, -3.0f, 1.0),
        glm::vec3(-2.5f, 1.0f, 1.5),
        glm::vec3(-1.5f, 2.0f, -3.0)
    };

    for(int i = 0; i < 5; i++){
        std::shared_ptr<Model> cube = std::make_shared<Model>(cubeGeometry, castMaterial);
        cube->Position = cubePositions[i];
        cube->Rotation = glm::rotate(cube->Rotation, i * 20.0f, glm::vec3(1.0f, 0.5f, 0.3f));
        scene->Add(cube);
    }
    
    
    /* ----------------Add Lights to the scene--------------- */
    std::shared_ptr<AmbientLight> ambientLight = std::make_shared<AmbientLight>(Color(1.0f), 0.1f);
    scene->Add(ambientLight);

    std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>(
        glm::vec3(0.0f),
        Color::WHITE,
        1.0f,
        1.0f,
        0.09f,
        0.032f
    );
    scene->Add(pointLight);

    /* ------------------------------------------------------ */

    /* ---------------------Render Loop---------------------- */
    while(renderer->IsRunning()){
        Time::Update(); // Update internal clock
        Input::Update(renderer->TargetWindow); // Update user input

        pointLight->Position.x = sin(Time::GetElapsedTimeF() * 0.5f) * 2.0f;
        pointLight->Position.y = cos(Time::GetElapsedTimeF() * 0.5f) * 2.0f;

        camera->Position.x = sin(Time::GetElapsedTimeF() * 0.25f) * 15.0f;
        camera->Position.z = cos(Time::GetElapsedTimeF() * 0.25f) * 15.0f;
        camera->Rotation = glm::lookAt(camera->Position, glm::vec3(0.0f), Camera::UP);

        renderer->Render(scene); // Render 
        renderer->EndFrame(); // End frame, poll events
    }
    /* ------------------------------------------------------ */

    return 0;
}