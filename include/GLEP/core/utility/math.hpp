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

#ifndef MATH_HPP
#define MATH_HPP

#include <algorithm>

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace GLEP{

    using json = nlohmann::ordered_json;

    enum class InterpFunc{
        LINEAR,
        EASE_IN_SINE,
        EASE_OUT_SINE,
        EASE_IN_OUT_SINE,
        EASE_IN_QUAD,
        EASE_OUT_QUAD,
        EASE_IN_OUT_QUAD,
        EASE_IN_CUBIC,
        EASE_OUT_CUBIC,
        EASE_IN_OUT_CUBIC
    };

    class Math{
        public:
            /// @brief PI to 20 decimal places
            static const double PI;

            /// @brief Serialize data to JSON format.
            /// @param value Value to convert
            /// @return Serialized data
            static json ToJson(glm::vec2 value);
            
            /// @brief Serialize data to JSON format.
            /// @param value Value to convert
            /// @return Serialized data
            static json ToJson(glm::vec3 value);
            
            /// @brief Serialize data to JSON format.
            /// @param value Value to convert
            /// @return Serialized data
            static json ToJson(glm::vec4 value);
            
            /// @brief Serialize data to JSON format.
            /// @param value Value to convert
            /// @return Serialized data
            static json ToJson(glm::quat value);

            /// @brief Deserialize data from JSON format.
            /// @param data glm::vec2 data in JSON format
            /// @return Deserialized glm::vec2
            static glm::vec2 Vec2FromJson(const json& data);

            /// @brief Deserialize data from JSON format.
            /// @param data glm::vec3 data in JSON format
            /// @return Deserialized glm::vec3
            static glm::vec3 Vec3FromJson(const json& data);

            /// @brief Deserialize data from JSON format.
            /// @param data glm::vec4 data in JSON format
            /// @return Deserialized glm::vec4
            static glm::vec4 Vec4FromJson(const json& data);

            /// @brief Deserialize data from JSON format.
            /// @param data glm::quat data in JSON format
            /// @return Deserialized glm::quat
            static glm::quat QuatFromJson(const json& data);

            /// @brief Linearly interpolate from one value to another.
            /// @tparam T Target type
            /// @param v0 Start value
            /// @param v1 End value
            /// @param t Time interval
            /// @return Interpolated result
            template <typename T>
            static T Lerp(T v0, T v1, float t){
                return (1 - t) * v0 + t * v1;
            }

            /// @brief Interpolate from one value to another using an interpolation function.
            /// @tparam T Target type
            /// @param func Interpolation function
            /// @param v0 Start value
            /// @param v1 End value
            /// @param t Time interval
            /// @return Interpolated result
            template <typename T>
            static T Interp(InterpFunc func, T v0, T v1, float t){
                t = GetInterpFunc(func, t);
                return Lerp(v0, v1, t);
            }

            /// @brief Get an interpolated time interval based on an interpolation function.
            /// @param func Type of function
            /// @param t Time interval
            /// @return Interpolated time interval
            static float GetInterpFunc(InterpFunc func, float t){
                switch(func){
                    case InterpFunc::EASE_IN_SINE:
                        return 1 - (float)cos((t * (float)PI) / 2);

                    case InterpFunc::EASE_OUT_SINE:
                        return (float)sin((t * (float)PI) / 2);

                    case InterpFunc::EASE_IN_OUT_SINE:
                        return -((float)cos((float)PI * t) - 1) / 2;

                    case InterpFunc::EASE_IN_QUAD:
                        return t * t;

                    case InterpFunc::EASE_OUT_QUAD:
                        return 1 - (1 - t) * (1 - t);

                    case InterpFunc::EASE_IN_OUT_QUAD:
                        return t < 0.5 ? 2 * t * t : 1 - (float)pow(-2 * t + 2, 2) / 2;

                    case InterpFunc::EASE_IN_CUBIC:
                        return t * t * t;

                    case InterpFunc::EASE_OUT_CUBIC:
                        return 1 - (float)pow(1 - t, 3);

                    case InterpFunc::EASE_IN_OUT_CUBIC:
                        return t < 0.5 ? 4 * t * t * t : 1 - (float)pow(-2 * t + 2, 3) / 2;

                    default:
                        return t;
                }
                
            }
    };
    
}

#endif //MATH_HPP