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

/* GLEP - Example 3: Materials and Textures */

#include <GLEP/core.hpp>

#include <vector>

using namespace GLEP;

const glm::vec2 screenResolution = glm::vec2(1200, 800);

int main(){

    /* -Initialise key objects (Window, Camera & Renderer)- */
    std::shared_ptr<Window> window = std::make_shared<Window>(
        WindowState::WINDOWED, // Window state (Windowed, Windowed Fullscreen, Fullscreen)
        screenResolution, // Window resolution
        "GLEP Example - Core: Materials and Textures" // Window title
    );

    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(
        45.0f, // FOV (Degrees)
        screenResolution.x / screenResolution.y, // Aspect Ratio
        0.01f, // Near Plane
        100.0f // Far Plane
    );
    camera->Position = glm::vec3(0.0f, 3.0f, 3.0f);
    camera->Rotation = glm::quat(glm::lookAt(camera->Position, glm::vec3(0.0f), Camera::UP));

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(window, camera);
    /* ------------------------------------------------------ */

    /* -----------------------Materials---------------------- */
    std::shared_ptr<Material> customMaterial = std::make_shared<Material>(
        File::GLEP_SHADERS_PATH / "default.vs",                                   // Vertex Shader Path
        File::DIRECTORY / "examples" / "res" / "shaders" / "3_materials_color.fs" // Fragment Shader Path
    );
    customMaterial->AddUniform<Color>(
        "color", // Uniform name within the shader
        Color::WHITE // Value to set
    );

    std::shared_ptr<Material> unlitMaterial = std::make_shared<UnlitMaterial>(
        Color::GLEP_GREEN // Diffuse Color
    );

    std::shared_ptr<Material> uvMaterial = std::make_shared<UVMaterial>();

    std::shared_ptr<Material> depthMaterial = std::make_shared<DepthMaterial>(
        1.0f, // Near Plane
        1000.0f, // Far Plane
        1.0f // Result Scale
    );
    /* ------------------------------------------------------ */

    /* ----------------------Textures------------------------ */
    std::shared_ptr<Texture> defaultTexture = std::make_shared<Texture>(
        File::GLEP_DEFUALT_TEXTURE, // Image file path
        TextureType::DIFFUSE // Texture Type (Diffuse, Specular, Normal, Displacement)
    );
    std::shared_ptr<Texture> containerATexture = std::make_shared<Texture>(
        File::DIRECTORY / "examples" / "res" / "textures" / "wood.png", // Image File Path
        TextureType::DIFFUSE // Texture Type (Diffuse, Specular, Normal, Displacement)
    );
    std::shared_ptr<Texture> containerBTexture = std::make_shared<Texture>(
        File::DIRECTORY / "examples" / "res" / "textures" / "containerB.png", // Image File Path
        TextureType::DIFFUSE // Texture Type (Diffuse, Specular, Normal, Displacement)
    );
    /* ------------------------------------------------------ */

    /* -----------------Texture Materials-------------------- */
    std::shared_ptr<Material> customTextureMaterial = std::make_shared<Material>(
        File::GLEP_SHADERS_PATH / "default.vs",                                     // Vertex Shader Path
        File::DIRECTORY / "examples" / "res" / "shaders" / "3_materials_texture.fs" // Fragment Shader Path
    );
    customTextureMaterial->AddUniform<std::shared_ptr<Texture>>("texture", defaultTexture);
    customTextureMaterial->AddUniform<Color>("tint", Color::WHITE);

    std::shared_ptr<Material> defaultTextureMaterial = std::make_shared<UnlitMaterial>(
        defaultTexture // Diffuse Texture
    );

    std::shared_ptr<Material> containerAMaterial = std::make_shared<UnlitMaterial>(
        containerATexture // Diffuse Texture
    );

    std::shared_ptr<Material> containerBMaterial = std::make_shared<UnlitMaterial>(
        containerBTexture // Diffuse Texture
    );
    /* ------------------------------------------------------ */

    std::shared_ptr<Material> materials[2][4];
    materials[0][0] = customMaterial;
    materials[0][1] = unlitMaterial;
    materials[0][2] = uvMaterial;
    materials[0][3] = depthMaterial;
    materials[1][0] = customTextureMaterial;
    materials[1][1] = defaultTextureMaterial;
    materials[1][2] = containerAMaterial;
    materials[1][3] = containerBMaterial;

    /* ---------Initialize Objects and Add to Scene---------- */
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    std::shared_ptr<Geometry> geometry = std::make_shared<CubeGeometry>(0.5f, 0.5f, 0.5f);

    for(int z = 0; z < 2; z++){
        for(int x = 0; x < 4; x++){
            std::shared_ptr<Model> model = std::make_shared<Model>(geometry, materials[z][x]);
            float xPos = x - 1.5f;
            float zPos = z - 0.5f;
            model->Position = glm::vec3(xPos, 0.0f, zPos);
            scene->Add(model);
        }
    }
        
    /* ------------------------------------------------------ */

    /* ---------------------Render Loop---------------------- */
    while(renderer->IsRunning()){
        Time::Update(); // Update internal clock
        Input::Update(renderer->TargetWindow); // Update user input

        for(int i = 0; i < scene->GetObjects().size(); i++){
            auto object = scene->GetObject(i); //Get object in the scene
            object->Rotation = glm::rotate(
                object->Rotation, 
                Time::GetDeltaTimeF() * 0.5f, 
                glm::vec3(0.0f, 1.0f, 0.0f)
            ); // Rotate object on the Y axis
        }
        
        renderer->Render(scene); // Render will set each uniform added to a material and set a MVP uniform as well as a GLEPInfo uniform
        renderer->EndFrame(); // End frame, poll events
    }
    /* ------------------------------------------------------ */

    return 0;
}