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

#ifndef SCENE_OBJECT_HPP
#define SCENE_OBJECT_HPP

#include <GLEP/core/utility/file.hpp>
#include <GLEP/core/utility/math.hpp>
#include <GLEP/core/utility/print.hpp>

#include <GLEP/core/object_component.hpp>

#include <string>
#include <sstream>
#include <memory>
#include <vector>

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace GLEP {

    using json = nlohmann::ordered_json;

    class ObjectComponent;
    
    class SceneObject : public std::enable_shared_from_this<SceneObject>{       
        protected:
            bool _firstUpdate = true;

            glm::mat4 _modelMatrix;

            glm::vec3 _worldPosition;
            glm::quat _worldRotation;
            glm::vec3 _worldScale;

            std::vector<std::shared_ptr<SceneObject>> _children;
            std::vector<std::unique_ptr<ObjectComponent>> _components;

            void updateModelMatrix();

        public:
            std::string Name = "Object";
            std::shared_ptr<SceneObject> Parent;

            glm::vec3 Position;
            glm::quat Rotation;
            glm::vec3 Scale;

            SceneObject();
            virtual ~SceneObject() = default;

            /// @brief Get the model matrix.
            /// @return Model matrix
            glm::mat4 GetModelMatrix();

            /// @brief Get the world postion.
            /// @return World position
            glm::vec3 GetWorldPosition();

            /// @brief Get the world rotation.
            /// @return World rotation
            glm::quat GetWorldRotation();

            /// @brief Get the world scale.
            /// @return World scale
            glm::vec3 GetWorldScale();

            /// @brief Get all children assigned to this object.
            /// @return Children objects
            std::vector<std::shared_ptr<SceneObject>>& GetChildren();

            /// @brief Get all components assigned to this object.
            /// @return Components
            std::vector<std::unique_ptr<ObjectComponent>>& GetComponents();


            /// @brief Make an object the child of this object.
            /// @param object Child to add
            void Add(std::shared_ptr<SceneObject> object);

            /// @brief Add a component to this object.
            /// @param component Component to add
            void Add(std::unique_ptr<ObjectComponent> component);


            /// @brief Trigger each components' update function and update this object's world transform.
            void Update();

            /// @brief Update this object's world transform.
            void UpdateTransformVectors();

            /// @brief Find the first matching component by its type.
            /// @tparam T Component type
            /// @return Found component, will return nullptr if not found
            template <typename T>
            std::unique_ptr<T>& GetComponentByType(){
                for(auto& c : _components){
                    if(auto cast = std::dynamic_pointer_cast<T>(c)){
                        return cast;
                    }
                }

                return nullptr;
            }

            /// @brief Find all matching components by their type.
            /// @tparam T Component type
            /// @return Found components
            template <typename T>
            std::vector<std::unique_ptr<T>&> GetComponentsByType(){
                std::vector<std::unique_ptr<T>&> result;
                for(auto& c : _components){
                    if(auto cast = std::dynamic_pointer_cast<T>(c)){
                        result.push_back(cast);
                    }
                }

                return result;
            }


            /// @brief Serialize data to JSON format.
            /// @return Serialized data
            virtual json ToJson();

            /// @brief Deserialize data from JSON format.
            /// @param data SceneObject data in JSON format
            /// @return Deserialized SceneObject
            static std::shared_ptr<SceneObject> FromJson(const json& data);

            /// @brief Apply SceneObject data to an object from JSON format.
            /// @param object Object to apply data
            /// @param data SceneObject data in JSON format
            static void ApplyFromJson(const std::shared_ptr<SceneObject>& object, const json& data);
    };
}

#endif //SCENE_OBJECT_HPP