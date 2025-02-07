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
        protected:
            unsigned int _framebuffer;

            int _width;
            int _height;

            virtual void initialize() = 0;

        public:
            Framebuffer();
            Framebuffer(glm::vec2 resolution);
            virtual ~Framebuffer();

            /// @brief Get the framebuffer ID.
            /// @return Framebuffer ID
            unsigned int GetBufferID();

            /// @brief Get the width of the buffer in pixels.
            /// @return Width
            int GetWidth();

            /// @brief Get the height of the buffer in pixels.
            /// @return Height
            int GetHeight();


            /// @brief Set the resolution of the buffer, re-initializing it
            /// @param resolution Resolution to set
            void SetResolution(glm::vec2 resolution);


            /// @brief Bind as the active framebuffer.
            virtual void Bind() = 0;

            /// @brief Unbind as the active framebuffer.
            void Unbind();

            /// @brief Bind the color and depth buffer to a shader uniform.
            virtual void BindResult() = 0;
    };

    class ColorFramebuffer : public Framebuffer{
        private:
            unsigned int _colorBufferID;

            void initialize() override;

        public:
            ColorFramebuffer();
            ColorFramebuffer(glm::vec2 resolution);
            ~ColorFramebuffer();

            unsigned int GetColorBufferID();

            void OverrideColorBuffer(unsigned int colorBuffer);

            void Bind() override;

            void BindResult() override;

    };

    class DepthFramebuffer : public Framebuffer{
        private:
            unsigned int _depthBufferID;

            void initialize() override;

        public:
            DepthFramebuffer();
            DepthFramebuffer(glm::vec2 resolution);
            ~DepthFramebuffer();

            unsigned int GetDepthBufferID();

            void OverrideDepthBuffer(unsigned int depthBuffer);

            void Bind() override;

            void BindResult() override;
    };

    class ColorDepthFramebuffer : public Framebuffer{
        private:
            unsigned int _colorBufferID;
            unsigned int _depthBufferID;

            void initialize() override;

        public:
            ColorDepthFramebuffer();
            ColorDepthFramebuffer(glm::vec2 resolution);
            ~ColorDepthFramebuffer();

            unsigned int GetColorBufferID();
            unsigned int GetDepthBufferID();

            void OverrideColorBuffer(unsigned int colorBuffer);
            void OverrideDepthBuffer(unsigned int depthBuffer);

            void Bind() override;

            void BindResult() override;
    };

}

#endif //FRAMEBUFFER_HPP