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

#ifndef FILE_HPP
#define FILE_HPP

#include <filesystem>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

namespace GLEP{

    class File{
        public: 
            /// @brief Base directory.
            const static std::filesystem::path DIRECTORY;

            /// @brief $DIR/res/
            const static std::filesystem::path RES_PATH;

            /// @brief $DIR/res/shaders/
            const static std::filesystem::path SHADERS_PATH;

            /// @brief $DIR/res/models/
            const static std::filesystem::path MODELS_PATH;

            /// @brief $DIR/res/textures/
            const static std::filesystem::path TEXTURES_PATH;

            /// @brief $DIR/res/audio/
            const static std::filesystem::path AUDIO_PATH;


            /// @brief $DIR/res/shaders/GLEP/
            const static std::filesystem::path GLEP_SHADERS_PATH;

            /// @brief $DIR/res/textures/GLEP/core/default.png
            const static std::filesystem::path GLEP_DEFUALT_TEXTURE;

            /// @brief $DIR/res/texture/GLEP/core/cube_maps/default.cubemap
            const static std::filesystem::path GLEP_DEFAULT_CUBE_MAP;
    };

}

#endif //FILE_HPP