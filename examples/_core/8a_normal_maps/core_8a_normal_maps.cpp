#include <GLEP/core.hpp>
#include <GLEP/control/camera/first_person.hpp>

using namespace GLEP;
using namespace GLEP::Control;

const glm::vec2 screenResolution = glm::vec2(1200, 800);

std::shared_ptr<Material> blinnPhong;
std::shared_ptr<Material> phong;
std::shared_ptr<Material> lambert;

void key_callback(Window* window, KeyCode key, int scancode, InputState state, KeyMod mod){
    if(state == InputState::PRESS){
        if(key == KeyCode::KEY_1){
            blinnPhong->SetUniformValue<bool>("hasNormalMap", !blinnPhong->GetUniformValue<bool>("hasNormalMap", false));
            phong->SetUniformValue<bool>("hasNormalMap", !phong->GetUniformValue<bool>("hasNormalMap", false));
            lambert->SetUniformValue<bool>("hasNormalMap", !lambert->GetUniformValue<bool>("hasNormalMap", false));
        }
        if(key == KeyCode::KEY_2){
            blinnPhong->SetUniformValue<bool>("hasDepthTex", !blinnPhong->GetUniformValue<bool>("hasDepthTex", false));
            phong->SetUniformValue<bool>("hasDepthTex", !phong->GetUniformValue<bool>("hasDepthTex", false));
            lambert->SetUniformValue<bool>("hasDepthTex", !lambert->GetUniformValue<bool>("hasDepthTex", false));
        }
    }
}

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

    std::shared_ptr<Texture> diffuseTex = std::make_shared<Texture>(File::DIRECTORY / "examples" / "res" / "textures" / "bricks2.jpg");
    std::shared_ptr<Texture> normalTex = std::make_shared<Texture>(File::DIRECTORY / "examples" / "res" / "textures" / "bricks2_normal.jpg", TextureType::NORMAL);
    std::shared_ptr<Texture> depthTex = std::make_shared<Texture>(File::DIRECTORY / "examples" / "res" / "textures" / "bricks2_disp.jpg", TextureType::HEIGHT);

    std::shared_ptr<Geometry> cubeGeometry = std::make_shared<CubeGeometry>(1.0f, 1.0f, 1.0f);
    blinnPhong = std::make_shared<BlinnPhongMaterial>(diffuseTex, Color(1.0f), 16.0f);
    blinnPhong->AddUniform<std::shared_ptr<Texture>>("normalTex", normalTex);
    blinnPhong->AddUniform<std::shared_ptr<Texture>>("depthTex", depthTex);
    blinnPhong->SetUniformValue<float>("depthScale", 0.1f);

    phong = std::make_shared<PhongMaterial>(diffuseTex, Color(1.0f), 32.0f);
    phong->AddUniform<std::shared_ptr<Texture>>("normalTex", normalTex);
    phong->AddUniform<std::shared_ptr<Texture>>("depthTex", depthTex);
    phong->SetUniformValue<float>("depthScale", 0.1f);

    lambert = std::make_shared<LambertMaterial>(diffuseTex);
    lambert->AddUniform<std::shared_ptr<Texture>>("normalTex", normalTex);
    lambert->AddUniform<std::shared_ptr<Texture>>("depthTex", depthTex);
    lambert->SetUniformValue<float>("depthScale", 0.1f);

    std::shared_ptr<Model> cube0 = std::make_shared<Model>(cubeGeometry, blinnPhong);
    cube0->Position.x = -1.5f;
    scene->Add(cube0);
    std::shared_ptr<Model> cube1 = std::make_shared<Model>(cubeGeometry, phong);
    scene->Add(cube1);
    std::shared_ptr<Model> cube2 = std::make_shared<Model>(cubeGeometry, lambert);
    cube2->Position.x = 1.5f;
    scene->Add(cube2);

    std::shared_ptr<AmbientLight> ambientLight = std::make_shared<AmbientLight>(Color(1.0f), 0.2f);
    scene->Add(ambientLight);
    std::shared_ptr<PointLight> pointLight0 = std::make_shared<PointLight>(glm::vec3(-1.0f, 1.0f, 2.0f), Color(1.0f), 1.0f, 1.0f, 0.09f, 0.032f);
    scene->Add(pointLight0);
    std::shared_ptr<PointLight> pointLight1 = std::make_shared<PointLight>(glm::vec3(1.0f, 1.0f, 2.0f), Color(1.0f), 1.0f, 1.0f, 0.09f, 0.032f);
    scene->Add(pointLight1);
    
    renderer->GuiRenderFunc = []{
        ImGui::Begin("Debug");

        ImGui::Separator();

        float blinnDepth = blinnPhong->GetUniformValue<float>("depthScale", -1.0f);
        if(ImGui::SliderFloat("Blinn Depth", &blinnDepth, -1.0f, 1.0f)){
            blinnPhong->SetUniformValue<float>("depthScale", blinnDepth);
        }

        float phongDepth = phong->GetUniformValue<float>("depthScale", -1.0f);
        if(ImGui::SliderFloat("Phong Depth", &phongDepth, -1.0f, 1.0f)){
            phong->SetUniformValue<float>("depthScale", phongDepth);
        }

        float lambertDepth = lambert->GetUniformValue<float>("depthScale", -1.0f);
        if(ImGui::SliderFloat("Lambert Depth", &lambertDepth, -1.0f, 1.0f)){
            lambert->SetUniformValue<float>("depthScale", lambertDepth);
        }
        
        ImGui::End();
    };
    

    Input::SetKeyCallback(renderer->TargetWindow, key_callback);
    
    /* ---------------------Render Loop---------------------- */
    while(renderer->IsRunning()){
        Time::Update(); // Update internal clock
        Input::Update(renderer->TargetWindow); // Update user input

        firstPersonController->Update(renderer->TargetWindow); //Update each frame

        renderer->Render(scene); // Render 
        renderer->EndFrame(); // End frame, poll events
    }
    /* ------------------------------------------------------ */

    return 0;
}