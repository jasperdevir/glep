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

#ifndef COLOR_HPP
#define COLOR_HPP

#include <array>

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

namespace GLEP{

    using json = nlohmann::ordered_json;

    class Color{
        public:
            float r;
            float g;
            float b;
            float a;

            Color();
            Color(float v);
            Color(float r, float g, float b);
            Color(float r, float g, float b, float a);
            Color(glm::vec4 rgba);
            Color(std::array<float, 4> v);

            /// @brief Generate an array.
            /// @return Values in an array
            std::array<float, 4> ToArray();

            /// @brief Serialize data to JSON data.
            /// @return Serialized data
            json ToJson();

            /// @brief Deserialize data from JSON format.
            /// @param data Color data in JSON format
            /// @return Deserialized Color
            static Color FromJson(const json& data);


            /// @brief Color(1.0f, 0.0f, 0.0f, 1.0f)
            static const Color RED;

            /// @brief Color(0.0f, 1.0f, 0.0f, 1.0f)
            static const Color GREEN;

            /// @brief Color(0.075f, 0.8f, 0.25f, 1.0f)
            static const Color GLEP_GREEN;

            /// @brief Color(1.0f, 1.0f, 0.0f, 1.0f)
            static const Color YELLOW;

            /// @brief Color(0.0f, 0.0f, 1.0f, 1.0f)
            static const Color BLUE;

            /// @brief Color(1.0f, 0.0f, 1.0f, 1.0f)
            static const Color MAGENTA;

            /// @brief Color(0.0f, 1.0f, 1.0f, 1.0f)
            static const Color CYAN;

            /// @brief Color(1.0f, 1.0f, 1.0f, 1.0f)
            static const Color WHITE;

            /// @brief Color(0.0f, 0.0f, 0.0f, 1.0f)
            static const Color BLACK;

            /// @brief Color(0.0f, 0.0f, 0.0f, 0.0f);
            static const Color CLEAR;
    };

}



#endif //COLOR_HPP