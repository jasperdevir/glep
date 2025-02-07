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

#ifndef BUFFER_PASS_HPP
#define BUFFER_PASS_HPP

#include <GLEP/core/time.hpp>
#include <GLEP/core/framebuffer.hpp>
#include <GLEP/core/geometry.hpp>
#include <GLEP/core/material.hpp>
#include <GLEP/core/mesh.hpp>

#include <string>
#include <vector>
#include <memory>
#include <filesystem>

#include <nlohmann/json.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace GLEP{

    using json = nlohmann::ordered_json;

    class BufferPass{
        protected:
            std::string _name = "buffer_pass";

            std::shared_ptr<ColorDepthFramebuffer> _framebuffer;
            std::shared_ptr<Mesh> _mesh;

        public:
            BufferPass(std::shared_ptr<Material> material);

            /// @brief Get the framebuffer of the pass.
            /// @return Framebuffer
            std::shared_ptr<ColorDepthFramebuffer> GetFramebuffer();
            
            /// @brief Get the material of the pass.
            /// @return Material
            std::shared_ptr<Material> GetMaterial();
            
            /// @brief Get the name of the pass.
            /// @return Name
            std::string GetName();


            /// @brief Initialize the buffers' resolution
            /// @param resolution Buffer resolution
            void InitializeBuffer(glm::vec2 resolution);


            /// @brief Bind the framebuffer as the active framebuffer
            void Bind();
            
            /// @brief Render the material to a full-screen quad
            void Render();


            /// @brief Serialize data to JSON format
            /// @return Serialized data
            json ToJson();
            
            /// @brief Deserialize data from JSON format
            /// @param data BufferPass data in JSON format
            /// @return Deserialized BufferPass
            static std::shared_ptr<BufferPass> FromJson(const json& data);
    };

    class RenderPass : public BufferPass{
        public:
            RenderPass();

            /// @brief Deserialize data from JSON format
            /// @param data RenderPass data in JSON format
            /// @return Deserialized RenderPass
            static std::shared_ptr<RenderPass> FromJson(const json& data);

    };

    class DepthPass : public BufferPass{
        public:
            DepthPass();

            /// @brief Deserialize data from JSON format
            /// @param data DepthPass data in JSON format
            /// @return Deserialized DepthPass
            static std::shared_ptr<DepthPass> FromJson(const json& data);

    };

    class KernelPass : public BufferPass {
        public:
            /// @brief Sharpen kernel
            static const float SHARPEN[9];

            /// @brief Box blur kernel
            static const float BLUR[9];

            /// @brief Edge detection kernel
            static const float EDGE[9];

            KernelPass(const float kernel[9], float offset);

            /// @brief Deserialize data from JSON format
            /// @param data KernelPass data in JSON format
            /// @return Deserialized KernelPass
            static std::shared_ptr<KernelPass> FromJson(const json& data);

    };

    class GrainPass : public BufferPass {
        public:
            GrainPass(glm::vec2 uvScale, float intensity);

            /// @brief Deserialize data from JSON format
            /// @param data BufferPass data in JSON format
            /// @return Deserialized BufferPass
            static std::shared_ptr<GrainPass> FromJson(const json& data);

    };

    class FogPass : public BufferPass {
        public:
            FogPass(float near, float far, Color fogColor);

            /// @brief Deserialize data from JSON format
            /// @param data BufferPass data in JSON format
            /// @return Deserialized BufferPass
            static std::shared_ptr<FogPass> FromJson(const json& data);

    };

    class BufferPassComposer{
        private:
            glm::vec2 _resolution;

            std::shared_ptr<RenderPass> _objectPass;
            std::shared_ptr<RenderPass> _skyboxPass;
            std::shared_ptr<RenderPass> _renderPass;
            std::shared_ptr<FogPass> _fogPass;

            std::vector<std::shared_ptr<BufferPass>> _bufferPasses;

            void initialize();

        public:
            Color ClearColor = Color::BLACK;

            BufferPassComposer(glm::vec2 resolution);

            /// @brief Get the fog pass, if assigned.
            /// @return Fog pass
            std::shared_ptr<FogPass> GetFogPass();
            
            /// @brief Get the assigned buffer passes.
            /// @return Buffer passes
            std::vector<std::shared_ptr<BufferPass>> GetBufferPasses();
            
            /// @brief Get the target render resolution.
            /// @return Render resolution
            glm::vec2 GetResolution();

            /// @brief Set the target render resolution.
            /// @param resolution Render resolution to set
            void SetResolution(glm::vec2 resolution);

            /// @brief Add a BufferPass to the end of the chain.
            /// @param pass BufferPass to add
            void Add(std::shared_ptr<BufferPass> pass);
            
            /// @brief Assign a FogPass.
            /// @param fogPass FogPass to assign
            void Add(std::shared_ptr<FogPass> fogPass);
            

            /// @brief Bind the first buffer pass in the chain, binding the framebuffer data of a render.
            void NewFrame();
            
            /// @brief Bind the framebuffer for the skybox render (Only if a fog pass is assigned).
            void PreSkybox();
            
            /// @brief Render each buffer pass in the chain consecutively.
            void Render();

            
            /// @brief Serialize data to JSON format.
            /// @return Serialized data
            json ToJson();
            
            /// @brief Deserialize data from JSON format.
            /// @param data BufferPassComposer data in JSON format
            /// @return Deserialize BufferPassComposer
            static std::shared_ptr<BufferPassComposer> FromJson(const json& data);
    };
}

#endif //BUFFER_PASS_HPP