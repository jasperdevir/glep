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

#ifndef SCENE_HPP
#define SCENE_HPP

#include <GLEP/core/color.hpp>
#include <GLEP/core/light.hpp>
#include <GLEP/core/buffer_pass.hpp>
#include <GLEP/core/mesh.hpp>
#include <GLEP/core/model.hpp>
#include <GLEP/core/cube_map.hpp>

#include <vector>
#include <filesystem>

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

namespace GLEP {

    using json = nlohmann::ordered_json;

    struct SceneLightData{
        bool AmbientLight = false;
        bool DirectionalLight = false;
        int PointLightsAmt = 0;
        int SpotLightsAmt = 0;
    };

    class Scene{
        private:
            SceneLightData _lightData;

            std::vector<std::shared_ptr<BakedCubeMap>> _bakedCubeMaps;
            std::vector<std::shared_ptr<SceneObject>> _objects;
            std::vector<std::shared_ptr<Light>> _lights;

            std::shared_ptr<CubeMap> findClosestCubeMap(glm::vec3 position);

        public:
            std::shared_ptr<BufferPassComposer> PassComposer;
            std::shared_ptr<Mesh> Skybox;

            Scene();
            ~Scene();

            /// @brief Get the current light data of the scene.
            /// @return Light data
            SceneLightData GetLightData();

            /// @brief Get all the objects currently in the scene.
            /// @return Objects
            std::vector<std::shared_ptr<SceneObject>>& GetObjects();

            /// @brief Get an object by its index.
            /// @param index Object index
            /// @return Found object, will return nullptr if not valid
            std::shared_ptr<SceneObject> GetObject(int index);

            /// @brief Get all the lights currently in the scene.
            /// @return Lights
            std::vector<std::shared_ptr<Light>>& GetLights();

            /// @brief Get a light by its index.
            /// @param index Light index
            /// @return Found light, will return nullptr if not valid
            std::shared_ptr<Light> GetLight(int index);

            /// @brief Get the directional light of the scene.
            /// @return Directional light, will return nullptr if no directional light has been assigned.
            std::shared_ptr<DirectionalLight> GetDirectionalLight();

            /// @brief Get all the baked cube maps currently in the scene.
            /// @return Baked cube maps
            std::vector<std::shared_ptr<BakedCubeMap>>& GetBakedCubeMaps();

            /// @brief Get a baked cube map by its index.
            /// @param index Baked cube map index
            /// @return Found baked cube map, will return nullptr if not valid
            std::shared_ptr<BakedCubeMap> GetBakedCubeMap(int index);


            /// @brief Remove an object from the scene.
            /// @param object Object to remove
            void Remove(std::shared_ptr<SceneObject> object);

            /// @brief Remove a light from the scene.
            /// @param light Light to remove
            void Remove(std::shared_ptr<Light> light);

            /// @brief Remove a baked cube map from the scene
            /// @param cubeMap Baked cube map to remove
            void Remove(std::shared_ptr<BakedCubeMap> cubeMap);

            /// @brief Add an object to the scene.
            /// @param object Object to add
            void Add(std::shared_ptr<SceneObject> object);

            /// @brief Add a light to the scene.
            /// @param light Light to add
            void Add(std::shared_ptr<Light> light);

            /// @brief Add a baked cube map to the scene.
            /// @param cubeMap Baked cube map to add
            void Add(std::shared_ptr<BakedCubeMap> cubeMap);

            /// @brief Call each objects update function.
            void UpdateObjects();

            /// @brief Assign baked cube maps to materials that require them based on their position.
            void UpdateBake();


            /// @brief Find an object by its name.
            /// @param name Object name
            /// @return Found object, will return nullptr if not found
            std::shared_ptr<SceneObject> FindObjectByName(std::string name);

            /// @brief Find all objects with a given name.
            /// @param name Object name
            /// @return Found objects
            std::vector<std::shared_ptr<SceneObject>> FindObjectsByName(std::string name);


            /// @brief Find an item in the scene by its type.
            /// @tparam T Item type
            /// @return Found item, will return nullptr if not found 
            template <typename T>
            std::shared_ptr<T> FindItemByType(){
                for(std::shared_ptr<SceneObject> o : _objects){
                    if(auto cast = std::dynamic_pointer_cast<T>(o)){
                        return cast;
                    }
                }

                for(std::shared_ptr<Light> l : _lights){
                    if(auto cast = std::dynamic_pointer_cast<T>(l)){
                        return cast;
                    }
                }  

                for(std::shared_ptr<BakedCubeMap> c : _bakedCubeMaps){
                    if(auto cast = std::dynamic_pointer_cast<T>(c)){
                        return cast;
                    }
                }

                return nullptr;              
            }


            /// @brief Find all items in a scene by their type.
            /// @tparam T Item type
            /// @return Found items
            template <typename T>
            std::vector<std::shared_ptr<T>> FindItemsByType(){
                std::vector<std::shared_ptr<T>> result;

                for(std::shared_ptr<SceneObject> o : _objects){
                    if(auto cast = std::dynamic_pointer_cast<T>(o)){
                        result.push_back(cast);
                    }
                }

                if(!result.empty()) return result;

                for(std::shared_ptr<Light> l : _lights){
                    if(auto cast = std::dynamic_pointer_cast<T>(l)){
                        result.push_back(cast);
                    }
                } 

                return result;
            }


            /// @brief Serialize data to JSON format.
            /// @return Serialized data
            json ToJson();

            /// @brief Deserialize data from JSON format.
            /// @param data Scene data in JSON format
            /// @return Deserialized Scene
            static std::shared_ptr<Scene> FromJson(const json& data);

            /// @brief Import scene data from a JSON file.
            /// @param filePath JSON file path
            /// @return Deserialized Scene
            static std::shared_ptr<Scene> ImportFromFile(std::filesystem::path filePath);
    };
    
}

#endif //SCENE_HPP