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

#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GLEP/core/utility/opengl.hpp>
#include <GLEP/core/utility/print.hpp>

#include <GLEP/core/camera.hpp>

#include <functional>
#include <vector>
#include <string>
#include <set>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace GLEP{
    enum class WindowState{
        WINDOWED,
        WINDOWED_FULL,
        FULLSCREEN
    };

    class Window{
        protected:
            GLFWwindow* _glfwWindow;
            const GLFWvidmode* _glfwVidMode;
            WindowState _state;
            std::vector<glm::vec2> _sizeOptions;
            std::vector<int> _refreshRateOptions;

            glm::vec2 _resolution;
            int _refreshRate;
            std::string _title;

            bool _shouldClose = false;

        public:
            Window(WindowState state, glm::vec2 resolution, std::string title);
            ~Window();

            /// @brief Initialize GLFW and OpenGL contexts, and create an application window (Called by Renderer).
            void Initialize();

            /// @brief Get the GLFWWindow object.
            /// @return GLFWWindow object
            GLFWwindow* GetGLFWWindow();

            /// @brief Get the current resolution.
            /// @return Current resolution
            glm::vec2 GetResolution();

            /// @brief Get the current refresh rate.
            /// @return Current refresh rate
            int GetRefreshRate();

            /// @brief Get the window title.
            /// @return Window itle
            std::string GetTitle();

            /// @brief Get the GLFWVidMode object.
            /// @return GLFWVidMode object
            const GLFWvidmode* GetGLFWVidMode();

            /// @brief Get the resolution options based on the monitor that contains the window.
            /// @return Resolution options
            std::vector<glm::vec2> GetSizeOptions();

            /// @brief Get the refresh rate options based on the monitor that contains the window.
            /// @return Refresh rate options
            std::vector<int> GetRefreshRateOptions();


            /// @brief Set the current resolution.
            /// @param resolution Resolution to set
            void SetResolution(glm::vec2 resolution);

            /// @brief Set the current refresh rate.
            /// @param refreshRate Refresh rate to set
            void SetRefreshRate(int refreshRate);

            /// @brief Set the window title.
            /// @param title Title to set
            void SetTitle(std::string title);

            /// @brief Set the current state of the window.
            /// @param state State to set
            void SetState(WindowState state);


            /// @brief Query the window and update if it should close.
            void UpdateShouldClose();

            /// @brief Set if the window should close.
            /// @param state State to set
            void SetShouldClose(bool state);

            /// @brief Get if the window should currently close.
            /// @return Should the window close
            bool GetShouldClose();


            /// @brief Convert a screen-space point to a world-space point using a camera.
            /// @param camera Active camera
            /// @param screenPos Screen-space point
            /// @return World-space conversion
            glm::vec3 GetScreenToWorld(const std::shared_ptr<Camera>& camera, glm::vec2 screenPos);

            static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
            static void WindowCloseCallback(GLFWwindow* window);

    };
}
    

#endif //WINDOW_HPP