#include <GLEP/core.hpp>

using namespace GLEP;

const glm::vec2 screenResolution = glm::vec2(1200, 800);

int main(){

    /* -Initialise key objects (Window, Camera & Renderer)- */
    std::shared_ptr<Window> window = std::make_shared<Window>(
        WindowState::WINDOWED, // Window state (Windowed, Windowed Fullscreen, Fullscreen)
        screenResolution, // Window resolution
        "GLEP Example - Core: Basic Scene" // Window title
    );

    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(
        60.0f, // FOV (Degrees)
        screenResolution.x / screenResolution.y, // Aspect Ratio
        0.01f, // Near Plane
        100.0f // Far Plane
    );
    camera->Position = glm::vec3(0.0f, 2.5f, 7.5f);
    camera->Rotation = glm::quat(glm::lookAt(camera->Position, glm::vec3(0.0f), Camera::UP));

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(window, camera);
    renderer->DebugRenderMode = true;
    renderer->DB_DrawLightAttenuation = false;
    /* ------------------------------------------------------ */

    /* -----Initialize the scene and add an object to it----- */
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    std::vector<glm::vec3> objectPositions;
    objectPositions.push_back(glm::vec3(-3.0,  -0.5, -3.0));
    objectPositions.push_back(glm::vec3( 0.0,  -0.5, -3.0));
    objectPositions.push_back(glm::vec3( 3.0,  -0.5, -3.0));
    objectPositions.push_back(glm::vec3(-3.0,  -0.5,  0.0));
    objectPositions.push_back(glm::vec3( 0.0,  -0.5,  0.0));
    objectPositions.push_back(glm::vec3( 3.0,  -0.5,  0.0));
    objectPositions.push_back(glm::vec3(-3.0,  -0.5,  3.0));
    objectPositions.push_back(glm::vec3( 0.0,  -0.5,  3.0));
    objectPositions.push_back(glm::vec3( 3.0,  -0.5,  3.0));

    std::filesystem::path backpackPath = File::DIRECTORY / "examples" / "res" / "models" / "backpack" / "backpack.obj";

    std::shared_ptr<Material> material = std::make_shared<BlinnPhongMaterial>(std::shared_ptr<Texture>(nullptr), Color::WHITE, 16.0f);
    std::shared_ptr<ImportModelTexture> backpackTexture = std::make_shared<ImportModelTexture>(backpackPath);
    std::shared_ptr<ImportGeometry> backpackGeometry = std::make_shared<ImportGeometry>(backpackPath);
    for(int i = 0; i < objectPositions.size(); i++){
        std::shared_ptr<ImportGeometryModel> backpack = std::make_shared<ImportGeometryModel>(backpackPath, material);
        backpack->ApplyImportTextures(backpackTexture);
        backpack->Scale = glm::vec3(0.005f);
        backpack->Position = objectPositions[i];
        scene->Add(backpack);
    }

    std::shared_ptr<AmbientLight> ambientLight = std::make_shared<AmbientLight>(Color::WHITE, 0.5f);
    scene->Add(ambientLight);

    srand(13);
    for(int i = 0; i < 10; i++){
        float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
        float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
        float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);

        float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5);
        float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5);
        float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5);

        std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>(
            glm::vec3(xPos, yPos, zPos),
            Color(rColor, gColor, bColor),
            0.5f,
            0.7f,
            1.8f
        );

        scene->Add(pointLight);
    }
        /* ------------------------------------------------------ */

    /* ---------------------Render Loop---------------------- */
    while(renderer->IsRunning()){
        Time::Update(); // Update internal clock
        Input::Update(renderer->TargetWindow); // Update user input

        renderer->Render(scene); // Render 
        renderer->EndFrame(); // End frame, poll events
    }
    /* ------------------------------------------------------ */

    return 0;
}