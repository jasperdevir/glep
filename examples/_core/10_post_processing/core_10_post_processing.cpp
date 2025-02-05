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

/* GLEP - Example 10: Post-Processing */

#include <GLEP/core.hpp>

using namespace GLEP;

const glm::vec2 screenResolution = glm::vec2(1200, 800);

bool showPostProcessing = true;
bool inputChange = false;

void key_callback(Window* window, KeyCode key, int scancode, InputState state, KeyMod mod){
    if(state == InputState::PRESS){
        if(key == KeyCode::SPACE){
            showPostProcessing = !showPostProcessing;
            inputChange = true;
        }
    }
}

int main(){

    /* -Initialise key objects (Window, Camera & Renderer)- */
    std::shared_ptr<Window> window = std::make_shared<Window>(
        WindowState::WINDOWED, // Window state (Windowed, Windowed Fullscreen, Fullscreen)
        screenResolution, // Window resolution
        "GLEP Example - Core: Post-Processing" // Window title
    );

    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(
        45.0f, // FOV (Degrees)
        screenResolution.x / screenResolution.y, // Aspect Ratio
        0.01f, // Near Plane
        100.0f // Far Plane
    );
    camera->Position = glm::vec3(0.0f, 1.5f, 6.0f);
    camera->Rotation = glm::quat(glm::lookAt(camera->Position, glm::vec3(0.0f), Camera::UP));

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(window, camera);

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    Input::SetKeyCallback(renderer->TargetWindow, key_callback);
    /* ------------------------------------------------------ */

    glm::vec3 positions[10] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    std::shared_ptr<Geometry> geometry = std::make_shared<CubeGeometry>(1.0f, 1.0f, 1.0f);
    std::shared_ptr<Texture> texture = std::make_shared<Texture>(File::GLEP_DEFAULT_TEXTURE);
    std::shared_ptr<Material> material = std::make_shared<UnlitMaterial>(texture);

    for(int i = 0; i < 10; i++){

        std::shared_ptr<Model> model = std::make_shared<Model>(geometry, material);
        model->Position = positions[i];
        model->Rotation = glm::rotate(model->Rotation, i * 20.0f, glm::vec3(1.0f, 0.5f, 0.3f));

        scene->Add(model);
    }

    /* ---Initialize the BufferPassComposer and add passes--- */
    std::shared_ptr<BufferPassComposer> bufferPassComposer = std::make_shared<BufferPassComposer>(
        renderer->TargetWindow->GetResolution() // Resolution
    );

    std::shared_ptr<Material> customPassMaterial = std::make_shared<Material>(
        File::GLEP_SHADERS_PATH / "post" / "defaultPass.vs",
        File::DIRECTORY / "examples" / "res" / "shaders" / "core_10_post_processing.fs"
    );

    std::shared_ptr<BufferPass> customBufferPass = std::make_shared<BufferPass>(
       customPassMaterial // Material
    );
    bufferPassComposer->Add(customBufferPass); // Add a pass to the end of the chain

    std::shared_ptr<KernelPass> blurPass = std::make_shared<KernelPass>(
        KernelPass::BLUR, // Kernel matrix
        1.0f / 600.0f // Sampling offset
    );
    bufferPassComposer->Add(blurPass);

    std::shared_ptr<GrainPass> grainPass = std::make_shared<GrainPass>(
        glm::vec2(10.0f, 10.0f), // UV scale
        0.4f // Intensity
    );
    bufferPassComposer->Add(grainPass);

    scene->PassComposer = bufferPassComposer; // Assign a composer to a scene
    /* ------------------------------------------------------ */

    Print(PrintCode::INFO, "===Core Example 10: Post-Processing===");
    Print(PrintCode::INFO, "Press SPACE to toggle post-processing.");
    Print(PrintCode::INFO, "======================================");

    /* ---------------------Render Loop---------------------- */
    while(renderer->IsRunning()){
        Time::Update(); // Update internal clock
        Input::Update(renderer->TargetWindow); // Update user input

        if(inputChange){
            scene->PassComposer = showPostProcessing ? bufferPassComposer : nullptr;
            std::string state = showPostProcessing ? "ENABLED" : "DISABLED";
            Print(PrintCode::INFO, "Post-Processing is " + state);
            
            inputChange = false;
        }

        // Rotate the camera in a radius around the center
        camera->Position.x = 6.0f * cos(Time::GetElapsedTimeF());
        camera->Position.z = 6.0f * sin(Time::GetElapsedTimeF());
        camera->Rotation = glm::quat(glm::lookAt(camera->Position, glm::vec3(0.0f), Camera::UP));

        renderer->Render(scene); // Render 
        renderer->EndFrame(); // End frame, poll events
    }
    /* ------------------------------------------------------ */

    return 0;
}