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

#include <GLEP/core/input.hpp>

namespace GLEP {

    bool Input::firstFrame = true;
    double Input::lastMouseX = 0.0;
    double Input::lastMouseY = 0.0;
    double Input::mouseX = 0.0;
    double Input::mouseY = 0.0;
    double Input::mouseDeltaX = 0.0;
    double Input::mouseDeltaY = 0.0;
    KeyFunc Input::keyCallback;
    ScrollFunc Input::scrollCallback;

    InputState Input::GetInput(const std::shared_ptr<Window>& window, KeyCode key){
        return (InputState)glfwGetKey(window->GetGLFWWindow(), (int)key);
    }

    InputState Input::GetInput(const std::shared_ptr<Window>& window, MouseCode mouse){
        return (InputState)glfwGetMouseButton(window->GetGLFWWindow(), (int)mouse);
    }

    CursorState Input::GetCursorState(const std::shared_ptr<Window>& window){
        return (CursorState)glfwGetInputMode(window->GetGLFWWindow(), GLFW_CURSOR);
    }

    CursorState Input::GetCursorState(Window* window){
        return (CursorState)glfwGetInputMode(window->GetGLFWWindow(), GLFW_CURSOR);
    }

    void Input::SetKeyCallback(const std::shared_ptr<Window>& window, KeyFunc callback){
        auto key_callback = [](GLFWwindow* glfwWindow, int key, int scancode, int state, int mods) {
            Window* glepWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
            keyCallback(glepWindow, (KeyCode)key, scancode, (InputState)state, (KeyMod)mods);
        };

        keyCallback = callback;
        glfwSetKeyCallback(window->GetGLFWWindow(), key_callback);
    }

    void Input::SetScrollCallback(const std::shared_ptr<Window>& window, ScrollFunc callback){
        auto key_callback = [](GLFWwindow* glfwWindow, double xoffset, double yoffset) {
            Window* glepWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
            scrollCallback(glepWindow, xoffset, yoffset);
        };

        scrollCallback = callback;
        glfwSetScrollCallback(window->GetGLFWWindow(), key_callback);
    }

    void Input::SetCursorState(const std::shared_ptr<Window>& window, CursorState state){
        glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, (int)state);
    }

    void Input::SetCursorState(Window* window, CursorState state){
        glfwSetInputMode(window->GetGLFWWindow(), GLFW_CURSOR, (int)state);
    }

    void Input::Update(const std::shared_ptr<Window>& window){
        glfwGetCursorPos(window->GetGLFWWindow(), &mouseX, &mouseY);

        if(firstFrame){
            lastMouseX = mouseX;
            lastMouseY = mouseY;
            firstFrame = false;
        }

        mouseDeltaX = mouseX - lastMouseX;
        mouseDeltaY = mouseY - lastMouseY;

        lastMouseX = mouseX;
        lastMouseY = mouseY;
    }

    glm::vec2 Input::GetMousePos(){
        return glm::vec2(mouseX, mouseY);
    }

    glm::vec2 Input::GetMouseDelta(){
        return glm::vec2(mouseDeltaX, mouseDeltaY);
    }
}