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

/* GLEP - Example 2: Geometry */

#include <GLEP/core.hpp>

#include <vector>

using namespace GLEP;

const glm::vec2 screenResolution = glm::vec2(1200, 800);

int main(){

    /* -Initialise key objects (Window, Camera & Renderer)- */
    std::shared_ptr<Window> window = std::make_shared<Window>(
        WindowState::WINDOWED, // Window state (Windowed, Windowed Fullscreen, Fullscreen)
        screenResolution, // Window resolution
        "GLEP Example - Core: Geometry" // Window title
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

    /* -------------------Buffer Geometry------------------ */
    // Vertex - Position(vec3), Normal(vec3), TextureCoord(vec2)

    // Pyramid Buffer Geometry
    std::vector<Vertex> vertices = {
        // Bottom Face Vertices
        {glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f)}, // Vertex 0
        {glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f)}, // Vertex 1
        {glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f)}, // Vertex 2
        {glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f)}, // Vertex 3

        // Apex Vertex
        {glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f), glm::vec3(0.0f)} // Vertex 4
    };

    std::vector<unsigned int> indices = {
        0, 1, 2, // Bottom Face
        0, 2, 3, // Bottom Face

        0, 4, 1, // Back Face
        1, 4, 2, // Right Face
        2, 4, 3, // Front Face
        3, 4, 0 // Left Face
    };

    std::shared_ptr<Geometry> bufferGeometry = std::make_shared<Geometry>(
        vertices, 
        indices
    );

    /* ------------------------------------------------------ */

    /* ------------------Procedual Geometry------------------ */
    std::shared_ptr<Geometry> cubeGeometry = std::make_shared<CubeGeometry>(
        0.5f, // Width
        0.5f, // Height
        0.5f, // Depth
        2,    // Width Segments
        2,    // Height Segments
        2     // Depth Segments
    );

    std::shared_ptr<Geometry> planeGeometry = std::make_shared<PlaneGeometry>(
        0.5f, // Width
        0.5f, // Height
        4,    // Width Segments
        4     // Height Segments
    );

    std::shared_ptr<Geometry> gridGeometry = std::make_shared<GridGeometry>(
        0.5f, // Width
        0.5f, // Height
        6,    // Width Segments
        6     // Height Segments
    );
    /* ------------------------------------------------------ */

    /* ---------Initialize Objects and Add to Scene---------- */
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    std::shared_ptr<Material> material = std::make_shared<UnlitMaterial>(
        Color::GLEP_GREEN
    );
    material->Wireframe = true;
    material->CullFace = MaterialCull::NONE; //Default: MaterialCull::BACK

    std::shared_ptr<Model> buffer = std::make_shared<Model>(bufferGeometry, material);
    buffer->Position.x = -1.5f; //Set object position on the X axis
    buffer->Scale = glm::vec3(0.25f); //Scale the object on each axis
    scene->Add(buffer);

    std::shared_ptr<Model> cube = std::make_shared<Model>(cubeGeometry, material);
    cube->Position.x = -0.5f; //Set object position on the X axis
    scene->Add(cube);

    std::shared_ptr<Model> plane = std::make_shared<Model>(planeGeometry, material);
    plane->Position.x = 0.5f; //Set object position on the X axis
    scene->Add(plane);

    std::shared_ptr<Model> grid = std::make_shared<Model>(gridGeometry, material);
    grid->Position.x = 1.5f; //Set object position on the X axis
    scene->Add(grid);
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
        
        renderer->Render(scene);
        renderer->EndFrame(); // End frame, poll events
    }
    /* ------------------------------------------------------ */

    return 0;
}