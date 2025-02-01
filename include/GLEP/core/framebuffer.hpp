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

#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <GLEP/core/utility/print.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace GLEP{

    class Framebuffer{
        private:
            unsigned int _framebuffer;
            unsigned int _colorBuffer;
            unsigned int _depthBuffer;

            int _width;
            int _height;

            void initialize();

        public:
            Framebuffer();
            Framebuffer(glm::vec2 resolution);
            ~Framebuffer();

            /// @brief Get the framebuffer ID.
            /// @return Framebuffer ID
            unsigned int GetBufferID();

            /// @brief Get the color buffer ID.
            /// @return Color buffer ID
            unsigned int GetColorBufferID();

            /// @brief Get the depth buffer ID.
            /// @return Depth buffer ID
            unsigned int GetDepthBufferID();

            /// @brief Get the width of the buffer in pixels.
            /// @return Width
            int GetWidth();

            /// @brief Get the height of the buffer in pixels.
            /// @return Height
            int GetHeight();


            /// @brief Set the depth buffer ID.
            /// @param depthTexture Depth buffer ID to set
            void SetDepthBufferID(unsigned int depthBuffer);

            /// @brief Set the resolution of the buffer, re-initializing it
            /// @param resolution Resolution to set
            void SetResolution(glm::vec2 resolution);


            /// @brief Bind as the active framebuffer.
            void Bind();

            /// @brief Unbind as the active framebuffer.
            void Unbind();

            /// @brief Bind the color and depth buffer to a shader uniform.
            void BindResult();
    };

}

#endif //FRAMEBUFFER_HPP