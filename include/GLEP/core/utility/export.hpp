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

#ifndef EXPORT_HPP
#define EXPORT_HPP

#include <GLEP/core/utility/print.hpp>

#include <GLEP/core/framebuffer.hpp>
#include <GLEP/core/scene.hpp>

#include <filesystem>
#include <string>

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

namespace GLEP{

    using json = nlohmann::ordered_json;
    
    class Export{
        public:

            /// @brief Export a scene to a JSON file.
            /// @param targetPath File path to write to
            /// @param scene Scene to write to a JSON file
            /// @return If the exporting process was a success
            static bool SceneToFile(std::filesystem::path targetPath, const std::shared_ptr<Scene>& scene);

            /// @brief Export a framebuffer's color buffer to a JPG file.
            /// @param targetPath File path to write to 
            /// @param framebuffer Buffer to read from
            /// @param quality Quality of the result
            static void ColorBufferToJPG(std::filesystem::path targetPath, const std::shared_ptr<Framebuffer>& framebuffer, int quality);
    };
    
}

#endif //EXPORT_HPP