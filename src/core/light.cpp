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

#include <GLEP/core/light.hpp>

namespace GLEP {

    Light::Light(LightType type, Color lightColor, float intensity){
        _type = type;

        LightColor = lightColor;
        Intensity = intensity;
    }

    json Light::ToJson(){
        json j;

        j["light_color"] = LightColor.ToJson();
        j["intensity"] = Intensity;

        return j;
    }

    std::shared_ptr<Light> Light::FromJson(const json& data){
        std::shared_ptr<Light> result;
 
        if(data["type"] == "ambient_light"){
            result = AmbientLight::FromJson(data);
        } else if (data["type"] == "point_light"){
            result = PointLight::FromJson(data);
        } else if (data["type"] == "directional_light"){
            result = DirectionalLight::FromJson(data);
        } else if (data["type"] == "spot_light"){
            result = SpotLight::FromJson(data);
        } else {
            throw std::invalid_argument("[Invalid JSON format]-> Unknown Light type: " + data["type"]);
        }

        return result;
    }

    LightType Light::GetType() { return _type; }
    
    AmbientLight::AmbientLight()
    : Light(LightType::AMBIENT, Color(1.0f), 0.5f){}

    AmbientLight::AmbientLight(Color lightColor, float intensity)
    : Light(LightType::AMBIENT, lightColor, intensity){}

    void AmbientLight::Bind(const std::shared_ptr<Material>& material, int index){
        material->SetUniform("uAmbient.color", LightColor);
        material->SetUniform("uAmbient.intensity", Intensity);
    }

    json AmbientLight::ToJson(){
        json j;

        j["type"] = "ambient_light";
        j["light_color"] = LightColor.ToJson();
        j["intensity"] = Intensity;

        return j;
    }

    std::shared_ptr<AmbientLight> AmbientLight::FromJson(const json& data){
        return std::make_shared<AmbientLight>(
            Color::FromJson(data["light_color"]), 
            data["intensity"]
        );
    }

    PointLight::PointLight(glm::vec3 position, Color lightColor, float intensity, float linear, float quadratic)
    : Light(LightType::POINT, lightColor, intensity) {
        Position = position;
        Constant = 1.0f;
        Linear = linear;
        Quadratic = quadratic;
    }

    void PointLight::Bind(const std::shared_ptr<Material>& material, int index){
        material->SetUniform("uPointLights[" + std::to_string(index) + "].position", Position);
        material->SetUniform("uPointLights[" + std::to_string(index) + "].color", LightColor);
        material->SetUniform("uPointLights[" + std::to_string(index) + "].intensity", Intensity);
        material->SetUniform("uPointLights[" + std::to_string(index) + "].constant", Constant);
        material->SetUniform("uPointLights[" + std::to_string(index) + "].linear", Linear);
        material->SetUniform("uPointLights[" + std::to_string(index) + "].quadratic", Quadratic);
    }

    json PointLight::ToJson(){
        json j;

        j["type"] = "point_light";
        j["light_color"] = LightColor.ToJson();
        j["intensity"] = Intensity;
        j["position"] = Math::ToJson(Position);
        j["linear"] = Linear;
        j["quadratic"] = Quadratic;

        return j;
    }

    std::shared_ptr<PointLight> PointLight::FromJson(const json& data){
        return std::make_shared<PointLight>(
            Math::Vec3FromJson(data["position"]),
            Color::FromJson(data["light_color"]), 
            data["intensity"], 
            data["linear"], 
            data["quadratic"]
        );
    }

    DirectionalLight::DirectionalLight(glm::vec3 direction, Color lightColor, float intensity)
    : Light(LightType::DIRECTION, lightColor, intensity) {
        Direction = direction;
    }

    void DirectionalLight::Bind(const std::shared_ptr<Material>& material, int index){
        material->SetUniform("uDirectionalLight.direction", Direction);
        material->SetUniform("uDirectionalLight.color", LightColor);
        material->SetUniform("uDirectionalLight.intensity", Intensity);
    }

    json DirectionalLight::ToJson(){
        json j;

        j["type"] = "directional_light";
        j["light_color"] = LightColor.ToJson();
        j["intensity"] = Intensity;
        j["direction"]  = Math::ToJson(Direction);

        return j;
    }

    std::shared_ptr<DirectionalLight> DirectionalLight::FromJson(const json& data){
        return std::make_shared<DirectionalLight>(
            Math::Vec3FromJson(data["direction"]),
            Color::FromJson(data["light_color"]), 
            data["intensity"]
        );
    }

    SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, Color lightColor, float intensity, float innerCutOff, float outerCutOff, float linear, float quadratic)
    : Light(LightType::SPOT, lightColor, intensity) {
        Position = position;
        Direction = direction;
        InnerCutOff = glm::cos(glm::radians(innerCutOff));
        OuterCutOff = glm::cos(glm::radians(outerCutOff));
        Constant = 1.0f;
        Linear = linear;
        Quadratic = quadratic;
    }

    void SpotLight::Bind(const std::shared_ptr<Material>& material, int index){
        material->SetUniform("uSpotLights[" + std::to_string(index) + "].position", Position);
        material->SetUniform("uSpotLights[" + std::to_string(index) + "].direction", Direction);
        material->SetUniform("uSpotLights[" + std::to_string(index) + "].color", LightColor);
        material->SetUniform("uSpotLights[" + std::to_string(index) + "].intensity", Intensity);
        material->SetUniform("uSpotLights[" + std::to_string(index) + "].innerCutOff", InnerCutOff);
        material->SetUniform("uSpotLights[" + std::to_string(index) + "].outerCutOff", OuterCutOff);
        material->SetUniform("uSpotLights[" + std::to_string(index) + "].constant", Constant);
        material->SetUniform("uSpotLights[" + std::to_string(index) + "].linear", Linear);
        material->SetUniform("uSpotLights[" + std::to_string(index) + "].quadratic", Quadratic);
    }

    json SpotLight::ToJson(){
        json j;

        j["type"] = "spot_light";
        j["light_color"] = LightColor.ToJson();
        j["intensity"] = Intensity;
        j["position"] = Math::ToJson(Position);
        j["direction"] = Math::ToJson(Direction);
        j["inner_cut_off"] = InnerCutOff;
        j["outer_cut_off"] = OuterCutOff;
        j["constant"] = Constant;
        j["linear"] = Linear;
        j["quadratic"] = Quadratic;

        return j;
    }

    std::shared_ptr<SpotLight> SpotLight::FromJson(const json& data){
        return std::make_shared<SpotLight>(
            Math::Vec3FromJson(data["position"]),
            Math::Vec3FromJson(data["direction"]),
            Color::FromJson(data["light_color"]),
            data["intensity"],
            data["inner_cut_off"],
            data["outer_cut_off"],
            data["linear"], 
            data["quatratic"]
        );
    }
}

