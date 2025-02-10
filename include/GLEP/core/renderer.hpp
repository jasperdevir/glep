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

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <GLEP/core/utility/opengl.hpp>
#include <GLEP/core/utility/print.hpp>

#include <GLEP/core/time.hpp>
#include <GLEP/core/color.hpp>
#include <GLEP/core/window.hpp>
#include <GLEP/core/light.hpp>
#include <GLEP/core/mesh.hpp>
#include <GLEP/core/model.hpp>
#include <GLEP/core/buffer_pass.hpp>
#include <GLEP/core/texture.hpp>
#include <GLEP/core/camera.hpp>
#include <GLEP/core/cube_map.hpp>
#include <GLEP/core/scene.hpp>

#include <memory>
#include <functional>
#include <string>

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace GLEP {

    class Renderer{
        protected:
            enum class RenderType{
                NORMAL,
                BAKE,
                SHADOW_MAP
            };

            bool _isGuiInitalized = false;
            bool _isGuiShutdown = false;

            std::shared_ptr<Framebuffer> _shadowMapBuffer;
            std::shared_ptr<Camera> _shadowMapCamera;
            glm::mat4 _lightSpaceMatrix = glm::mat4(1.0f);

            std::shared_ptr<Mesh> _DB_lightMesh;
            std::shared_ptr<Mesh> _DB_lightAttenuationMesh;
            std::shared_ptr<Mesh> _DB_normalDirMesh;

            void initializeDefaults();
            void initializeGui();

            void renderSkybox(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera, bool depthTest = true);
            void renderShadowMap(std::shared_ptr<Scene> scene);
            void renderSceneObjects(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera, RenderType type = RenderType::NORMAL);
            void renderDebugMode(std::shared_ptr<Scene> scene, std::shared_ptr<Camera> camera);
            void renderMesh(std::shared_ptr<Geometry> geo, std::shared_ptr<Material> mat, std::shared_ptr<Scene> scene, glm::vec3 cameraPos, glm::mat4 projection, glm::mat4 view, glm::mat4 model, RenderType type);

            void updateResolution(const std::shared_ptr<BufferPassComposer>& passComposer);

        public: 
            std::shared_ptr<Window> TargetWindow;
            std::shared_ptr<Camera> TargetCamera;

            std::function<void(void)> GuiRenderFunc;

            Color ClearColor = Color::BLACK;

            float ShadowMapDistance = 2.0f;
            bool RenderShadows = false;

            bool DebugRenderMode = false;
            bool DB_OverrideMaterialDebugMode = false;
            bool DB_DrawLightPositions = true;
            bool DB_DrawLightAttenuation = true;
            bool DB_DrawLightDirections = true;
            float DB_LightDirectionDistance = 2.0f;
            bool DB_DrawVertexNormals = false;
            float DB_VertexNormalDistance = 1.0f;

            Renderer(std::shared_ptr<Window> window);
            Renderer(std::shared_ptr<Window> window, std::shared_ptr<Camera> camera);
            ~Renderer();

            /// @brief Get the ImGuiIO struct.
            /// @return ImGuiIO struct
            ImGuiIO& GetGuiIO();


            /// @brief Get the camera used for rendering the shadow map.
            /// @return Shadow map camera
            std::shared_ptr<Camera> GetShadowMapCamera();

            /// @brief Get the buffer used for rendering the shadow map.
            /// @return Shadow map buffer
            std::shared_ptr<Framebuffer> GetShadowMapBuffer();


            /// @brief Convert a screen-space point to a world-space point using the target camera.
            /// @param screenPos Screen-space point
            /// @return World-space conversion
            glm::vec2 GetScreenToWorld(glm::vec2 screenPos);


            /// @brief Set the current state of the target window.
            /// @param state State to set
            void SetWindowState(WindowState state);

            /// @brief Set the dimensions of the rendering viewport.
            /// @param x X axis start point
            /// @param y Y axis end point
            /// @param width Total width in pixels
            /// @param height Total height in pixels
            void SetViewport(int x, int y, int width, int height);

            /// @brief Reset the viewport to the target window's current resolution.
            void ResetViewport();


            /// @brief Get if the target window should currently be open.
            /// @return If the target window is open
            bool IsRunning();


            /// @brief Render each baked cube maps and assign them to scene materials that require them.
            /// @param scene Target scene
            void Bake(std::shared_ptr<Scene> scene);

            /// @brief Render a scene to either the target window or a framebuffer.
            /// @param scene Target scene
            /// @param buffer Target framebuffer (If this is nullptr the scene will be rendered to the target window instead).
            void Render(std::shared_ptr<Scene> scene, std::shared_ptr<Framebuffer> buffer = nullptr);

            /// @brief Swap buffers and poll events.
            void EndFrame();
    };
}

#endif //RENDERER_HPP