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

#include <GLEP/core/buffer_pass.hpp>

namespace GLEP {
    
    BufferPass::BufferPass(std::shared_ptr<Material> material){
        std::shared_ptr<Geometry> geometry = std::make_shared<PlaneGeometry>(2.0f, 2.0f);
        _mesh = std::make_shared<Mesh>(geometry, material);
    }

    void BufferPass::InitializeBuffer(glm::vec2 resolution){
        _framebuffer = std::make_shared<Framebuffer>(resolution);
        _mesh->MaterialData->AddUniform<std::shared_ptr<Framebuffer>>("framebuffer", _framebuffer);
    }

    std::shared_ptr<Framebuffer> BufferPass::GetFramebuffer(){ return _framebuffer; }
    std::shared_ptr<Material> BufferPass::GetMaterial(){ return _mesh->MaterialData; }
    std::string BufferPass::GetName(){ return _name; }

    void BufferPass::Bind(){
        _framebuffer->Bind();
    }

    void BufferPass::Render() {
        _mesh->MaterialData->Use();

        _mesh->MaterialData->SetUniform("time", Time::GetElapsedTimeF());
        _mesh->MaterialData->SetUniform("deltaTime", Time::GetDeltaTimeF());

        _mesh->GeometryData->Draw();
    }

    json BufferPass::ToJson(){
        json j;
        j["type"] = _name;
        j["material_data"] = GetMaterial()->ToJson();
        return j;
    }

    std::shared_ptr<BufferPass> BufferPass::FromJson(const json& data){
        std::shared_ptr<BufferPass> result;

        std::string type = data["type"];
        if(type == "buffer_pass"){
            result = std::make_shared<BufferPass>(Material::FromJson(data["material_data"]));
        } else if (type == "render_pass"){
            result = RenderPass::FromJson(data["material_data"]);
        } else if (type == "depth_pass"){
            result = DepthPass::FromJson(data["material_data"]);
        } else if (type == "kernel_pass"){
            result = KernelPass::FromJson(data["material_data"]);
        } else if (type == "grain_pass"){
            result = GrainPass::FromJson(data["material_data"]);
        } else if (type == "fog_pass"){
            result = FogPass::FromJson(data["material_data"]);
        } else {
            Print(PrintCode::ERROR, "BUFFER_PASS", "Unknown Buffer Pass type: " + type);
        }

        return result;
    }

    RenderPass::RenderPass()
    : BufferPass(
        std::make_shared<Material>(
            File::GLEP_SHADERS_PATH / "post" / "defaultPass.vs",
            File::GLEP_SHADERS_PATH / "post" / "renderPass.fs"
        )
    ){
        _name = "render_pass";
    }

    std::shared_ptr<RenderPass> RenderPass::FromJson(const json& data){
        return std::make_shared<RenderPass>();
    }

    DepthPass::DepthPass()
    : BufferPass(
        std::make_shared<Material>(
            File::GLEP_SHADERS_PATH / "post" / "defaultPass.vs",
            File::GLEP_SHADERS_PATH / "post" / "depthPass.fs"
        )
    ){
        _name = "depth_pass";
    }

    std::shared_ptr<DepthPass> DepthPass::FromJson(const json& data){
        return std::make_shared<DepthPass>();
    }

    KernelPass::KernelPass(const float kernel[9], float offset)
    : BufferPass(
        std::make_shared<Material>(
            File::GLEP_SHADERS_PATH / "post" / "defaultPass.vs",
            File::GLEP_SHADERS_PATH / "post" / "kernelPass.fs"
        )
    ){
        _name = "kernel_pass";

        for(int i = 0; i < 9; i++){
            GetMaterial()->AddUniform<float>("kernel[" + std::to_string(i) + "]", kernel[i]);
        }
        
        GetMaterial()->AddUniform<float>("offset", offset);
    }

    std::shared_ptr<KernelPass> KernelPass::FromJson(const json& data){
        float kernel[9];
        for(int i = 0; i < 9; i++){
            std::string name = "uMaterial.kernel[" + std::to_string(i) + "]";
            kernel[i] = data["uniforms"][name];
        }
        return std::make_shared<KernelPass>(kernel, data["uniforms"]["uMaterial.offset"]);
    }

    const float KernelPass::SHARPEN[9] = {
        -1, -1, -1,
        -1, 9, -1,
        -1, -1, -1
    };

    const float KernelPass::BLUR[9] = {
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    };

    const float KernelPass::EDGE[9] = {
        1, 1, 1,
        1, -8, 1,
        1, 1, 1
    };

    GrainPass::GrainPass(glm::vec2 uvScale, float intensity)
    : BufferPass(
        std::make_shared<Material>(
            File::GLEP_SHADERS_PATH / "post"/ "defaultPass.vs",
            File::GLEP_SHADERS_PATH / "post" / "grainPass.fs"
        )
    ){
        _name = "grain_pass";
        GetMaterial()->AddUniform<glm::vec2>("uvScale", uvScale);
        GetMaterial()->AddUniform<float>("intensity", intensity);
    }

    std::shared_ptr<GrainPass> GrainPass::FromJson(const json& data){
        return std::make_shared<GrainPass>(
            Math::Vec2FromJson(data["uniforms"]["uMaterial.uvScale"]),
            data["uniforms"]["uMaterial.intensity"]
        );
    }

    FogPass::FogPass(float near, float far, Color fogColor)
    : BufferPass(
        std::make_shared<Material>(
            File::GLEP_SHADERS_PATH / "post" / "defaultPass.vs",
            File::GLEP_SHADERS_PATH / "post" / "fogPass.fs"
        )
    ){
        _name = "fog_pass";

        GetMaterial()->AddUniform<float>("nearPlane", near);
        GetMaterial()->AddUniform<float>("farPlane", far);
        GetMaterial()->AddUniform<Color>("fogColor", fogColor);
    }

    std::shared_ptr<FogPass> FogPass::FromJson(const json& data){
        return std::make_shared<FogPass>(
            data["uniforms"]["uMaterial.nearPlane"],
            data["uniforms"]["uMaterial.farPlane"],
            Color::FromJson(data["uniforms"]["uMaterial.fogColor"])
        );
    }

    BufferPassComposer::BufferPassComposer(glm::vec2 resolution){
        _resolution = resolution;

        initialize();
    }

    void BufferPassComposer::initialize(){
        _objectPass = std::make_shared<RenderPass>();
        _objectPass->InitializeBuffer(_resolution);
        _skyboxPass = std::make_shared<RenderPass>();
        _skyboxPass->InitializeBuffer(_resolution);
        _renderPass = std::make_shared<RenderPass>();
        _renderPass->InitializeBuffer(_resolution);
    }

    std::shared_ptr<FogPass> BufferPassComposer::GetFogPass(){ return _fogPass;}
    std::vector<std::shared_ptr<BufferPass>> BufferPassComposer::GetBufferPasses(){ return _bufferPasses; }
    glm::vec2 BufferPassComposer::GetResolution(){ return _resolution; }

    void BufferPassComposer::SetResolution(glm::vec2 resolution){
        _resolution = resolution;

        _objectPass->InitializeBuffer(_resolution);
        _skyboxPass->InitializeBuffer(_resolution);
        _renderPass->InitializeBuffer(_resolution);

        if(_fogPass){
            _fogPass->InitializeBuffer(_resolution);
        }

        for(std::shared_ptr<BufferPass> p : _bufferPasses){
            p->InitializeBuffer(_resolution);
        }
    }

    void BufferPassComposer::Add(std::shared_ptr<BufferPass> pass){
        pass->InitializeBuffer(_resolution);
        _bufferPasses.push_back(pass);
    }

    void BufferPassComposer::Add(std::shared_ptr<FogPass> fogPass){
        fogPass->InitializeBuffer(_resolution);
        _fogPass = fogPass;
    }

    void BufferPassComposer::NewFrame(){
        if(_fogPass){
            _objectPass->Bind();
        } else {
            _renderPass->Bind();
        }
    }

    void BufferPassComposer::PreSkybox(){
        if(_fogPass){
            _skyboxPass->Bind();
        }
    }

    void BufferPassComposer::Render(){
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);

        glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(_fogPass){
            _fogPass->Bind();
            _fogPass->GetFramebuffer()->SetDepthBufferID(_objectPass->GetFramebuffer()->GetDepthBufferID());

            _objectPass->Render();

            _renderPass->Bind();
            _renderPass->GetFramebuffer()->SetDepthBufferID(_objectPass->GetFramebuffer()->GetDepthBufferID());

            _skyboxPass->Render();
            _fogPass->Render();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        if(_bufferPasses.size() > 0){
            _bufferPasses[0]->Bind();
            _bufferPasses[0]->GetFramebuffer()->SetDepthBufferID(_renderPass->GetFramebuffer()->GetDepthBufferID());
        }

        _renderPass->Render();
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        for(int i = 0; i < _bufferPasses.size(); i++){
            if(i < _bufferPasses.size() - 1){
                _bufferPasses[i+1]->Bind();
                _bufferPasses[i+1]->GetFramebuffer()->SetDepthBufferID(_renderPass->GetFramebuffer()->GetDepthBufferID());
            } 

            _bufferPasses[i]->Render();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        glEnable(GL_DEPTH_TEST);

    }

    json BufferPassComposer::ToJson(){
        json j;

        j["resolution"] = Math::ToJson(_resolution);

        j["buffer_passes"] = json::array();
        for(auto& p : _bufferPasses){
            j["buffer_passes"].push_back(p->ToJson());
        }

        j["fog_pass"] = json();
        if(_fogPass)
            j["fog_pass"] = _fogPass->ToJson();

        return j;
    }

    std::shared_ptr<BufferPassComposer> BufferPassComposer::FromJson(const json& data){
        std::shared_ptr<BufferPassComposer> result = std::make_shared<BufferPassComposer>(glm::vec2(data["resolution"]["x"], data["resolution"]["y"]));

        if(data["buffer_passes"].is_array()){
            for(int i = 0; i < data["buffer_passes"].size(); i++){
                result->Add(BufferPass::FromJson(data["buffer_passes"][i]));
            }
        }

        if(!data["fog_pass"].is_null())
            result->Add(FogPass::FromJson(data["fog_pass"]["material_data"]));

        return result;
    }
}