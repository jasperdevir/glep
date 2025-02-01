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

#include <GLEP/core/scene_object.hpp>

namespace GLEP {
    
    SceneObject::SceneObject(){
        _worldPosition = glm::vec3(0.0f);
        _worldRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        _worldScale = glm::vec3(1.0f);

        _modelMatrix = glm::mat4(1.0f);

        Position = _worldPosition;
        Rotation = _worldRotation;
        Scale = _worldScale;
    }

    void SceneObject::Add(std::unique_ptr<ObjectComponent> component){
        component->Initialize(shared_from_this());
        _components.push_back(std::move(component));
    }

    void SceneObject::Add(std::shared_ptr<SceneObject> object){
        object->Parent = shared_from_this();
        _children.push_back(object);
    }

    void SceneObject::Update(){
        UpdateTransformVectors();
        
        for(std::unique_ptr<ObjectComponent>& c : _components){
            if(_firstUpdate){
                c->Start();
                _firstUpdate = false;
            } else {
                c->Update();
            }
        }
        


    }

    void SceneObject::UpdateTransformVectors(){
        if (Parent) {
            _worldPosition = Parent->GetWorldPosition() + Position; 
            _worldRotation = Parent->GetWorldRotation() * Rotation; 
            _worldScale = Parent->GetWorldScale() * Scale;           
        } else {
            _worldPosition = Position;
            _worldRotation = Rotation;
            _worldScale = Scale;
        }
    }

    void SceneObject::updateModelMatrix(){
        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), _worldScale);
        glm::mat4 rotationMat = glm::mat4_cast(_worldRotation);  
        glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), _worldPosition);
        
        _modelMatrix = translateMat * rotationMat * scaleMat;
    }

    std::vector<std::unique_ptr<ObjectComponent>>& SceneObject::GetComponents(){
        return _components;
    }

    std::vector<std::shared_ptr<SceneObject>>& SceneObject::GetChildren(){
        return _children;
    }

    glm::mat4 SceneObject::GetModelMatrix(){
        updateModelMatrix();
        return _modelMatrix;
    }

    glm::vec3 SceneObject::GetWorldPosition(){
        UpdateTransformVectors();
        return _worldPosition;
    }

    glm::quat SceneObject::GetWorldRotation(){
        UpdateTransformVectors();
        return _worldRotation;
    }

    glm::vec3 SceneObject::GetWorldScale(){
        UpdateTransformVectors();
        return _worldScale;
    }

    json SceneObject::ToJson(){
        json j;

        j["name"] = Name;
        j["position"] = Math::ToJson(Position);
        j["rotation"] = Math::ToJson(Rotation);
        j["scale"] = Math::ToJson(Scale);

        return j;
    }

    //INCOMPLETE
    std::shared_ptr<SceneObject> SceneObject::FromJson(const json& data){
        std::shared_ptr<SceneObject> result = std::make_shared<SceneObject>();

        result->Name = data["name"];
        result->Position = Math::Vec3FromJson(data["position"]);
        result->Rotation = Math::QuatFromJson(data["rotation"]);
        result->Scale = Math::Vec3FromJson(data["scale"]);

        return result;
    }

    //INCOMPLETE
    void SceneObject::ApplyFromJson(const std::shared_ptr<SceneObject>& object, const json& data){
        object->Name = data["name"];
        object->Position = Math::Vec3FromJson(data["position"]);
        object->Rotation = Math::QuatFromJson(data["rotation"]);
        object->Scale = Math::Vec3FromJson(data["scale"]);
    }

}