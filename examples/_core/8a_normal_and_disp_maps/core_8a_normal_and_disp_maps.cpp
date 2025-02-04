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

/* GLEP - Example 8a: Normal and Displacement Maps */

#include <GLEP/core.hpp>

using namespace GLEP;

const glm::vec2 screenResolution = glm::vec2(1200, 800);

int main(){

    std::shared_ptr<Window> window = std::make_shared<Window>(
        WindowState::WINDOWED, // Window state (Windowed, Windowed Fullscreen, Fullscreen)
        screenResolution, // Window resolution
        "GLEP Example - Core: Normal and Displacement Maps" // Window title
    );

    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(
        60.0f, // FOV (Degrees)
        screenResolution.x / screenResolution.y, // Aspect Ratio
        0.01f, // Near Plane
        100.0f // Far Plane
    );
    camera->Position = glm::vec3(0.0f, 0.0f, 3.0f);

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(window, camera);

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    /* --------------------Load Textures--------------------- */
    std::shared_ptr<Texture> diffuseTex = std::make_shared<Texture>(
        File::DIRECTORY / "examples" / "res" / "textures" / "bricks2.jpg" // Image file path
    );
    std::shared_ptr<Texture> normalTex = std::make_shared<Texture>(
        File::DIRECTORY / "examples" / "res" / "textures" / "bricks2_normal.jpg", // Image file path
        TextureType::NORMAL // Texture type
    );
    std::shared_ptr<Texture> dispTex = std::make_shared<Texture>(
        File::DIRECTORY / "examples" / "res" / "textures" / "bricks2_disp.jpg", // Image file path
        TextureType::DISP // Texture type
    );
    /* ------------------------------------------------------ */

    std::shared_ptr<Geometry> cubeGeometry = std::make_shared<CubeGeometry>(0.75f, 0.75f, 0.75f);

    std::shared_ptr<BlinnPhongMaterial> diffuseMat = std::make_shared<BlinnPhongMaterial>(
        diffuseTex, 
        Color(1.0f), 
        16.0f
    );

    /* --------------Assign Normal Map Texture--------------- */
    std::shared_ptr<BlinnPhongMaterial> normalMat = std::make_shared<BlinnPhongMaterial>(
        diffuseTex, 
        Color(1.0f), 
        16.0f
    );

    //Add the normal texture as a uniform, automatically enabling the (bool) hasNormalTex uniform.
    normalMat->AddUniform<std::shared_ptr<Texture>>(
        "normalTex", // This uniform name is required for normal maps
        normalTex // Normal map texture
    );
    /* ------------------------------------------------------ */

    /* -----------Assign Displacement Map Texture------------ */
    std::shared_ptr<BlinnPhongMaterial> dispMat = std::make_shared<BlinnPhongMaterial>(
        diffuseTex, 
        Color(1.0f), 
        16.0f
    );
    dispMat->AddUniform<std::shared_ptr<Texture>>(
        "normalTex", 
        normalTex
    );

    //Add the depth texture as a uniform, automatically enabling the (bool) hasDispTex uniform and adding (float) dispScale.
    dispMat->AddUniform<std::shared_ptr<Texture>>(
        "dispTex", // This uniform name is required for displacement maps
        dispTex // Displacement map texture
    );
    //Specify the depth of the displacement (Default value is 0.1f)
    dispMat->SetUniformValue<float>(
        "dispScale",
        0.1f // Scale to set
    );
    /* ------------------------------------------------------ */

    std::shared_ptr<Model> diffuseCube = std::make_shared<Model>(cubeGeometry, diffuseMat);
    diffuseCube->Position.x = -1.5f;
    scene->Add(diffuseCube);
    std::shared_ptr<Model> normalCube = std::make_shared<Model>(cubeGeometry, normalMat);
    scene->Add(normalCube);
    std::shared_ptr<Model> dispCube = std::make_shared<Model>(cubeGeometry, dispMat);
    dispCube->Position.x = 1.5f;
    scene->Add(dispCube);

    std::shared_ptr<AmbientLight> ambientLight = std::make_shared<AmbientLight>(Color(1.0f), 0.2f);
    scene->Add(ambientLight);
    std::shared_ptr<PointLight> pointLight0 = std::make_shared<PointLight>(glm::vec3(-1.0f, 1.0f, 2.0f), Color(1.0f), 0.5f, 1.0f, 0.09f, 0.032f);
    scene->Add(pointLight0);
    std::shared_ptr<PointLight> pointLight1 = std::make_shared<PointLight>(glm::vec3(1.0f, 1.0f, 2.0f), Color(1.0f), 0.5f, 1.0f, 0.09f, 0.032f);
    scene->Add(pointLight1);    
    
    /* ---------------------Render Loop---------------------- */
    while(renderer->IsRunning()){
        Time::Update(); // Update internal clock
        Input::Update(renderer->TargetWindow); // Update user input

        diffuseCube->Rotation = glm::rotate(diffuseCube->Rotation, Time::GetDeltaTimeF() * 0.5f, glm::vec3(0.3f, 0.5f, 1.0f));
        normalCube->Rotation = glm::rotate(normalCube->Rotation, Time::GetDeltaTimeF() * 0.5f, glm::vec3(0.3f, 0.5f, 1.0f));
        dispCube->Rotation = glm::rotate(dispCube->Rotation, Time::GetDeltaTimeF() * 0.5f, glm::vec3(0.3f, 0.5f, 1.0f));

        renderer->Render(scene); // Render 
        renderer->EndFrame(); // End frame, poll events
    }
    /* ------------------------------------------------------ */

    return 0;
}