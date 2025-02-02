#include <GLEP/core.hpp>
#include <GLEP/control/camera/first_person.hpp>

using namespace GLEP;
using namespace GLEP::Control;

const glm::vec2 screenResolution = glm::vec2(1200, 800);

int main(){

    std::shared_ptr<Window> window = std::make_shared<Window>(
        WindowState::WINDOWED, // Window state (Windowed, Windowed Fullscreen, Fullscreen)
        screenResolution, // Window resolution
        "GLEP Example - Core: Baked Cube Maps" // Window title
    );

    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(
        45.0f, // FOV (Degrees)
        screenResolution.x / screenResolution.y, // Aspect Ratio
        0.01f, // Near Plane
        100.0f // Far Plane
    );
    camera->Position = glm::vec3(0.0f, 0.0f, 3.0f);
    camera->Rotation = glm::quat(glm::lookAt(camera->Position, glm::vec3(0.0f), Camera::UP));

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(window, camera);

    /* ----------------First Person Controller--------------- */
    std::shared_ptr<FirstPersonController> firstPersonController = std::make_shared<FirstPersonController>(
        camera, //Target camera
        2.0f, // Move speed
        0.1f, // Look speed
        false, // Invert X Axis
        false // Invert Y Axis
    );
    /* ------------------------------------------------------ */

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    std::shared_ptr<Texture> diffuseTex = std::make_shared<Texture>(File::DIRECTORY / "brickwall.jpg");
    std::shared_ptr<Texture> normalTex = std::make_shared<Texture>(File::DIRECTORY / "brickwall_normal.jpg", TextureType::NORMAL);

    std::shared_ptr<Geometry> planeGeometry = std::make_shared<PlaneGeometry>(1.0f, 1.0f);
    std::shared_ptr<Material> normalMaterial = std::make_shared<BlinnPhongMaterial>(diffuseTex, Color(1.0f), 16.0f);
    std::shared_ptr<Material> phongMaterial = std::make_shared<PhongMaterial>(diffuseTex, Color(1.0f), 8.0f);
    phongMaterial->AddUniform<std::shared_ptr<Texture>>("uMaterial.normalTex", normalTex);
    phongMaterial->AddUniform<bool>("uMaterial.hasNormalMap", true);
    normalMaterial->AddUniform<std::shared_ptr<Texture>>("uMaterial.normalTex", normalTex);
    normalMaterial->AddUniform<bool>("uMaterial.hasNormalMap", true);
    std::shared_ptr<Model> plane0 = std::make_shared<Model>(planeGeometry, normalMaterial);
    //plane0->Rotation = glm::lookAt(glm::vec3(0.0f), Camera::UP, Camera::FRONT);
    plane0->Position.x = -1.0f;
    scene->Add(plane0);
    std::shared_ptr<Model> plane1 = std::make_shared<Model>(planeGeometry, phongMaterial);
    //plane1->Rotation = glm::lookAt(glm::vec3(0.0f), Camera::UP, Camera::FRONT);
    plane1->Position.x = 1.0f;
    scene->Add(plane1);

    std::shared_ptr<AmbientLight> ambientLight = std::make_shared<AmbientLight>(Color(1.0f), 0.2f);
    scene->Add(ambientLight);
    std::shared_ptr<PointLight> pointLight0 = std::make_shared<PointLight>(glm::vec3(-1.0f, 0.5f, 0.5f), Color(1.0f), 1.0f, 1.0f, 0.09f, 0.032f);
    scene->Add(pointLight0);
    std::shared_ptr<PointLight> pointLight1 = std::make_shared<PointLight>(glm::vec3(1.0f, 0.5f, 0.5f), Color(1.0f), 1.0f, 1.0f, 0.09f, 0.032f);
    scene->Add(pointLight1);

    //std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>(glm::vec3(0.2f, -1.0f, 0.0f), Color(1.0f), 1.0f);
    //scene->Add(directionalLight);

    /*
    std::shared_ptr<SpotLight> spotLight0 = std::make_shared<SpotLight>(
        glm::vec3(-1.0f, 0.0f, 2.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        Color::WHITE,
        2.0f,
        15.0f,
        10.0f,
        1.0f, 
        0.09f,
        0.032f
    );
    scene->Add(spotLight0);

    std::shared_ptr<SpotLight> spotLight1 = std::make_shared<SpotLight>(
        glm::vec3(1.0f, 0.0f, 2.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        Color::WHITE,
        2.0f,
        15.0f,
        10.0f,
        1.0f, 
        0.09f,
        0.032f
    );
    scene->Add(spotLight1);

    
    renderer->GuiRenderFunc = [spotLight0, spotLight1]{
        ImGui::Begin("Debug");

        ImGui::Separator();

        if(ImGui::SliderFloat("Inner", &spotLight0->InnerCutOff, 0.001f, 1.0f)){
            spotLight1->InnerCutOff = spotLight0->InnerCutOff;
        }

        if(ImGui::SliderFloat("Outer", &spotLight0->OuterCutOff, 0.001f, 1.0f)){
            spotLight1->OuterCutOff = spotLight0->OuterCutOff;
        }

        if(ImGui::SliderFloat("Intensity", &spotLight0->Intensity, 0.0f, 10.0f)){
            spotLight1->Intensity = spotLight0->Intensity;
        }
        
        ImGui::End();
    };
    */
    


    /* ---------------------Render Loop---------------------- */
    while(renderer->IsRunning()){
        Time::Update(); // Update internal clock
        Input::Update(renderer->TargetWindow); // Update user input

        firstPersonController->Update(renderer->TargetWindow); //Update each frame

        if(Input::GetInput(renderer->TargetWindow, KeyCode::SPACE) == InputState::PRESS){
            normalMaterial->SetUniformValue<bool>("uMaterial.hasNormalMap", !normalMaterial->GetUniform<bool>("uMaterial.hasNormalMap")->Value);
        }

        renderer->Render(scene); // Render 
        renderer->EndFrame(); // End frame, poll events
    }
    /* ------------------------------------------------------ */

    return 0;
}