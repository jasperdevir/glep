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

#ifndef MESH_HPP
#define MESH_HPP

#include <GLEP/core/geometry.hpp>
#include <GLEP/core/material.hpp>
#include <GLEP/core/texture.hpp>

#include <filesystem>
#include <string>
#include <sstream>

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

namespace GLEP {

    using json = nlohmann::ordered_json;
    
    class Mesh{
        public:
            std::shared_ptr<Geometry> GeometryData;
            std::shared_ptr<Material> MaterialData;

            Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material);
            ~Mesh();


            /// @brief Recalaculate each vertex normal of the geometry data using their positions.
            void CalculateNormals();


            /// @brief Serialize data to JSON format.
            /// @return Serialized data
            json ToJson();

            /// @brief Deserialize data from JSON format.
            /// @param data Mesh data in JSON format 
            /// @return Deserialized Mesh
            static std::shared_ptr<Mesh> FromJson(const json& data);
    };

}

#endif //MESH_HPP