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

#ifndef SHADER_HPP
#define SHADER_HPP

#include <GLEP/core/utility/print.hpp>
#include <GLEP/core/utility/file.hpp>

#include <filesystem>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>

namespace GLEP {

    class Shader{
        private:
            std::filesystem::path _vsFilePath;
            std::filesystem::path _gsFilePath;
            std::filesystem::path _fsFilePath;
            std::string _vsSrc;
            std::string _gsSrc;
            std::string _fsSrc;


            unsigned int _ID;

            bool checkCompileErrors(unsigned int shader, std::string type);
            bool readFiles();

            bool initialize();

        public:
            Shader();
            Shader(std::filesystem::path fsFilePath);
            Shader(std::filesystem::path vsFilePath, std::filesystem::path fsFilePath);
            Shader(std::filesystem::path vsFilePath, std::filesystem::path gsFilePath, std::filesystem::path fsFilePath);
            ~Shader();

            /// @brief Get the shader program ID.
            /// @return Shader program ID
            unsigned int GetID();

            /// @brief Get the file path of the vertex shader.
            /// @return Vertex shader file path
            std::filesystem::path GetVsPath();

            /// @brief Get the file path of the Geometry shader.
            /// @return Geometry shader file path
            std::filesystem::path GetGsPath();

            /// @brief Get the file path of the fragment shader.
            /// @return Fragment shader file path
            std::filesystem::path GetFsPath();


            /// @brief Set as the active shader program.
            void Use();
    };

}

#endif //SHADER_HPP