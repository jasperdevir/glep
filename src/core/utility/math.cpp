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

#include <GLEP/core/utility/math.hpp>

namespace GLEP{
    const double Math::PI = 3.14159265358979323846;

    json Math::ToJson(glm::vec2 v){
        json j;
        j["x"] = v.x;
        j["y"] = v.y;
        return j;
    }

    json Math::ToJson(glm::vec3 v){
        json j;
        j["x"] = v.x;
        j["y"] = v.y;
        j["z"] = v.z;
        return j;
    }

    json Math::ToJson(glm::vec4 v){
        json j;
        j["x"] = v.x;
        j["y"] = v.y;
        j["z"] = v.z;
        j["w"] = v.w;
        return j;
    }

    json Math::ToJson(glm::quat v){
        json j;
        j["x"] = v.x;
        j["y"] = v.y;
        j["z"] = v.z;
        j["w"] = v.w;
        return j;
    }

    glm::vec2 Math::Vec2FromJson(const json& data){
        return glm::vec2(data["x"], data["y"]);
    }

    glm::vec3 Math::Vec3FromJson(const json& data){
        return glm::vec3(data["x"], data["y"], data["z"]);
    }

    glm::vec4 Math::Vec4FromJson(const json& data){
        return glm::vec4(data["x"], data["y"], data["z"], data["w"]);
    }

    glm::quat Math::QuatFromJson(const json& data){
        return glm::quat(data["w"], data["x"], data["y"], data["z"]);
    }
}