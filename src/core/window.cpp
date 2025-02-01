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

#include <GLEP/core/window.hpp>

namespace GLEP{
    
    Window::Window(WindowState state, glm::vec2 resolution, std::string title){
        _resolution = resolution;
        _title = title;
        _state = state;
    }

    Window::~Window(){
        glfwDestroyWindow(_glfwWindow);

        Print(PrintCode::INFO, "WINDOW", "Window '" + _title + "' successfully shutdown");
    }

    GLFWwindow* Window::GetGLFWWindow(){ return _glfwWindow; }
    const GLFWvidmode* Window::GetGLFWVidMode(){ return _glfwVidMode; }
    glm::vec2 Window::GetResolution() { return _resolution; }
    int Window::GetRefreshRate(){ return _refreshRate; }
    std::string Window::GetTitle(){ return _title; }
    std::vector<glm::vec2> Window::GetSizeOptions(){ return _sizeOptions; }
    std::vector<int> Window::GetRefreshRateOptions(){ return _refreshRateOptions; }

    void Window::SetRefreshRate(int refreshRate){
        _refreshRate = refreshRate;
    }

    void Window::SetResolution(glm::vec2 resolution){
        _resolution = resolution;

        glfwSetWindowSize(_glfwWindow, (int)_resolution.x, (int)_resolution.y);
    }

    void Window::SetTitle(std::string title){
        _title = title;

        glfwSetWindowTitle(_glfwWindow, _title.c_str());
    }

    void Window::SetState(WindowState state){
        _state = state;

        switch(_state){
            case WindowState::WINDOWED:
                glfwSetWindowMonitor(_glfwWindow, NULL, 100, 100, (int)_resolution.x, (int)_resolution.y, _refreshRate);
                break;

            case WindowState::WINDOWED_FULL:
                glfwSetWindowMonitor(_glfwWindow, NULL, 0, 0, _glfwVidMode->width, _glfwVidMode->height, _refreshRate);
                _resolution = glm::vec2(_glfwVidMode->width, _glfwVidMode->height);
                break;

            case WindowState::FULLSCREEN:
                glfwSetWindowMonitor(_glfwWindow, glfwGetPrimaryMonitor(), 0, 0, _glfwVidMode->width, _glfwVidMode->height, _refreshRate);
                _resolution = glm::vec2(_glfwVidMode->width, _glfwVidMode->height);
                break;
        }
    }

    void Window::UpdateShouldClose(){
        _shouldClose = glfwWindowShouldClose(_glfwWindow);
    }

    void Window::SetShouldClose(bool state){
        glfwSetWindowShouldClose(_glfwWindow, state);
        UpdateShouldClose();
    }

    bool Window::GetShouldClose(){
        return _shouldClose;
    }

    void Window::Initialize(){
        if(!glfwInit()){
            exit(EXIT_FAILURE);
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_MAJ_VERSION);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_MIN_VERSION);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);
        #endif

        glfwWindowHint(GLFW_SAMPLES, GL_MULTISAMPLES);

        _glfwWindow = glfwCreateWindow((int)_resolution.x, (int)_resolution.y, _title.c_str(), nullptr, nullptr);

        if (!_glfwWindow) {
            Print(PrintCode::CRITICAL, "WINDOW", "Failed to initialize glfwWindow.");
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(_glfwWindow);

        glfwSetWindowUserPointer(_glfwWindow, reinterpret_cast<void*>(this));
        glfwSetFramebufferSizeCallback(_glfwWindow, FramebufferSizeCallback);
        glfwSetWindowCloseCallback(_glfwWindow, WindowCloseCallback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            Print(PrintCode::CRITICAL, "WINDOW", "Failed to initialize glad.");
            exit(EXIT_FAILURE);
        }  

        _glfwVidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        int vidModesCount;
        const GLFWvidmode* vidModes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &vidModesCount);

        std::set<int> width;
        std::set<int> height;
        std::set<int> refresh;

        for(int i = 0; i < vidModesCount; i++){
            width.insert(vidModes[i].width);
            height.insert(vidModes[i].height);
            refresh.insert(vidModes[i].refreshRate);
        }

        auto widthIt = width.begin(); 
        auto heightIt = height.begin(); 

        while (widthIt != width.end() && heightIt != height.end()) {
            _sizeOptions.push_back(glm::vec2(*widthIt, *heightIt));
            ++widthIt;
            ++heightIt;
        }

        _refreshRateOptions.assign(refresh.begin(), refresh.end());

        SetState(_state);

        Print(PrintCode::INFO, "WINDOW", "Window '" + _title + "' successfully initialized with resolution "  + std::to_string((int)_resolution.x) + " x " + std::to_string((int)_resolution.y));
    }

    void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height){
        if(auto glepWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window)))
            glepWindow->SetResolution(glm::vec2(width, height));
            glViewport(0, 0, width, height);
    }

    void Window::WindowCloseCallback(GLFWwindow* window){
        if(auto glepWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window)))
            glepWindow->UpdateShouldClose();
    }
}