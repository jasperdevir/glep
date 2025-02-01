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

/* GLEP - Example 4: Input */

#include <GLEP/core.hpp>

using namespace GLEP;

const glm::vec2 screenResolution = glm::vec2(1200, 800);


/* -----------------Key Callback Function---------------- */
/*
    <===Useful Input Functions===>

    Input::GetMousePos() - Returns the cursors position in screen space relative to the window
    Input::GetMouseDelta() - Returns the movement of the cursor in between frames

    CursorStates:
        NORMAL - Cursor appears as default
        HIDDEN - Same as normal but the cursor is not visable
        DISABLED - Cursor is locked to window and will move without OS limits, it is not visable
    Input::GetCursorState()
    Input::SetCursorState(Window, NewState)
*/

glm::vec3 moveDirection = glm::vec3(0.0f);
float movementSpeed = 2.0f;

void key_callback(Window* window, KeyCode code, int scancode, InputState state, KeyMod mod){
    if(state == InputState::PRESS){
        if(code == KeyCode::ESCAPE){
            Print(PrintCode::INFO, "INPUT", "CLOSE");
            window->SetShouldClose(true);
        }

        if(code == KeyCode::Q){
            // Flip cursor state
            CursorState cursorState = Input::GetCursorState(window) == CursorState::NORMAL ? CursorState::DISABLED : CursorState::NORMAL;
            if(cursorState == CursorState::NORMAL)
                Print(PrintCode::INFO, "INPUT", "Unlock Cursor");
            else 
                Print(PrintCode::INFO, "INPUT", "Lock Cursor");
            
            
            Input::SetCursorState(window, cursorState);
        }

        if(code == KeyCode::SPACE){

            glm::vec2 mousePos = Input::GetMousePos();
            Print(PrintCode::INFO, "INPUT", "MousePos - X: " + std::to_string(mousePos.x) + ", Y: " + std::to_string(mousePos.y));
        }

        if(code == KeyCode::W || code == KeyCode::UP){
            Print(PrintCode::INFO, "INPUT", "Up");
            moveDirection = glm::vec3(0.0f, 0.0f, -1.0f);
        }
        if(code == KeyCode::S || code == KeyCode::DOWN){
            Print(PrintCode::INFO, "INPUT", "Down");
            moveDirection = glm::vec3(0.0f, 0.0f, 1.0f);
        }
        if(code == KeyCode::A || code == KeyCode::LEFT){
            Print(PrintCode::INFO, "INPUT", "Left");
            moveDirection = glm::vec3(-1.0f, 0.0f, 0.0f);
        }
        if(code == KeyCode::D || code == KeyCode::RIGHT){
            Print(PrintCode::INFO, "INPUT", "Right");
            moveDirection = glm::vec3(1.0f, 0.0f, 0.0f);
        }

        if(mod == KeyMod::SHIFT){
            Print(PrintCode::INFO, "INPUT", "SHIFT MOD");
            moveDirection *= 2.0f;
        }
    }
}
/* ------------------------------------------------------ */

/* ----------------Scroll Callback Function-------------- */
float yPos = 0.0f;

void scroll_callback(Window* window, double scrollX, double scrollY){
    Print(PrintCode::INFO, "INPUT", "ScrollDelta - X: " + std::to_string(scrollX) + ", Y:" + std::to_string(scrollY));
    yPos += (float)scrollY;
    yPos = glm::clamp(yPos, -5.0f, 0.0f);
}
/* ------------------------------------------------------ */

int main(){

    /* -Initialise key objects (Window, Camera & Renderer)- */
    std::shared_ptr<Window> window = std::make_shared<Window>(
        WindowState::WINDOWED, // Window state (Windowed, Windowed Fullscreen, Fullscreen)
        screenResolution, // Window resolution
        "GLEP Example - Core: Input" // Window title
    );

    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(
        45.0f, // FOV (Degrees)
        screenResolution.x / screenResolution.y, // Aspect Ratio
        0.01f, // Near Plane
        100.0f // Far Plane
    );
    camera->Position = glm::vec3(0.0f, 10.0f, 0.0f);
    camera->Rotation = glm::quat(glm::lookAt(camera->Position, glm::vec3(0.0f), -Camera::FRONT));

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(window, camera);
    /* ------------------------------------------------------ */

    /* -------------------------Input------------------------ */
    //Set Input Callback Functions
    Input::SetKeyCallback(renderer->TargetWindow, key_callback);
    Input::SetScrollCallback(renderer->TargetWindow, scroll_callback);

    //Inputs can also be queried outside of the callback functions
    InputState enterState = Input::GetInput(renderer->TargetWindow, KeyCode::ENTER);
    InputState leftMouseState = Input::GetInput(renderer->TargetWindow, MouseCode::LEFT);
    /* ------------------------------------------------------ */

    /* -----Initialize the scene and add an object to it----- */
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    std::shared_ptr<Geometry> geometry = std::make_shared<CubeGeometry>(
        1.0f, // Width
        1.0f, // Height
        1.0f // Depth
    );
    std::shared_ptr<Material> material = std::make_shared<UnlitMaterial>(
        Color::GLEP_GREEN // Diffuse color
    );
    material->Wireframe = true;
    std::shared_ptr<Model> cube = std::make_shared<Model>(
        geometry, 
        material
    );

    scene->Add(cube);
    /* ------------------------------------------------------ */
    
    Print(PrintCode::INFO, "==============Core Example 4: Input==============");
    Print(PrintCode::INFO, "Press W A S D or Arrow Keys to move");
    Print(PrintCode::INFO, "Press SHIFT to increase movement speed");
    Print(PrintCode::INFO, "Press Q to lock/unlock the cursor from the window");
    Print(PrintCode::INFO, "Press SPACE to log the current mouse position");
    Print(PrintCode::INFO, "Scroll up or down to zoom");
    Print(PrintCode::INFO, "=================================================");

    /* ---------------------Render Loop---------------------- */
    while(renderer->IsRunning()){
        Time::Update(); // Update internal clock
        Input::Update(renderer->TargetWindow); // Update user input

        cube->Position += movementSpeed * moveDirection * Time::GetDeltaTimeF();
        cube->Position = glm::clamp(cube->Position, glm::vec3(-2.0f, 0.0f, -2.0f), glm::vec3(2.0f, 0.0f, 2.0f));
        cube->Position.y = yPos;

        renderer->Render(scene); // Render 
        renderer->EndFrame(); // End frame, poll events
    }
    /* ------------------------------------------------------ */

    return 0;
}