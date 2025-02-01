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

#include <GLEP/core/color.hpp>

namespace GLEP {

    const Color Color::RED = Color(1.0f, 0.0f, 0.0f);
    const Color Color::GREEN = Color(0.0f, 1.0f, 0.0f);
    const Color Color::GLEP_GREEN = Color(0.075f, 0.8f, 0.25f);
    const Color Color::YELLOW = Color(1.0f, 1.0f, 0.0f);
    const Color Color::BLUE = Color(0.0f, 0.0f, 1.0f);
    const Color Color::MAGENTA = Color(1.0f, 0.0f, 1.0f);
    const Color Color::CYAN = Color(0.0f, 1.0f, 1.0f);
    const Color Color::WHITE = Color(1.0f);
    const Color Color::BLACK = Color(0.0f);

    Color::Color(): r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}

    Color::Color(float v) : r(v), g(v), b(v), a(1.0f) {}

    Color::Color(float r, float g, float b) : r(r), g(g), b(b), a(1.0f) {}

    Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

    Color::Color(glm::vec4 rgba): r(rgba.x), g(rgba.y), b(rgba.z), a(rgba.w) {}

    Color::Color(std::array<float, 4> v) : r(v[0]), g(v[1]), b(v[2]), a(v[3]) {}

    std::array<float, 4> Color::ToArray(){
        return {r,g,b,a};
    }

    json Color::ToJson(){
        json j;
        j["r"] = r;
        j["g"] = g;
        j["b"] = b;
        j["a"] = a;
        return j;
    }

    Color Color::FromJson(const json& data){
        return Color(data["r"], data["g"], data["b"], data["a"]);
    }
}


