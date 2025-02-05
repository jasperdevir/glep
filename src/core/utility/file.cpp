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

#include <GLEP/core/utility/file.hpp>

namespace GLEP {

    const std::filesystem::path File::DIRECTORY = std::filesystem::current_path().parent_path();
    const std::filesystem::path File::RES_PATH = DIRECTORY / "res";
    const std::filesystem::path File::SHADERS_PATH = RES_PATH / "shaders";
    const std::filesystem::path File::MODELS_PATH = RES_PATH / "models";
    const std::filesystem::path File::TEXTURES_PATH = RES_PATH / "textures";
    const std::filesystem::path File::AUDIO_PATH = RES_PATH / "audio";

    const std::filesystem::path File::GLEP_DEFAULT_TEXTURE = TEXTURES_PATH / "GLEP" / "core" / "default.png";
    const std::filesystem::path File::GLEP_DEFAULT_CUBE_MAP = TEXTURES_PATH / "GLEP" / "core" / "cube_maps" / "default.cubemap";
    const std::filesystem::path File::GLEP_SHADERS_PATH = SHADERS_PATH / "GLEP" / "core";
    const std::filesystem::path File::GLEP_DEFAULT_VERTEX_SHADER = SHADERS_PATH / "GLEP" / "core" / "default.vs";
     
};