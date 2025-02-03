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

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <GLEP/core/utility/file.hpp>
#include <GLEP/core/utility/print.hpp>

#include <GLEP/core/framebuffer.hpp>
#include <GLEP/core/texture.hpp>
#include <GLEP/core/cube_map.hpp>
#include <GLEP/core/shader.hpp>

#include <vector>
#include <filesystem>
#include <sstream>

#include <nlohmann/json.hpp>

namespace GLEP{

    using json = nlohmann::ordered_json;

    class Material;
    template <typename T>
    class ShaderUniform;

    enum class MaterialCull{
        NONE,
        FRONT = 0x0404,
        BACK = 0x0405,
        FRONT_AND_BACK = 0x0408
    };

    class TypelessShaderUniform {
        public:
            std::string Name;

            TypelessShaderUniform(std::string name);

            /// @brief Bind the uniform to a material's shader.
            /// @param material Target material
            virtual void SetUniform(Material* material) = 0;


            /// @brief Serialize data to JSON format.
            /// @return Serialized data
            virtual json ToJson() = 0;
    };

    class Material{
        protected:
            std::string _name;
            std::shared_ptr<Shader> _shader;

            std::vector<std::shared_ptr<TypelessShaderUniform>> _uniforms;

        public:
            bool LightingRequired = false;
            bool ReceiveShadows = false;
            bool CastShadows = false;
            bool BakeRequired = false;
            bool Wireframe = false;
            MaterialCull CullFace = MaterialCull::BACK;

            Material();
            Material(std::shared_ptr<Material> material, bool copyUniforms = true);
            Material(std::filesystem::path vsFilePath, std::filesystem::path fsFilePath);
            Material(std::shared_ptr<Shader> shader);
            ~Material();


            /// @brief Get the name of the material.
            /// @return Name
            std::string GetName();

            /// @brief Get the shader of the material.
            /// @return Shader
            std::shared_ptr<Shader> GetShader();

            /// @brief Get all uniforms assigned to the material.
            /// @return Uniforms
            std::vector<std::shared_ptr<TypelessShaderUniform>> GetUniforms();


            /// @brief Add a uniform to this material.
            /// @tparam T Uniform type
            /// @param name Uniform name
            /// @param value Uniform value
            template <typename T>
            void AddUniform(const std::string& name, T value) {
                if(name == "normalTex" && GetUniform<bool>("hasNormalMap") == nullptr){
                    AddUniform<bool>("hasNormalMap", true);
                } else if (name == "depthTex" && GetUniform<bool>("hasDepthTex") == nullptr){
                    AddUniform<bool>("hasDepthTex", true);
                    AddUniform<float>("depthScale", 1.0f);
                }


                std::string uniformName = "uMaterial." + name;
                std::shared_ptr<ShaderUniform<T>> uniform = std::make_shared<ShaderUniform<T>>(uniformName, value);
                uniform->SetUniform(this);

                _uniforms.push_back(uniform);
            }


            /// @brief Get a uniform assigned to this material.
            /// @tparam T Uniform type
            /// @param name Uniform name
            /// @return First uniform that matches the specified name, will return nullptr if not found.
            template <typename T>
            std::shared_ptr<ShaderUniform<T>> GetUniform(const std::string& name){
                std::string uniformName = "uMaterial." + name;
                for(auto& u : _uniforms){
                    auto uniform = std::dynamic_pointer_cast<ShaderUniform<T>>(u);
                    if(uniform && uniform->Name == uniformName){
                        return uniform;
                    }
                }

                return nullptr;
            }


            /// @brief Get the value of a specified uniform.
            /// @tparam T Uniform type
            /// @param name Uniform name
            /// @param nullValue Value to return if uniform is not found
            /// @return Uniform value
            template <typename T>
            T GetUniformValue(const std::string& name, T nullValue){
                std::shared_ptr<ShaderUniform<T>> uniform = GetUniform<T>(name);

                if(!uniform) return nullValue;
                
                return uniform->Value;
            }

            /// @brief Set the value of a specified uniform.
            /// @tparam T Uniform type
            /// @param name Uniform name
            /// @param value Uniform value to set
            /// @return The uniform that's value was updated, will return nullptr if not found.
            template <typename T>
            std::shared_ptr<ShaderUniform<T>> SetUniformValue(const std::string& name, T value){
                std::shared_ptr<ShaderUniform<T>> uniform = GetUniform<T>(name);

                if(!uniform) return nullptr;
                
                uniform->Value = value;

                return uniform;
            }


            /// @brief Bind this material's shader and it's assigned uniforms.
            void Use();


            /// @brief Get the ID of a uniforms location based on its name.
            /// @param name Uniform name
            /// @return Location ID
            GLint GetUniformLocation(const std::string &name);

            /// @brief Set a uniform value.
            /// @param name Uniform name
            /// @param value Uniform value to set
            void SetUniform(const std::string &name, bool value);

            /// @brief Set a uniform value.
            /// @param name Uniform name
            /// @param value Uniform value to set
            void SetUniform(const std::string &name, int value);

            /// @brief Set a uniform value.
            /// @param name Uniform name
            /// @param value Uniform value to set
            void SetUniform(const std::string &name, float value);

            /// @brief Set a uniform value.
            /// @param name Uniform name
            /// @param value Uniform value to set
            void SetUniform(const std::string &name, glm::vec2 value);

            /// @brief Set a uniform value.
            /// @param name Uniform name
            /// @param value Uniform value to set
            void SetUniform(const std::string &name, glm::vec3 value);

            /// @brief Set a uniform value.
            /// @param name Uniform name
            /// @param value Uniform value to set
            void SetUniform(const std::string &name, glm::vec4 value);

            /// @brief Set a uniform value.
            /// @param name Uniform name
            /// @param value Uniform value to set
            void SetUniform(const std::string &name, Color value);

            /// @brief Set a uniform value.
            /// @param name Uniform name
            /// @param value Uniform value to set (glm::mat4)
            void SetUniform(const std::string &name, float* value);

            /* Uniform Texture Slot Layout
            TEXTURE
                0 = DIFFUSE
                1 = SPECULAR
                2 = NORMAL
                3 = HEIGHT
            
            FRAMEBUFFER
                4 = COLOR
                5 = DEPTH

            CUBEMAP
                6 = CUBEMAP
            */

            /// @brief Set a uniform value.
            /// @param name Uniform name
            /// @param value Uniform value to set
            void SetUniform(const std::string &name, std::shared_ptr<Texture> value);

            /// @brief Set a uniform value.
            /// @param name Uniform name
            /// @param value Uniform value to set
            void SetUniform(const std::string &name, std::shared_ptr<TextureMap> value);

            /// @brief Set a uniform value.
            /// @param name Uniform name
            /// @param value Uniform value to set
            void SetUniform(const std::string &name, std::shared_ptr<CubeMap> value);

            /// @brief Set a uniform value.
            /// @param name Uniform name
            /// @param value Uniform value to set
            void SetUniform(const std::string &name, std::shared_ptr<Framebuffer> value);


            /// @brief Serialize data to JSON format.
            /// @return Serialized data 
            json ToJson();

            /// @brief Deserialize data from JSON format.
            /// @param data Material data in JSON format
            /// @return Deserialized Material
            static std::shared_ptr<Material> FromJson(const json& data);

    };

    template <typename T>
    class ShaderUniform : public TypelessShaderUniform{
        public:
            T Value;

            ShaderUniform<T>(std::string name, T value)
            : TypelessShaderUniform(name) {
                Value = value;
            }


            /// @brief Bind the uniform to a material's shader.
            /// @param material Target material
            void SetUniform(Material* material) override{
                material->SetUniform(Name, Value);
            }


            /// @brief Serialize data to JSON format.
            /// @return Serialized data
            json ToJson() override {
                json j;

                j["name"] = Name;
                j["value"] = json();

                return j;
            }   
    };

    class UVMaterial : public Material {
        public:
            UVMaterial();

            /// @brief Deserialize data from JSON format.
            /// @param data UVMaterial data in JSON format
            /// @return Deserialized UVMaterial
            static std::shared_ptr<UVMaterial> FromJson(const json& data);
    };

    class DepthMaterial : public Material {
        public:
            DepthMaterial(float nearPlane, float farPlane, float resultScale = 1.0f);

            /// @brief Deserialize data from JSON format.
            /// @param data DepthMaterial data in JSON format
            /// @return Deserialized DepthMaterial
            static std::shared_ptr<DepthMaterial> FromJson(const json& data);

    }; 

    class SkyboxMaterial : public Material {
        public:
            SkyboxMaterial(std::shared_ptr<TextureCubeMap> cubeMap);

            /// @brief Deserialize data from JSON format.
            /// @param data SkyboxMaterial data in JSON format
            /// @return Deserialized SkyboxMaterial
            static std::shared_ptr<SkyboxMaterial> FromJson(const json& data);

    };

    class ReflectionMaterial : public Material {
        public:
            ReflectionMaterial(std::shared_ptr<CubeMap> cubeMap, Color tint = Color::WHITE);

            /// @brief Deserialize data from JSON format.
            /// @param data ReflectionMaterial data in JSON format
            /// @return Deserialized ReflectionMaterial
            static std::shared_ptr<ReflectionMaterial> FromJson(const json& data);

    };

    class RefractionMaterial : public Material {
        public:
            RefractionMaterial(std::shared_ptr<CubeMap> cubeMap, float refractiveIndex, Color tint = Color::WHITE);

            /// @brief Deserialize data from JSON format.
            /// @param data RefractionMaterial data in JSON format
            /// @return Deserialized RefractionMaterial
            static std::shared_ptr<RefractionMaterial> FromJson(const json& data);
    };

    class UnlitMaterial : public Material {            
        public:
            UnlitMaterial(Color diffuse);
            UnlitMaterial(std::shared_ptr<Texture> diffuse);

            /// @brief Deserialize data from JSON format.
            /// @param data UnlitMaterial data in JSON format
            /// @return Deserialized UnlitMaterial
            static std::shared_ptr<UnlitMaterial> FromJson(const json& data);
    }; 

    class LambertMaterial : public Material {
        public:
            LambertMaterial(Color diffuse);
            LambertMaterial(std::shared_ptr<Texture> diffuse);

            /// @brief Deserialize data from JSON format.
            /// @param data LambertMaterial data in JSON format
            /// @return Deserialized LambertMaterial
            static std::shared_ptr<LambertMaterial> FromJson(const json& data);
    };

    class PhongMaterial : public Material{
        public:
            PhongMaterial(Color diffuse, Color specular, float shininess);
            PhongMaterial(std::shared_ptr<Texture> diffuse, Color specular, float shininess);
            PhongMaterial(std::shared_ptr<Texture> diffuse, std::shared_ptr<Texture> specular, float shininess);

            /// @brief Deserialize data from JSON format.
            /// @param data PhongMaterial data in JSON format
            /// @return Deserialized PhongMaterial
            static std::shared_ptr<PhongMaterial> FromJson(const json& data);
    };

    class BlinnPhongMaterial : public Material{
        public:
            BlinnPhongMaterial(Color diffuse, Color specular, float shininess);
            BlinnPhongMaterial(std::shared_ptr<Texture> diffuse, Color specular, float shininess);
            BlinnPhongMaterial(std::shared_ptr<Texture> diffuse, std::shared_ptr<Texture> specular, float shininess);

            /// @brief Deserialize data from JSON format.
            /// @param data BlinnPhongMaterial data in JSON format
            /// @return Deserialized BlinnPhongMaterial
            static std::shared_ptr<BlinnPhongMaterial> FromJson(const json& data);
    };

}

#endif //MATERIAL_HPP