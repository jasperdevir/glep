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

#include <GLEP/core/material.hpp>

namespace GLEP {

    Material::Material() {}
    
    Material::Material(std::shared_ptr<Material> material, bool copyUniforms){
        _name = material->GetName();
        _shader = material->GetShader();
        LightingRequired = material->LightingRequired;
        ReceiveShadows = material->ReceiveShadows;
        CastShadows = material->CastShadows;
        BakeRequired = material->BakeRequired;
        Wireframe = material->Wireframe;
        CullFace = material->CullFace;

        if(copyUniforms)
            _uniforms = material->GetUniforms();
    }

    Material::Material(std::filesystem::path vsFilePath, std::filesystem::path fsFilePath){
        _shader = std::make_shared<Shader>(vsFilePath, fsFilePath);
    }

    Material::Material(std::shared_ptr<Shader> shader){
        _shader = shader;
    }

    Material::~Material(){}

    void Material::Use(){ 
        glPolygonMode(GL_FRONT_AND_BACK, Wireframe ? GL_LINE : GL_FILL);
        if(CullFace == MaterialCull::NONE) glDisable(GL_CULL_FACE);
        else{
            glEnable(GL_CULL_FACE);
            glCullFace((GLenum)CullFace);
        }

        _shader->Use();
        
        for (auto& uniform : _uniforms) {
            uniform->SetUniform(this); 
        }
    }

    std::vector<std::shared_ptr<TypelessShaderUniform>> Material::GetUniforms(){
        return _uniforms;
    }

    GLint Material::GetUniformLocation(const std::string &name){
        return glGetUniformLocation(_shader->GetID(), name.c_str());
    }

    void Material::SetUniform(const std::string &name, bool value){         
        glUniform1i(GetUniformLocation(name), (int)value); 
    }

    void Material::SetUniform(const std::string &name, int value){ 
        glUniform1i(GetUniformLocation(name), value); 
    }

    void Material::SetUniform(const std::string &name, float value){ 
        glUniform1f(GetUniformLocation(name), value); 
    } 

    void Material::SetUniform(const std::string &name, glm::vec2 value){
        glUniform2f(GetUniformLocation(name), value.x, value.y);
    }

    void Material::SetUniform(const std::string &name, glm::vec3 value){
        glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
    }

    void Material::SetUniform(const std::string &name, glm::vec4 value){
        glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
    }

    void Material::SetUniform(const std::string &name, Color value){
        glUniform4f(GetUniformLocation(name), value.r, value.g, value.b, value.a);
    }
    
    void Material::SetUniform(const std::string &name, float* value){
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value);
    }

    template <>
    json ShaderUniform<int>::ToJson(){
        json j;
        j[Name] = Value;
        return j;
    }

    template <>
    json ShaderUniform<float>::ToJson(){
        json j;
        j[Name] = Value;
        return j;
    }

    template <>
    json ShaderUniform<bool>::ToJson(){
        json j;
        j[Name] = Value;
        return j;
    }

    template <>
    json ShaderUniform<glm::vec2>::ToJson(){
        json j;
        j[Name] = Math::ToJson(Value);
        return j;
    }

    template <>
    json ShaderUniform<glm::vec3>::ToJson(){
        json j;
        j[Name] = Math::ToJson(Value);
        return j;
    }

    template <>
    json ShaderUniform<glm::vec4>::ToJson(){
        json j;
        j[Name] = Math::ToJson(Value);
        return j;
    }

    template <>
    json ShaderUniform<Color>::ToJson(){
        json j;
        j[Name] = Value.ToJson();
        return j;
    }

    template <>
    json ShaderUniform<std::shared_ptr<Texture>>::ToJson(){
        json j;
        j[Name] = Value->ToJson();
        return j;
    }

    template <>
    json ShaderUniform<std::shared_ptr<CubeMap>>::ToJson(){
        json j;
        j[Name] = Value->ToJson();
        return j;
    }

    template <>
    json ShaderUniform<std::shared_ptr<TextureCubeMap>>::ToJson(){
        json j;
        j[Name] = Value->ToJson();
        return j;
    }

    json Material::ToJson(){
        json j;

        j["type"] = _name;
        j["wireframe"] = Wireframe;
        j["lighting_required"] = LightingRequired;
        j["bake_required"] = BakeRequired;
        j["cast_shadows"] = CastShadows;
        j["receive_shadows"] = ReceiveShadows;
        j["cull_face"] = CullFace;

        if(_name == "material"){
            j["shader"]["vsPath"] = _shader->GetVsPath();
            j["shader"]["fsPath"] = _shader->GetFsPath();
        }

        j["uniforms"] = json();
        for(auto& u : _uniforms){
            j["uniforms"][u->Name] = u->ToJson()[u->Name];
        }
        return j;
    }

    std::shared_ptr<Material> Material::FromJson(const json& data){
        std::shared_ptr<Material> result;

        std::string type = data["type"];
        if(type == "material"){
            result = std::make_shared<Material>(data["shader"]["vsPath"], data["shader"]["fsPath"]);
        } else if(type == "depth_material"){
            result = DepthMaterial::FromJson(data["uniforms"]);
        } else if(type == "uv_material"){
            result = UVMaterial::FromJson(data["uniforms"]);
        } else if(type == "skybox_material"){
            result = SkyboxMaterial::FromJson(data["uniforms"]);
        } else if(type == "reflection_material"){
            result = ReflectionMaterial::FromJson(data["uniforms"]);
        } else if(type == "refraction_material"){
            result = RefractionMaterial::FromJson(data["uniforms"]);
        } else if(type == "unlit_material"){
            result = UnlitMaterial::FromJson(data["uniforms"]);
        } else if(type == "lambert_material"){
            result = LambertMaterial::FromJson(data["uniforms"]);
        } else if(type == "phong_material"){
            result = PhongMaterial::FromJson(data["uniforms"]);
        } else if(type == "blinn_phong_material"){
            result = BlinnPhongMaterial::FromJson(data["uniforms"]);
        } else {
            Print(PrintCode::ERROR, "IMPORT", "Unrecognised Material type attempted to be imported - Type: " + type);
            return nullptr;
        }

        if(result){
            result->Wireframe = data["wireframe"];
            result->LightingRequired = data["lighting_required"];
            result->BakeRequired = data["bake_required"];
            result->CastShadows = data["cast_shadows"];
            result->ReceiveShadows = data["receive_shadows"];
            result->CullFace = (MaterialCull) data["cull_face"];
        }

        return result;
    }

    void Material::SetUniform(const std::string &name, std::shared_ptr<Texture> value){
        if(value){
            glUniform1i(GetUniformLocation(name), (int)value->GetType()); 
            value->Bind();
        }
    }

    void Material::SetUniform(const std::string &name, std::shared_ptr<TextureMap> value){
        if(value){
            glUniform1i(GetUniformLocation(name + ".diffuseTex"), 0);
            glUniform1i(GetUniformLocation(name + ".specularTex"), 1);
            glUniform1i(GetUniformLocation(name + ".normalTex"), 2);
            glUniform1i(GetUniformLocation(name + ".heightTex"), 3);

            value->Bind();
        }
    }

    void Material::SetUniform(const std::string &name, std::shared_ptr<CubeMap> value){
        if(value){
            glUniform1i(GetUniformLocation(name), 6); 
            value->Bind();
        }
    }

    void Material::SetUniform(const std::string &name, std::shared_ptr<Framebuffer> value){
        if(value){
            glUniform1i(GetUniformLocation(name + ".color"), 4);
            glUniform1i(GetUniformLocation(name + ".depth"), 5);
            value->BindResult();
        }
    }

    TypelessShaderUniform::TypelessShaderUniform(std::string name){
        Name = name;
    }

    std::shared_ptr<Shader> Material::GetShader() { return _shader; }
    std::string Material::GetName() { return _name; }

    DepthMaterial::DepthMaterial(float nearPlane, float farPlane, float resultScale)
    : Material (
        File::GLEP_SHADERS_PATH / "default.vs",
        File::GLEP_SHADERS_PATH / "utility" / "depthBuffer.fs"
    ){
        _name = "depth_material";
        AddUniform<float>("nearPlane", nearPlane);
        AddUniform<float>("farPlane", farPlane);
        AddUniform<float>("resultScale", resultScale);
    }

    std::shared_ptr<DepthMaterial> DepthMaterial::FromJson(const json& data){
        return std::make_shared<DepthMaterial>(
            data["uMaterial.nearPlane"], 
            data["uMaterial.farPlane"],
            data["uMaterial.resultScale"]
        );
    }

    UVMaterial::UVMaterial()
    : Material (
        File::GLEP_SHADERS_PATH / "default.vs",
        File::GLEP_SHADERS_PATH / "utility" / "uv.fs"
    ){
        _name = "uv_material";
    }

    std::shared_ptr<UVMaterial> UVMaterial::FromJson(const json& data){
        return std::make_shared<UVMaterial>();
    }

    SkyboxMaterial::SkyboxMaterial(std::shared_ptr<TextureCubeMap> cubeMap)
    : Material (
        File::GLEP_SHADERS_PATH / "skybox" / "skybox.vs",
        File::GLEP_SHADERS_PATH / "skybox" / "skybox.fs"
    ){
        _name = "skybox_material";
        CullFace = MaterialCull::NONE;
        AddUniform<std::shared_ptr<TextureCubeMap>>("cubeMap", cubeMap);
    }

    std::shared_ptr<SkyboxMaterial> SkyboxMaterial::FromJson(const json& data){
        return std::make_shared<SkyboxMaterial>(
            TextureCubeMap::FromJson(data["uMaterial.cubeMap"])
        );
    }

    ReflectionMaterial::ReflectionMaterial(std::shared_ptr<CubeMap> cubeMap, Color tint)
    : Material (
        File::GLEP_SHADERS_PATH / "default.vs",
        File::GLEP_SHADERS_PATH / "unlit" / "reflection.fs"
    ){
        _name = "reflection_material";
        BakeRequired = cubeMap == nullptr;
        AddUniform<std::shared_ptr<CubeMap>>("cubeMap", cubeMap);
        AddUniform<Color>("tint", tint);
    }

    std::shared_ptr<ReflectionMaterial> ReflectionMaterial::FromJson(const json& data){
        return std::make_shared<ReflectionMaterial>(
            CubeMap::FromJson(data["uMaterial.cubeMap"]),
            Color::FromJson(data["uMaterial.tint"])
        );
    }

    RefractionMaterial::RefractionMaterial(std::shared_ptr<CubeMap> cubeMap, float refractiveIndex, Color tint)
    : Material (
        File::GLEP_SHADERS_PATH / "default.vs",
        File::GLEP_SHADERS_PATH / "unlit" / "refraction.fs"
    ){
        _name = "refraction_material";
        BakeRequired = cubeMap == nullptr;
        AddUniform<std::shared_ptr<CubeMap>>("cubeMap", cubeMap);
        AddUniform<float>("refractiveIndex", refractiveIndex);
        AddUniform<Color>("tint", tint);
    }

    std::shared_ptr<RefractionMaterial> RefractionMaterial::FromJson(const json& data){
        return std::make_shared<RefractionMaterial>(
            CubeMap::FromJson(data["uMaterial.cubeMap"]),
            data["uMaterial.refractiveIndex"],
            Color::FromJson(data["uMaterial.tint"])
        );
    }

    UnlitMaterial::UnlitMaterial(Color diffuse) :
    Material (
        File::GLEP_SHADERS_PATH / "default.vs",
        File::GLEP_SHADERS_PATH / "unlit" / "unlit.fs"
    ){
        _name = "unlit_material";
        AddUniform<int>("type", 1);
        AddUniform<Color>("colorDiffuse", diffuse);
    }

    UnlitMaterial::UnlitMaterial(std::shared_ptr<Texture> diffuse) :
    Material (
        File::GLEP_SHADERS_PATH / "default.vs",
        File::GLEP_SHADERS_PATH / "unlit" / "unlit.fs"
    ){
        _name = "unlit_material";
        AddUniform<int>("type", 2);
        AddUniform<std::shared_ptr<Texture>>("texDiffuse", diffuse);
    }

    std::shared_ptr<UnlitMaterial> UnlitMaterial::FromJson(const json& data){
        if(data["uMaterial.type"] == 1){
            return std::make_shared<UnlitMaterial>(
                Color::FromJson(data["uMaterial.colorDiffuse"])
            );
        } else if (data["uMaterial.type"] == 2){
            return std::make_shared<UnlitMaterial>(
                Texture::FromJson(data["uMaterial.texDiffuse"])
            );
        } else {
            Print(PrintCode::ERROR, "MATERIAL", "Unknown unlit_material type: " + data["type"]);
            return nullptr; 
        }
    }

    LambertMaterial::LambertMaterial(std::shared_ptr<Texture> diffuse) :
    Material (
        File::GLEP_SHADERS_PATH / "default.vs",
        File::GLEP_SHADERS_PATH / "lit" / "lambert.fs"
    ){
        _name = "lambert_material";
        AddUniform<int>("type", 2);
        AddUniform<std::shared_ptr<Texture>>("diffuseTex", diffuse);
        LightingRequired = true;
    }

    LambertMaterial::LambertMaterial(Color diffuse) :
    Material (
        File::GLEP_SHADERS_PATH / "default.vs",
        File::GLEP_SHADERS_PATH / "lit" / "lambert.fs"
    ){
        _name = "lambert_material";
        AddUniform<int>("type", 1);
        AddUniform<Color>("diffuseColor", diffuse);
        LightingRequired = true;
    }

    std::shared_ptr<LambertMaterial> LambertMaterial::FromJson(const json& data){
        if(data["uMaterial.type"] == 1){
            return std::make_shared<LambertMaterial>(
                Color::FromJson(data["uMaterial.diffuseColor"])
            );
        } else if (data["uMaterial.type"] == 2){
            return std::make_shared<LambertMaterial>(
                Texture::FromJson(data["uMaterial.diffuseTex"])
            );
        } else {
            Print(PrintCode::ERROR, "MATERIAL", "Unknown lambert_material type: " + data["type"]);
            return nullptr;
        }
    }

    PhongMaterial::PhongMaterial(Color diffuse, Color specular, float shininess):
    Material(
        File::GLEP_SHADERS_PATH / "default.vs", 
        File::GLEP_SHADERS_PATH / "lit" / "phong.fs"
    ){
        _name = "phong_material";
        AddUniform<int>("type", 1);
        AddUniform<Color>("diffuseColor", diffuse);
        AddUniform<Color>("specularColor", diffuse);
        AddUniform<float>("shininess", shininess);
        LightingRequired = true;
    }

    PhongMaterial::PhongMaterial(std::shared_ptr<Texture> diffuse, Color specular, float shininess):
    Material(
        File::GLEP_SHADERS_PATH / "default.vs", 
        File::GLEP_SHADERS_PATH / "lit" / "phong.fs"
    ){
        _name = "phong_material";
        AddUniform<int>("type", 2);
        AddUniform<std::shared_ptr<Texture>>("diffuseTex", diffuse);
        AddUniform<Color>("specularColor", specular);
        AddUniform<float>("shininess", shininess);
        LightingRequired = true;
    }

    PhongMaterial::PhongMaterial(std::shared_ptr<Texture> diffuse, std::shared_ptr<Texture> specular, float shininess):
    Material(
        File::GLEP_SHADERS_PATH / "default.vs", 
        File::GLEP_SHADERS_PATH / "lit" / "phong.fs"
    ){
        _name = "phong_material";
        AddUniform<int>("type", 3);
        AddUniform<std::shared_ptr<Texture>>("diffuseTex", diffuse);
        AddUniform<std::shared_ptr<Texture>>("specularTex", specular);
        AddUniform<float>("shininess", shininess);
        LightingRequired = true;
    }

    std::shared_ptr<PhongMaterial> PhongMaterial::FromJson(const json& data){
        if(data["uMaterial.type"] == 1){
            return std::make_shared<PhongMaterial>(
                Color::FromJson(data["uMaterial.diffuseColor"]),
                Color::FromJson(data["uMaterial.specularColor"]),
                data["uMaterial.shininess"]
            );
        } else if (data["uMaterial.type"] == 2){
            return std::make_shared<PhongMaterial>(
                Texture::FromJson(data["uMaterial.diffuseTex"]),
                Color::FromJson(data["uMaterial.specularColor"]),
                data["uMaterial.shininess"]
            );
        } else if (data["uMaterial.type"] == 3){
            return std::make_shared<PhongMaterial>(
                Texture::FromJson(data["uMaterial.diffuseTex"]),
                Texture::FromJson(data["uMaterial.specularTex"]),
                data["uMaterial.shininess"]
            );
        } else {
            Print(PrintCode::ERROR, "MATERIAL", "Unknown phong_material type: " + data["type"]);
            return nullptr;
        }
    }

    BlinnPhongMaterial::BlinnPhongMaterial(Color diffuse, Color specular, float shininess):
    Material(
        File::GLEP_SHADERS_PATH / "default.vs", 
        File::GLEP_SHADERS_PATH / "lit" / "blinnPhong.fs"
    ){
        _name = "blinn_phong_material";
        AddUniform<int>("type", 1);
        AddUniform<Color>("diffuseColor", diffuse);
        AddUniform<Color>("specularColor", specular);
        AddUniform<float>("shininess", shininess);
        LightingRequired = true;
    }

    BlinnPhongMaterial::BlinnPhongMaterial(std::shared_ptr<Texture> diffuse, Color specular, float shininess):
    Material(
        File::GLEP_SHADERS_PATH / "default.vs", 
        File::GLEP_SHADERS_PATH / "lit" / "blinnPhong.fs"
    ){
        _name = "blinn_phong_material";
        AddUniform<int>("type", 2);
        AddUniform<std::shared_ptr<Texture>>("diffuseTex", diffuse);
        AddUniform<Color>("specularColor", specular);
        AddUniform<float>("shininess", shininess);
        LightingRequired = true;
    }

    BlinnPhongMaterial::BlinnPhongMaterial(std::shared_ptr<Texture> diffuse, std::shared_ptr<Texture> specular, float shininess):
    Material(
        File::GLEP_SHADERS_PATH / "default.vs", 
        File::GLEP_SHADERS_PATH / "lit" / "blinnPhong.fs"
    ){
        _name = "blinn_phong_material";
        AddUniform<int>("type", 3);
        AddUniform<std::shared_ptr<Texture>>("diffuseTex", diffuse);
        AddUniform<std::shared_ptr<Texture>>("specularTex", specular);
        AddUniform<float>("shininess", shininess);
        LightingRequired = true;
    }

    std::shared_ptr<BlinnPhongMaterial> BlinnPhongMaterial::FromJson(const json& data){
        if(data["uMaterial.type"] == 1){
            return std::make_shared<BlinnPhongMaterial>(
                Color::FromJson(data["uMaterial.diffuseColor"]),
                Color::FromJson(data["uMaterial.specularColor"]),
                data["uMaterial.shininess"]
            );
        } else if (data["uMaterial.type"] == 2){
            return std::make_shared<BlinnPhongMaterial>(
                Texture::FromJson(data["uMaterial.diffuseTex"]),
                Color::FromJson(data["uMaterial.specularColor"]),
                data["uMaterial.shininess"]
            );
        } else if (data["uMaterial.type"] == 3){
            return std::make_shared<BlinnPhongMaterial>(
                Texture::FromJson(data["uMaterial.diffuseTex"]),
                Texture::FromJson(data["uMaterial.specularTex"]),
                data["uMaterial.shininess"]
            );
        } else {
            Print(PrintCode::ERROR, "MATERIAL", "Unknown blinn_phong_material type: " + data["type"]);
            return nullptr;
        }
    }
}