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

#include <GLEP/core/scene.hpp>

namespace GLEP {

    Scene::Scene(){}

    Scene::~Scene(){}

    SceneLightData Scene::GetLightData(){ return _lightData; }

    void Scene::Remove(std::shared_ptr<BakedCubeMap> cubeMap){
        _bakedCubeMaps.erase(std::remove(_bakedCubeMaps.begin(), _bakedCubeMaps.end(), cubeMap), _bakedCubeMaps.end());
    }

    void Scene::Remove(std::shared_ptr<SceneObject> object){
        _objects.erase(std::remove(_objects.begin(), _objects.end(), object), _objects.end());
    }

    void Scene::Remove(std::shared_ptr<Light> light){
        switch(light->GetType()){
            case LightType::AMBIENT:
                _lightData.AmbientLight = false;
                break;
            
            case LightType::DIRECTION:
                _lightData.DirectionalLight = false;
                break;

            case LightType::POINT:
                _lightData.PointLightsAmt--;
                break;

            case LightType::SPOT:
                _lightData.SpotLightsAmt--;
                break;
        }
        _lights.erase(std::remove(_lights.begin(), _lights.end(), light), _lights.end());
    }

    void Scene::Add(std::shared_ptr<BakedCubeMap> cubeMap){
        _bakedCubeMaps.push_back(cubeMap);
    }

    void Scene::Add(std::shared_ptr<SceneObject> object){
        _objects.push_back(object);
    }

    void Scene::Add(std::shared_ptr<Light> light){
        switch(light->GetType()){
            case LightType::AMBIENT:
                _lightData.AmbientLight = true;
                break;
            
            case LightType::DIRECTION:
                _lightData.DirectionalLight = true;
                break;

            case LightType::POINT:
                _lightData.PointLightsAmt++;
                break;

            case LightType::SPOT:
                _lightData.SpotLightsAmt++;
                break;
        }

        _lights.push_back(light);
    }

    void Scene::UpdateObjects(){
        for(std::shared_ptr<SceneObject> o : _objects){
            o->Update();
        }
    }

    std::shared_ptr<CubeMap> Scene::findClosestCubeMap(glm::vec3 position){
        if(_bakedCubeMaps.empty()) return nullptr;

        std::shared_ptr<BakedCubeMap> closest = _bakedCubeMaps[0];
        float closestDist = glm::distance(position, closest->GetPosition());

        for(int i = 1; i < _bakedCubeMaps.size(); i++){
            float dist = glm::distance(position, _bakedCubeMaps[i]->GetPosition());
        
            if(dist < closestDist){
                closest = _bakedCubeMaps[i];
                closestDist = dist;
            }
        }

        return std::dynamic_pointer_cast<CubeMap>(closest);
    }

    void Scene::UpdateBake(){
        if(_bakedCubeMaps.empty()) return;

        for(std::shared_ptr<SceneObject> o : _objects){
            std::shared_ptr<Model> model = std::dynamic_pointer_cast<Model>(o);
            if(!model) continue;

            for(std::shared_ptr<Mesh> mesh : model->GetMeshes()){
                std::shared_ptr<Material> material = mesh->MaterialData;
                if(material->BakeRequired){
                    auto closest = findClosestCubeMap(model->GetWorldPosition());
                    material->SetUniformValue<std::shared_ptr<CubeMap>>("cubeMap", closest);
                }
            }
        }
    }

    std::vector<std::shared_ptr<SceneObject>>& Scene::GetObjects(){
        return _objects;
    }

    std::shared_ptr<SceneObject> Scene::GetObject(int index){
        if(index < 0 || index >= _objects.size()) return nullptr;
        return _objects[index];
    }

    std::vector<std::shared_ptr<Light>>& Scene::GetLights(){
        return _lights;
    }

    std::shared_ptr<Light> Scene::GetLight(int index){
        if(index < 0 || index >= _lights.size()) return nullptr;
        return _lights[index];
    }

    std::vector<std::shared_ptr<BakedCubeMap>>& Scene::GetBakedCubeMaps(){
        return _bakedCubeMaps;
    }

    std::shared_ptr<BakedCubeMap> Scene::GetBakedCubeMap(int index){
        if(index < 0 || index >= _bakedCubeMaps.size()) return nullptr;
        return _bakedCubeMaps[index];
    }

    std::shared_ptr<DirectionalLight> Scene::GetDirectionalLight(){
        for(std::shared_ptr<Light> l : _lights){
            if(auto dirLight = std::dynamic_pointer_cast<DirectionalLight>(l)){
                return dirLight;
            }
        }

        return nullptr;
    }

    std::shared_ptr<SceneObject> Scene::FindObjectByName(std::string name){
        for(std::shared_ptr<SceneObject> o : _objects){
            if(o->Name == name) return o;
        }

        return nullptr;
    }

    std::vector<std::shared_ptr<SceneObject>> Scene::FindObjectsByName(std::string name){
        std::vector<std::shared_ptr<SceneObject>> result;

        for(std::shared_ptr<SceneObject> o : _objects){
            if(o->Name == name) result.push_back(o);
        }

        return result;
    }

    json Scene::ToJson(){
        json j;

        j["objects"] = json::array();
        for(auto& o : _objects){
            j["objects"].push_back(o->ToJson());
        }

        j["lights"] = json::array();
        for(auto& l : _lights){
            j["lights"].push_back(l->ToJson());
        }

        j["baked_cube_maps"] = json::array();
        for(auto& b : _bakedCubeMaps){
            j["baked_cube_maps"].push_back(b->ToJson());
        }

        j["pass_composer"] = json();
        if(PassComposer){
            j["pass_composer"] = PassComposer->ToJson();
        }

        j["skybox"] = json();
        if(Skybox){
            j["skybox"] = Skybox->ToJson();
        }

        return j;
    }

    std::shared_ptr<Scene> Scene::FromJson(const json& data){
        std::shared_ptr<Scene> result = std::make_shared<Scene>();

        json objectsData = data["objects"];
        json lightsData = data["lights"];
        json bakedCubeMapsData = data["baked_cube_maps"];
        json passComposerData = data["pass_composer"];
        json skyboxData = data["skybox"];

        if(!objectsData.is_array() || !lightsData.is_array() || !bakedCubeMapsData.is_array()){
            Print(PrintCode::ERROR, "SCENE", "objects, lights, and baked_cube_maps must all be arrays");
            return nullptr;
        }

        for(int i = 0; i < objectsData.size(); i++){
            json object = objectsData[i];
            if(object["type"] == "model" || object["type"] == "import_model" || object["type"] == "import_geometry_model"){
                std::shared_ptr<Model> model = Model::FromJson(object);
                SceneObject::ApplyFromJson(model, object["object_data"]);
                result->Add(model);
            } else {
                result->Add(SceneObject::FromJson(object));
            }
        }

        for(int i = 0; i < lightsData.size(); i++){
            result->Add(Light::FromJson(lightsData[i]));
        }

        for(int i = 0; i < bakedCubeMapsData.size(); i++){
            result->Add(BakedCubeMap::FromJson(bakedCubeMapsData[i]));
        }

        if(!passComposerData.is_null())
            result->PassComposer = BufferPassComposer::FromJson(passComposerData);

        if(!skyboxData.is_null())
            result->Skybox = Mesh::FromJson(skyboxData);

        return result;
    }

    std::shared_ptr<Scene> Scene::ImportFromFile(std::filesystem::path filePath){
        if(!std::filesystem::exists(filePath) || filePath.extension() != ".json"){
            Print(PrintCode::ERROR, "SCENE", "Invalid file path to import: " + filePath.string());
            return nullptr;
        }

        Print(PrintCode::INFO, "SCENE", "Importing scene from file...");

        std::ifstream f(filePath);
        json j = json::parse(f);

        std::shared_ptr<Scene> result = FromJson(j);

        if(!result){
            Print(PrintCode::ERROR, "SCENE", "Failed to import scene from file at: " + filePath.string());
        } else {
            Print(PrintCode::INFO, "SCENE", "Successfully imported scene from file!");
        }

        return result;
    }

    
}