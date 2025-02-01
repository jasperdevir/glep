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

#include <GLEP/core/mesh.hpp>

namespace GLEP {
    Mesh::Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material){
        GeometryData = geometry;
        MaterialData = material;
    }

    Mesh::~Mesh(){}

    json Mesh::ToJson(){
        json j;

        j["geometry_data"] = GeometryData->ToJson();
        j["material_data"] = MaterialData->ToJson();

        return j;
    }

    std::shared_ptr<Mesh> Mesh::FromJson(const json& data){
        return std::make_shared<Mesh>(
            Geometry::FromJson(data["geometry_data"]),
            Material::FromJson(data["material_data"])
        );
    }

}
