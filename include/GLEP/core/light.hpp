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

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <GLEP/core/utility/print.hpp>
#include <GLEP/core/utility/math.hpp>

#include <GLEP/core/color.hpp>
#include <GLEP/core/scene_object.hpp>
#include <GLEP/core/material.hpp>

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

namespace GLEP {

    using json = nlohmann::ordered_json;

    class Material;

    enum class LightType {
        AMBIENT,
        POINT,
        DIRECTION,
        SPOT
    };

    class Light{
        protected:
            LightType _type;

        public:
            Color LightColor;
            float Intensity;

            Light(LightType type, Color lightColor, float intensity);

            /// @brief Bind the light data to a specifed material's shader uniforms.
            /// @param material Target material
            /// @param index Uniform array index
            virtual void Bind(const std::shared_ptr<Material>& material, int index) = 0;


            /// @brief Get the type of the light
            /// @return Type
            LightType GetType();


            /// @brief Serialize data to JSON format.
            /// @return Serialized data
            virtual json ToJson();

            /// @brief Deserialize data from JSON format.
            /// @param data Light data in JSON format
            /// @return Deserialized Light
            static std::shared_ptr<Light> FromJson(const json& data);

    };

    class AmbientLight : public Light{
        public:  
            AmbientLight();
            AmbientLight(Color ambientColor, float intensity);


            /// @brief Bind the light data to a specifed material's shader uniforms.
            /// @param material Target material
            /// @param index Uniform array index (Unused)
            void Bind(const std::shared_ptr<Material>& material, int index) override;


            /// @brief Serialize data in JSON format.
            /// @return Serialized data
            json ToJson() override;

            /// @brief Deserialize data from JSON format.
            /// @param data AmbientLight data in JSON format
            /// @return Deserialized AmbientLight
            static std::shared_ptr<AmbientLight> FromJson(const json& data);

    };

    class PointLight : public Light {
        public:
            glm::vec3 Position;
            float Constant;
            float Linear;
            float Quadratic;

            PointLight(glm::vec3 position, Color color, float intensity, float constant, float linear, float quadratic);


            /// @brief Bind the light data to a specifed material's shader uniforms.
            /// @param material Target material
            /// @param index Uniform array index
            void Bind(const std::shared_ptr<Material>& material, int index) override;


            /// @brief Serialize data in JSON format.
            /// @return Serialized data
            json ToJson() override;

            /// @brief Deserialize data from JSON format.
            /// @param data PointLight data in JSON format
            /// @return Deserialzied PointLight
            static std::shared_ptr<PointLight> FromJson(const json& data);

    };

    class DirectionalLight : public Light {
        public:
            glm::vec3 Direction;

            DirectionalLight(glm::vec3 direction, Color lightColor, float intensity);


            /// @brief Bind the light data to a specifed material's shader uniforms.
            /// @param material Target material
            /// @param index Uniform array index (Unused)
            void Bind(const std::shared_ptr<Material>& material, int index) override;


            /// @brief Serialize data in JSON format.
            /// @return Serialized data
            json ToJson() override;

            /// @brief Deserialize data from JSON format.
            /// @param data DirectionalLight data in JSON format
            /// @return Deserialized DirectionalLight
            static std::shared_ptr<DirectionalLight> FromJson(const json& data);

    };

    class SpotLight : public Light {
        public:
            glm::vec3 Position;
            glm::vec3 Direction;
            float InnerCutOff;
            float OuterCutOff;

            float Constant;
            float Linear;
            float Quadratic;

            SpotLight(glm::vec3 position, glm::vec3 direction, Color lightColor, float intensity, float innerCutOff, float outerCutOff, float constant, float linear, float quadratic);


            /// @brief Bind the light data to a specifed material's shader uniforms.
            /// @param material Target material
            /// @param index Uniform array index
            void Bind(const std::shared_ptr<Material>& material, int index) override;


            /// @brief Serialize data in JSON format.
            /// @return Serialized data
            json ToJson() override;

            /// @brief Deserialize data from JSON format.
            /// @param data SpotLight data in JSON format
            /// @return Deserialized data
            static std::shared_ptr<SpotLight> FromJson(const json& data);

    };

}

#endif //LIGHT_HPP