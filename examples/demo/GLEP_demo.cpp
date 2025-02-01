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

/* GLEP Demo */

#include <GLEP/core.hpp>
#include <GLEP/audio.hpp>
#include <GLEP/control/automation.hpp>
#include <GLEP/control/camera/first_person.hpp>

using namespace GLEP;
using namespace GLEP::Audio;
using namespace GLEP::Control;

const glm::vec2 screenResolution = glm::vec2(1200, 800);

std::unique_ptr<Renderer> renderer;
std::shared_ptr<Scene> scene;
std::unique_ptr<AudioEngine> audioEngine;

std::shared_ptr<FirstPersonController> controls;

void InitRenderer(){
    std::shared_ptr<Window> window = std::make_shared<Window>(
        WindowState::FULLSCREEN,
        screenResolution, 
        "GLEP Demo"
    );

    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(
        90.0f,
        screenResolution.x / screenResolution.y,
        0.01f,
        100.0f
    );
    camera->Position = glm::vec3(8.3f, 7.0f, -2.2f);
    camera->Rotation = glm::lookAt(camera->Position, glm::vec3(-2.0f, 0.0f, 5.0f), Camera::UP);

    renderer = std::make_unique<Renderer>(window, camera);
    renderer->RenderShadows = true;
    renderer->ShadowMapDistance = 20.0f;
    renderer->GetShadowMapBuffer()->SetResolution(glm::vec2(2048));
    std::shared_ptr<OrthographicCamera> shadowMapCamera = std::dynamic_pointer_cast<OrthographicCamera>(renderer->GetShadowMapCamera());
    shadowMapCamera->SetFarPlane(100.0f);
    shadowMapCamera->SetSize(12.0f);
    scene = std::make_shared<Scene>();

    controls = std::make_shared<FirstPersonController>(camera, 2.0f, 0.1f);
}

void InitScene(){
    std::shared_ptr<Geometry> cubeGeometry = std::make_shared<CubeGeometry>(1.0f, 1.0f, 1.0f);
    std::shared_ptr<TextureCubeMap> skyboxCubeMap = std::make_shared<TextureCubeMap>(File::GLEP_DEFAULT_CUBE_MAP);
    std::shared_ptr<Material> skyboxMaterial = std::make_shared<SkyboxMaterial>(skyboxCubeMap);
    scene->Skybox = std::make_shared<Mesh>(cubeGeometry, skyboxMaterial); 

    std::filesystem::path sponzaPath = File::DIRECTORY / "examples" / "res" / "models" / "sponza" / "sponza.obj";

    std::shared_ptr<ImportModelTexture> sponzaTextures = std::make_shared<ImportModelTexture>(sponzaPath);
    std::shared_ptr<LambertMaterial> sponzaMaterial = std::make_shared<LambertMaterial>(std::shared_ptr<Texture>(nullptr));
    sponzaMaterial->ReceiveShadows = true;
    sponzaMaterial->CastShadows = true;
    std::shared_ptr<ImportGeometryModel> sponza = std::make_shared<ImportGeometryModel>(sponzaPath, sponzaMaterial);
    sponza->ApplyImportTextures(sponzaTextures);
    sponza->Scale = glm::vec3(0.01f);
    scene->Add(sponza);

    std::shared_ptr<ImportGeometry> isosphereGeometry = std::make_shared<ImportGeometry>(File::DIRECTORY / "examples" / "res" / "models"/ "isosphere" / "isosphere.obj");
    std::shared_ptr<Material> isosphereMaterial = std::make_shared<ReflectionMaterial>(std::shared_ptr<CubeMap>(nullptr), Color(0.9f, 0.9f, 1.0f));
    isosphereMaterial->CastShadows = true;
    std::shared_ptr<ImportGeometryModel> isosphere = std::make_shared<ImportGeometryModel>(isosphereGeometry, isosphereMaterial);
    isosphere->Position = glm::vec3(-3.0f, 2.0f, 0.3f);
    scene->Add(isosphere);

    std::shared_ptr<InterpClip<glm::vec3>> isosphereClip0 = std::make_shared<InterpClip<glm::vec3>>(&isosphere->Position, isosphere->Position, glm::vec3(7.0f, 2.0f, 0.3f), 5.0f, InterpFunc::EASE_IN_OUT_CUBIC);
    std::shared_ptr<InterpClip<glm::vec3>> isosphereClip1 = std::make_shared<InterpClip<glm::vec3>>(&isosphere->Position, glm::vec3(7.0f, 2.0f, 0.3f), isosphere->Position, 5.0f, InterpFunc::EASE_IN_OUT_CUBIC);

    std::shared_ptr<InterpSequence<glm::vec3>> isosphereSequence = std::make_shared<InterpSequence<glm::vec3>>(0.0f, true);
    isosphereSequence->Add(isosphereClip0);
    isosphereSequence->Add(isosphereClip1);
    InterpManager::Queue(isosphereSequence);

    std::shared_ptr<BakedCubeMap> isosphereCubeMap = std::make_shared<BakedCubeMap>(isosphere->Position, 1024);
    isosphere->Add(isosphereCubeMap->GetCamera());
    scene->Add(isosphereCubeMap);
}

std::shared_ptr<AmbientLight> ambientLight;
std::shared_ptr<DirectionalLight> directionalLight;

void InitLighting(){
    ambientLight = std::make_shared<AmbientLight>(Color(0.7f, 0.8f, 1.0f), 0.2f);
    scene->Add(ambientLight);

    directionalLight = std::make_shared<DirectionalLight>(glm::vec3(0.25f, -1.0f, 0.11f), Color(1.0f, 0.9f, 0.7f), 1.0f);
    scene->Add(directionalLight);
}

std::shared_ptr<FogPass> fogPass;

void InitPostProcessing(){
    std::shared_ptr<BufferPassComposer> passComposer = std::make_shared<BufferPassComposer>(screenResolution);
    scene->PassComposer = passComposer;

    fogPass = std::make_shared<FogPass>(0.001f, 7.0f, Color(0.7f, 0.8f, 1.0f));
    passComposer->Add(fogPass);
}

std::shared_ptr<AudioSource> ambientSfx;

void InitAudio(){
    audioEngine = std::make_unique<AudioEngine>();

    std::shared_ptr<AudioBuffer> ambientSfxBuffer = std::make_shared<AudioBuffer>(File::DIRECTORY / "examples" / "res" / "audio" / "volivieri_civenna_morning.wav");
    ambientSfx = std::make_shared<AudioSource>(ambientSfxBuffer);
    ambientSfx->SetGain(4.0f);
    ambientSfx->SetLoop(true);

    std::shared_ptr<AudioEffect> reverbEffect = std::make_shared<AudioEffect>(AudioEffectType::REVERB);
    ambientSfx->AddEffect(reverbEffect);

    ambientSfx->Play();
}

void GuiRender(){
    ImGui::Begin("Debug");

    ImGui::Separator();
    ImGui::Text("Press Q to control camera.");
    ImGui::Separator();

    std::string fpsText = "FPS: " + std::to_string(Time::GetFPS());
    ImGui::Text(fpsText.c_str());

    ImGui::Separator();
    ImGui::Text("Ambient Light");

    std::array<float, 4> ambientColor = ambientLight->LightColor.ToArray();
    if(ImGui::ColorEdit4("Ambient Color", ambientColor.data())){
        ambientLight->LightColor = Color(ambientColor);
    }

    ImGui::SliderFloat("Ambient Intensity", &ambientLight->Intensity, 0.0f, 10.0f);

    ImGui::Separator();
    ImGui::Text("Directional Light");

    std::array<float, 4> directionalColor = directionalLight->LightColor.ToArray();
    if(ImGui::ColorEdit4("Directional Color", directionalColor.data())){
        directionalLight->LightColor = Color(directionalColor);
    }

    ImGui::SliderFloat("Directional Intensity", &directionalLight->Intensity, 0.0f, 10.0f);

    float directionalDir[3] = {directionalLight->Direction.x, directionalLight->Direction.y, directionalLight->Direction.z};
    if(ImGui::SliderFloat3("Sun Direction", directionalDir, -1.0f, 1.0f)){
        directionalLight->Direction = glm::vec3(directionalDir[0], directionalDir[1], directionalDir[2]);
    }
    
    ImGui::Separator();
    ImGui::Text("Shadow Map");

    std::shared_ptr<OrthographicCamera> shadowMapCamera = std::dynamic_pointer_cast<OrthographicCamera>(renderer->GetShadowMapCamera());

    ImGui::SliderFloat("Shadow Distance", &renderer->ShadowMapDistance, 0.01f, 100.0f);

    float shadowMapCameraSize = shadowMapCamera->GetSize();
    if(ImGui::SliderFloat("Camera Size:", &shadowMapCameraSize, 0.1f, 100.0f)){
        shadowMapCamera->SetSize(shadowMapCameraSize);
    }

    ImGui::Separator();
    ImGui::Text("Fog");

    std::array<float, 4> fogColor = fogPass->GetMaterial()->GetUniform<Color>("uMaterial.fogColor")->Value.ToArray();
    if(ImGui::ColorEdit4("Fog Color", fogColor.data())){
        fogPass->GetMaterial()->SetUniformValue<Color>("uMaterial.fogColor", fogColor);
    }

    float fogNear = fogPass->GetMaterial()->GetUniform<float>("uMaterial.nearPlane")->Value;
    if(ImGui::SliderFloat("Fog Near Plane", &fogNear, 0.0001f, 1.0f)){
        fogPass->GetMaterial()->SetUniformValue<float>("uMaterial.nearPlane", fogNear);
    }

    float fogFar = fogPass->GetMaterial()->GetUniform<float>("uMaterial.farPlane")->Value;
    if(ImGui::SliderFloat("Fog Far Plane", &fogFar, 0.0001f, 20.0f)){
        fogPass->GetMaterial()->SetUniformValue<float>("uMaterial.farPlane", fogFar);
    }
    
    
    ImGui::End();
}

void Update(){
    while(renderer->IsRunning()){
        Time::Update();
        Input::Update(renderer->TargetWindow);
        InterpManager::Update();

        controls->Update(renderer->TargetWindow);

        renderer->Bake(scene);

        renderer->Render(scene);
        renderer->EndFrame();
    }
}

int main(){

    InitRenderer();
    InitScene();
    InitLighting();
    InitPostProcessing();
    InitAudio();
    //renderer->GuiRenderFunc = GuiRender;
    Update();   
    
    return 0;
}