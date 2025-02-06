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

#include <GLEP/core/renderer.hpp>

namespace GLEP{

    Renderer::Renderer(std::shared_ptr<Window> window){
        TargetWindow = window;

        TargetWindow->Initialize();
        initializeDefaults();
    }

    Renderer::Renderer(std::shared_ptr<Window> window, std::shared_ptr<Camera> camera){
        TargetWindow = window;
        TargetCamera = camera;

        TargetWindow->Initialize();
        initializeDefaults();
    }

    void Renderer::initializeDefaults(){
        glEnable(GL_DEPTH_TEST);  

        glEnable(GL_MULTISAMPLE);  

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glFrontFace(GL_CCW); 

        _dirShadowMapBuffer = std::make_shared<Framebuffer>(glm::vec2(1024)); 
        _dirShadowMapCamera = std::make_shared<OrthographicCamera>(10.0f, 1.0f, 0.01f, 10.0f);

        _pointShadowCubeMap = std::make_shared<ShadowCubeMap>(glm::vec3(0.0f), 1042);
        std::shared_ptr<Shader> pointShadowShader = std::make_shared<Shader>(File::GLEP_SHADERS_PATH / "pointShadow.vs", File::GLEP_SHADERS_PATH / "pointShadow.gs", File::GLEP_SHADERS_PATH / "pointShadow.fs");
        _pointShadowMapMaterial = std::make_shared<Material>(pointShadowShader);

        std::shared_ptr<Geometry> db_lightGeometry = std::make_shared<CubeGeometry>(0.25f, 0.25f, 0.25f);
        std::shared_ptr<Material> db_lightMaterial = std::make_shared<UnlitMaterial>(Color(1.0f));
        _DB_lightMesh = std::make_shared<Mesh>(db_lightGeometry, db_lightMaterial);

        std::shared_ptr<Geometry> db_normalDirGeometry = std::make_shared<LineGeometry>(glm::vec3(0.0f), glm::vec3(1.0f));
        std::shared_ptr<Material> db_normalDirMaterial = std::make_shared<UnlitMaterial>(Color(1.0f));
        _DB_normalDirMesh = std::make_shared<Mesh>(db_normalDirGeometry, db_normalDirMaterial);

        Print(PrintCode::INFO, "RENDERER", "Renderer successfully initialized - OpenGL version " + std::to_string(GL_MAJ_VERSION) + std::to_string(GL_MIN_VERSION) + "0");

        initializeGui();
    }

    Renderer::~Renderer(){
        if(!_isGuiShutdown && _isGuiInitalized){
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            _isGuiShutdown = true;
            Print(PrintCode::INFO, "RENDERER", "ImGui successfully shutdown");
        }
        
        glfwTerminate();

        Print(PrintCode::INFO, "RENDERER", "Renderer successfully shutdown");
    }

    void Renderer::SetWindowState(WindowState state){
        TargetWindow->SetState(state);
        glm::vec2 resolution = TargetWindow->GetResolution();

        TargetCamera->SetAspectRatio(resolution.x / resolution.y);
    }

    bool Renderer::IsRunning(){        
        return !TargetWindow->GetShouldClose();
    }

    void Renderer::initializeGui(){
        if(!_isGuiInitalized){
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO io = ImGui::GetIO(); (void)io;
            ImGui::StyleColorsDark();
            ImGui_ImplGlfw_InitForOpenGL(TargetWindow->GetGLFWWindow(), true); 

            std::string glMaj = std::to_string(GL_MAJ_VERSION);
            std::string glMin = std::to_string(GL_MIN_VERSION);
            std::string glVersion = "#version " + glMaj + glMin + "0";

            ImGui_ImplOpenGL3_Init(glVersion.c_str());  

            _isGuiInitalized = true; 

            Print(PrintCode::INFO, "RENDERER", "ImGui successfully initialized");
        }
    }

    ImGuiIO& Renderer::GetGuiIO(){
        return ImGui::GetIO();
    }

    std::shared_ptr<Camera> Renderer::GetShadowMapCamera(){ return _dirShadowMapCamera;}
    std::shared_ptr<Framebuffer> Renderer::GetShadowMapBuffer(){ return _dirShadowMapBuffer; }

    void Renderer::SetViewport(int x, int y, int width, int height){
        glViewport(x,y,width,height);
    }

    void Renderer::ResetViewport(){
        glm::vec2 resolution = TargetWindow->GetResolution();
        glViewport(0,0, (int)resolution.x, (int)resolution.y);
    }

    void Renderer::renderSceneObjects(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera, RenderType type){
        glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projectionMatrix = camera->GetProjectionMatrix();
        glm::mat4 viewMatrix = camera->GetViewMatrix();
        glm::vec3 cameraPos = camera->GetWorldPosition();

        for(std::shared_ptr<SceneObject> object : scene->GetObjects()){
            std::shared_ptr<Model> model = std::dynamic_pointer_cast<Model>(object);

            if(!model) continue;

            glm::mat4 modelMatrix = model->GetModelMatrix();
            for(std::shared_ptr<Mesh> m : model->GetMeshes()){
                if(type == RenderType::BAKE && m->MaterialData->BakeRequired)
                    continue;

                if(type == RenderType::SHADOW_MAP && !m->MaterialData->CastShadows)
                    continue;
                
                renderMesh(m->GeometryData, m->MaterialData, scene, cameraPos, projectionMatrix, viewMatrix, modelMatrix, type);
            }
        }
    }

    void Renderer::renderDebugMode(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera){
        glm::mat4 projectionMatrix = camera->GetProjectionMatrix();
        glm::mat4 viewMatrix = camera->GetViewMatrix();
        glm::vec3 cameraPos = camera->GetWorldPosition();

        std::shared_ptr<LineGeometry> db_lineDirGeometry = std::dynamic_pointer_cast<LineGeometry>(_DB_normalDirMesh->GeometryData);

        for(std::shared_ptr<Light> l : scene->GetLights()){
            switch(l->GetType()){
                case LightType::POINT:{
                    auto point = std::dynamic_pointer_cast<PointLight>(l);
                    if(point){
                        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), point->Position);
                        if(DB_DrawLightPositions){
                            _DB_lightMesh->MaterialData->SetUniformValue<Color>("diffuseColor", point->LightColor);
                            renderMesh(_DB_lightMesh->GeometryData, _DB_lightMesh->MaterialData, scene, cameraPos, projectionMatrix, viewMatrix, modelMatrix, RenderType::NORMAL);
                        }
                    }  
                    break;
                }

                case LightType::SPOT:{
                    auto spot = std::dynamic_pointer_cast<SpotLight>(l);
                    if(spot){
                        glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), spot->Position);
                        if(DB_DrawLightPositions){
                            _DB_lightMesh->MaterialData->SetUniformValue<Color>("diffuseColor", spot->LightColor);
                            renderMesh(_DB_lightMesh->GeometryData, _DB_lightMesh->MaterialData, scene, cameraPos, projectionMatrix, viewMatrix, modelMatrix, RenderType::NORMAL);
                        }

                        if(DB_DrawLightDirections){
                            glm::vec3 direction = glm::normalize(spot->Direction);
                            db_lineDirGeometry->Regenerate(spot->Position, spot->Position + direction * DB_LightDirectionDistance);
                            renderMesh(_DB_normalDirMesh->GeometryData, _DB_normalDirMesh->MaterialData, scene, cameraPos, projectionMatrix, viewMatrix, glm::mat4(1.0f), RenderType::NORMAL);
                        }
                    }
                    break;  
                }
                                   
            }
        }

        if(DB_DrawVertexNormals){
            for(std::shared_ptr<SceneObject> object : scene->GetObjects()){
                auto model = std::dynamic_pointer_cast<Model>(object);
                if(!model)
                    continue;

                for(std::shared_ptr<Mesh> mesh : model->GetMeshes()){
                    if(!mesh->MaterialData->DebugMode && !DB_OverrideMaterialDebugMode)
                        continue;

                    for(Vertex vertex : mesh->GeometryData->GetVertices()){
                        glm::vec3 normal = glm::normalize(vertex.Normal);
                        db_lineDirGeometry->Regenerate(vertex.Position, vertex.Position + normal * DB_VertexNormalDistance);
                        renderMesh(_DB_normalDirMesh->GeometryData, _DB_normalDirMesh->MaterialData, scene, cameraPos, projectionMatrix, viewMatrix, model->GetModelMatrix(), RenderType::NORMAL);
                    }
                }
            }
        }
        
    }

    void Renderer::renderMesh(std::shared_ptr<Geometry> geo, std::shared_ptr<Material> mat, std::shared_ptr<Scene> scene, glm::vec3 cameraPos, glm::mat4 projection, glm::mat4 view, glm::mat4 model, RenderType type){
        if(type != RenderType::SHADOW_MAP){
            mat->Use();
            
            mat->SetUniform("projection", glm::value_ptr(projection));
            mat->SetUniform("view", glm::value_ptr(view));

            if(RenderShadows && mat->ReceiveShadows)
                mat->SetUniform("lightSpaceMatrix", glm::value_ptr(_dirLightSpaceMatrix));
                mat->SetUniform("uShadowMap", _dirShadowMapBuffer);
                mat->SetUniform("uPointShadowMap", _pointShadowCubeMap);

            mat->SetUniform("viewPos", cameraPos);
            mat->SetUniform("time", Time::GetElapsedTimeF());
            mat->SetUniform("deltaTime", Time::GetDeltaTimeF());

            if(mat->LightingRequired){

                SceneLightData lightData = scene->GetLightData();
                mat->SetUniform("uAmbientLightSet", lightData.AmbientLight);
                mat->SetUniform("uDirectionalLightSet", lightData.DirectionalLight);
                mat->SetUniform("uPointLightsAmt", lightData.PointLightsAmt);
                mat->SetUniform("uSpotLightsAmt", lightData.SpotLightsAmt);

                int pointIndex = 0;
                int spotIndex = 0;

                auto dirLight = scene->GetDirectionalLight();

                for(int i = 0; i < scene->GetLights().size(); i++){
                    switch(scene->GetLight(i)->GetType()){
                        case LightType::AMBIENT:
                            scene->GetLight(i)->Bind(mat, 0);
                            break;
                        
                        case LightType::DIRECTION:
                            if(RenderShadows)
                                mat->SetUniform("uDirectionalLight.position", ShadowMapDistance * -dirLight->Direction);
                            scene->GetLight(i)->Bind(mat, 0);
                            break;

                        case LightType::POINT:
                            scene->GetLight(i)->Bind(mat, pointIndex);
                            pointIndex++;
                            break;

                        case LightType::SPOT:
                            scene->GetLight(i)->Bind(mat, spotIndex);
                            spotIndex++;
                            break;
                    }
                    
                }
            }
        } else {
            //Override MaterialCull when rendering shadow map
            glCullFace(GL_BACK);
        }
        
        mat->SetUniform("model", glm::value_ptr(model));

        geo->Draw();
    }

    void Renderer::renderSkybox(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera, bool depthTest){
        if(scene->Skybox){
            glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix()));  
            glm::mat4 projection = camera->GetProjectionMatrix();
            glDepthFunc(GL_LEQUAL);
            if(depthTest) glDisable(GL_DEPTH_TEST);

            scene->Skybox->MaterialData->Use();
            scene->Skybox->MaterialData->SetUniform("projection", glm::value_ptr(projection));
            scene->Skybox->MaterialData->SetUniform("view", glm::value_ptr(view));
            scene->Skybox->GeometryData->Draw(); 

            glDepthFunc(GL_LESS); 
            if(depthTest) glEnable(GL_DEPTH_TEST);
        }
    }

    void Renderer::Bake(std::shared_ptr<Scene> scene){
        scene->UpdateObjects();

        for(std::shared_ptr<BakedCubeMap> cubeMap : scene->GetBakedCubeMaps()){
            SetViewport(0,0,cubeMap->GetFramebuffer()->GetWidth(), cubeMap->GetFramebuffer()->GetHeight());

            for(int i = 0; i < 6; i++){
                cubeMap->SetDirection(i);
                cubeMap->BindBuffer(i);

                renderSceneObjects(scene, cubeMap->GetCamera(), RenderType::BAKE);
                renderSkybox(scene, cubeMap->GetCamera(), false);

            }
            cubeMap->UnbindBuffer();
        }
        

        scene->UpdateBake();
        
        ResetViewport();
    }

    void Renderer::renderDirShadowMap(std::shared_ptr<Scene> scene){
        auto dirLight = scene->GetDirectionalLight();        
        if(!dirLight) return;
        _dirShadowMapCamera->Position = ShadowMapDistance * -dirLight->Direction;
        _dirShadowMapCamera->Rotation = glm::quat(glm::lookAt(_dirShadowMapCamera->Position, glm::vec3(0.0f), Camera::UP));

        _dirShadowMapCamera->Update();

        _dirLightSpaceMatrix = _dirShadowMapCamera->GetProjectionMatrix() * _dirShadowMapCamera->GetViewMatrix();

        SetViewport(0,0,_dirShadowMapBuffer->GetWidth(),_dirShadowMapBuffer->GetHeight());
        _dirShadowMapBuffer->Bind();

        renderSceneObjects(scene, _dirShadowMapCamera, RenderType::SHADOW_MAP);

        _dirShadowMapBuffer->Unbind();
        ResetViewport();

    }

    void Renderer::renderPointShadowMap(std::shared_ptr<Scene> scene){
        std::shared_ptr<PointLight> pointLight = scene->FindItemByType<PointLight>();

        glm::mat4 shadowProj = _pointShadowCubeMap->GetCamera()->GetProjectionMatrix();
        glm::vec3 lightPos = pointLight->Position;

        std::vector<glm::mat4> shadowTransforms;
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)));

        SetViewport(0, 0, _pointShadowCubeMap->GetWidth(), _pointShadowCubeMap->GetHeight());
        _pointShadowCubeMap->GetBuffer()->Bind();

        glClear(GL_DEPTH_BUFFER_BIT);
        _pointShadowMapMaterial->Use();

        for (unsigned int i = 0; i < 6; ++i)
            _pointShadowMapMaterial->SetUniform("shadowMatrices[" + std::to_string(i) + "]", glm::value_ptr(shadowTransforms[i]));

        _pointShadowMapMaterial->SetUniform("far_plane", _pointShadowCubeMap->GetCamera()->GetFarPlane());
        _pointShadowMapMaterial->SetUniform("lightPos", lightPos);

        renderSceneObjects(scene, TargetCamera, RenderType::SHADOW_MAP);

        _pointShadowCubeMap->GetBuffer()->Unbind();
        ResetViewport();
    }

    void Renderer::updateResolution(const std::shared_ptr<BufferPassComposer>& passComposer){
        if(!TargetWindow) return;

        glm::vec2 resolution = TargetWindow->GetResolution();
        float aspect = resolution.x / resolution.y;

        if(TargetCamera->GetAspectRatio() != aspect){
            TargetCamera->SetAspectRatio(resolution.x / resolution.y);
        }

        if(passComposer && passComposer->GetResolution() != resolution){
            passComposer->SetResolution(resolution);
        }

    }
    

    void Renderer::Render(std::shared_ptr<Scene> scene, std::shared_ptr<Framebuffer> buffer){
        if((!buffer && !TargetWindow)|| !scene || !TargetCamera) return;

        TargetCamera->UpdateTransformVectors();
        scene->UpdateObjects();

        if(buffer) buffer->Bind();

        if(RenderShadows){
            renderDirShadowMap(scene);
            renderPointShadowMap(scene);
        }

        std::shared_ptr<BufferPassComposer> passComposer = scene->PassComposer;

        //TODO: Don't update every frame
        updateResolution(passComposer);

        bool depthTestSkybox = false;
        if(passComposer) {
            passComposer->NewFrame();
            depthTestSkybox = passComposer->GetFogPass() != nullptr;
        }

        if(GuiRenderFunc){
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }


        renderSceneObjects(scene, TargetCamera);
        if(DebugRenderMode)
            renderDebugMode(scene, TargetCamera);

        if(passComposer)
            passComposer->PreSkybox();
        renderSkybox(scene, TargetCamera, depthTestSkybox);

        if(passComposer) passComposer->Render();

        if(buffer) buffer->Unbind();

        if(GuiRenderFunc){
            GuiRenderFunc();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
    }

    void Renderer::EndFrame(){
        if(TargetWindow){
            glfwSwapBuffers(TargetWindow->GetGLFWWindow());
        }
        glfwPollEvents();
    }

    glm::vec2 Renderer::GetScreenToWorld(glm::vec2 screenPos){
        if(!TargetWindow || !TargetCamera)
            return glm::vec3(0.0f);

        glm::vec4 ndcPos(screenPos.x, screenPos.y, 0.0f, 1.0f);
    
        glm::vec4 worldPos = glm::inverse(TargetCamera->GetProjectionMatrix()) * ndcPos;
    
        if (worldPos.w != 0.0f) {
            worldPos /= worldPos.w;
        }
    
        return glm::vec2(worldPos.x, worldPos.y);
    }
}