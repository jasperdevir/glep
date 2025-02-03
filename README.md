# GLEP (OpenGL Engine Platform)

GLEP (OpenGL Engine Platform) is a work-in-progress real-time graphics rasterization framework written in C++, using the OpenGL API. 
Heavily assisted by Joey de Vries' OpenGL tutorial series [LearnOpenGL](https://learnopengl.com/), as well as inspired by Ricardo Cabello's JavaScript framework [Three.js](https://threejs.org/) and Ramon Santamaria's C library [raylib](https://github.com/raysan5/raylib).

If you encounter any bugs, issues, or have any suggestions please let me know.

![](glep_demo.gif)

### External Libraries: 
- [glad](https://github.com/Dav1dde/glad) (OpenGL Binding)
- [glfw](https://github.com/glfw/glfw) (Window and Input Management)
- [glm](https://github.com/g-truc/glm) (Mathematics)
- [ImGui](https://github.com/ocornut/imgui) (GUI)
- [assimp](https://github.com/assimp/assimp) (Model Loading)
- [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) (Texture Loading)
- [nlohmann/json](https://github.com/nlohmann/json) (Serialisation)
- [OpenAL Soft](https://github.com/kcat/openal-soft) (Audio API) 
- [libsndfile](https://github.com/libsndfile/libsndfile) (Audio File Loading)

## Contents

- [Docs](https://jasperdevir.com/docs/glep)
- [Features](#features)
- [Building](#building)
- [Usage](#usage)
- [Goals](#goals)
- [Known Issues](#known-issues)
- [References](#references)

## Features
### Core Module
- Scene Management
    - World and Local Transforms
    - Parent, Child hierarchy
    - JSON Scene Serialisation
- Geometry
    - Procedual Cube, Plane, and Grid Geometry
    - Custom Model Support
- Materials
    - Texture Support
    - Built-In Materials (Unlit, Lambert, Phong)
- Lighting
    - Point Lights, Directional Lights, and Spotlights
    - Directional Shadow Maps
    - Baked Cube Maps
- Post Processing
    - Buffer Pass Composer
    - Built-In FX (Grain, Depth, Kernel Filter)
### Audio Module
- Audio File Support (.wav)
- Built-In FX (Chorus, Reverb, etc.)
### Control Module 
- Interp Clip, Sequence, and Manager for automated interpolation
- First-Person Camera Controller

## Building The Library
### Currently Tested Platforms:
- Windows 11 (x64)
- MacOS Sonoma 14.5 (M1)

### Requirements:
- cmake
```bash
git clone https://github.com/jasperdevir/glep.git
```
```bash
cd GLEP
cd build
```
```bash
cmake ..
cmake --build . --config Release
```
If there was no errors, the required folders for a GLEP project include:
- ```bin``` (Executable Files and Dynamic Libraries)
- ```include``` (GLEP and External Header Files)
- ```lib``` (Static Libraries)
- ```res``` (Resource Files)

I'd recommend the following structure for a GLEP projects:
```
Project/
├─ bin/
├─ build/
├─ include/
├─ lib/
├─ res/
├─ src/
│  ├─ main.cpp
├─ CMakeLists.txt
```
## Usage
In-depth examples are avaliable in the ```examples``` directory, with their programs built by default to ```bin```. This can be disabled by setting ```GLEP_BUILD_EXAMPLES``` to ```false``` during the build process. 
### Basic Example

```cpp
#include <GLEP/core.hpp>

using namespace GLEP;

const glm::vec2 screenResolution = glm::vec2(1200, 800);

int main(){
    /* Initialization */
    std::shared_ptr<Window> window = std::make_shared<Window>(
        WindowState::WINDOWED,
        screenResolution, 
        "GLEPWindow"
    );

    std::shared_ptr<Camera> camera = std::make_shared<PerspectiveCamera>(
        45.0f, 
        screenResolution.x / screenResolution.y, 
        0.01f, 
        100.0f
    );
    camera->Position = glm::vec3(0.0f, 0.0f, 3.0f);

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>(window, camera);

    /* Scene Management */
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    std::shared_ptr<Geometry> geometry = std::make_shared<CubeGeometry>(1.0f, 1.0f, 1.0f);
    std::shared_ptr<Material> material = std::make_shared<UnlitMaterial>(Color(1.0f));
    std::shared_ptr<Model> model = std::make_shared<Model>(geometry, material);
    scene->Add(model);

    /* Render Loop */
    while(renderer->IsRunning()){
        Time::Update();
        Input::Update(renderer->TargetWindow);

        renderer->Render(scene);
        renderer->EndFrame();
    }

    return 0;
}
```
### Audio Module Example
```cpp
#include <GLEP/audio.hpp>

using namespace GLEP::Audio;
```

```cpp
std::unique_ptr<AudioEngine> audioEngine = std::make_shared<AudioEngine>();

std::shared_ptr<AudioBuffer> buffer = std::make_shared<AudioBuffer>("audio.wav");
std::shared_ptr<AudioSource> source = std::make_shared<AudioSource>(buffer);
source->Play();
```

## Goals
### Short-term
- Light Helper Classes
- Point Shadow Maps (In Progress)
- Normal Map Support (Implemented - Needs Testing)
- Height Map Support (Implemented - Needs Testing)
- Reflection Map Support
### Long-term
- Linux Support
- UI Rendering Module
- Custom Math Module
- Collision and Event Module
- Editor Application
## Known Issues
### Core
- OPTIMISATION
- FromJson() formating error exceptions
- _(MacOS)_ BakedCubeMaps don't render
- Camera direction vectors wrong? - (Core example 4 - Input)
### Control
- FirstPersonController ignores initial camera rotation
### Audio
- Issues with multiple AudioEffect's on a single AudioSource
- 3D audio

## References
### Articles, Books & Videos:
- [LearnOpenGL](https://learnopengl.com/) by Joey de Vries
- [Real-Time Renderering, Fourth Edition](https://www.realtimerendering.com/) by Tomas Akenine-Möller, Eric Haines, Naty Hoffman, Angelo Pesce, Michał Iwanicki, and Sébastien Hillaire
- [Coding Adventures Series](https://www.youtube.com/playlist?list=PLFt_AvWsXl0ehjAfLFsp1PGaatzAwo0uK) by Sebastian Lague
### Assets:
- [backpack.obj](https://sketchfab.com/3d-models/survival-guitar-backpack-799f8c4511f84fab8c3f12887f7e6b36) - Survival Guitar Backpack by Berk Gedik
- [bunny.ply](https://graphics.stanford.edu/data/3Dscanrep/) - Stanford Bunny
- [sponza.obj](https://github.com/jimmiebergmann/Sponza) Sponza Version by Jimmie Bergmann (Original by Frank Meinl)
- [containerB.png](https://learnopengl.com/img/textures/container2.png) - LearnOpenGL
- [containerB_specular.png](https://learnopengl.com/img/textures/container2_specular.png) - LearnOpenGL
- [default.cubemap](https://learnopengl.com/img/textures/skybox.zip) - LearnOpenGL
- [volivieri_civenna_morning.wav](https://freesound.org/people/volivieri/sounds/161186/) - Civenna Morning by Volivieri

