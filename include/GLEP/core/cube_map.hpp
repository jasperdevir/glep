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

#ifndef CUBE_MAP_HPP
#define CUBE_MAP_HPP

#include <GLEP/core/utility/math.hpp>

#include <GLEP/core/texture.hpp>
#include <GLEP/core/framebuffer.hpp>
#include <GLEP/core/camera.hpp>

#include <vector>
#include <memory>
#include <filesystem>

#include <nlohmann/json.hpp>

namespace GLEP{

    using json = nlohmann::ordered_json;

    class CubeMap{
        protected:            
            unsigned int _ID;
            int _width;
            int _height;
            int _nrChannels;

            virtual void initialize() = 0;

        public:
            CubeMap();
            ~CubeMap();

            /// @brief Get the textures width dimension in pixels.
            /// @return Texture width
            int GetWidth();

            /// @brief Get the textures height dimension in pixels.
            /// @return Texture height
            int GetHeight();

            /// @brief Get the amount of color channels the textures contains.
            /// @return The amount of color channels
            int GetNrChannels();

            /// @brief Get the buffer ID
            /// @return Buffer ID
            unsigned int GetID();


            /// @brief Set the wrap properties for both texture axis.
            /// @param wrap Wrap properties to set for both axis
            void SetWrap(TextureWrap wrap);

            /// @brief Set the wrap properties for each texture axis.
            /// @param s Wrap properties to set for the S axis
            /// @param t Wrap properties to set for the T axis
            void SetWrap(TextureWrap s, TextureWrap t);

            /// @brief Set the border color for when TextureWrap::CLAMP_BORDER is set.
            /// @param color Border color to set
            void SetBorderColor(Color color);

            /// @brief Set the filter properties for both minifying and magnifying operations.
            /// @param filter Filter properties to set
            void SetFilter(TextureFilter filter);

            /// @brief Set the filter properties for the minifying and magnifying operations (Including Mipmaps).
            /// @param min Filter properties to set for minifying
            /// @param mag Filter properties to set for magnifying
            void SetFilter(TextureFilter min, TextureFilter mag);


            /// @brief Set active texture slot and bind texture.
            void Bind();


            /// @brief Serialize data to JSON format.
            /// @return Serialized data
            virtual json ToJson() = 0;

            /// @brief Deserialize data from JSON format.
            /// @param data CubeMap data in JSON format
            /// @return Deserialized CubeMap
            static std::shared_ptr<CubeMap> FromJson(const json& data);
    };

    class TextureCubeMap : public CubeMap{
        private:
            std::vector<std::filesystem::path> _filePaths;
            std::shared_ptr<Texture> _thumbTex;

            void initialize() override;

        public:
            TextureCubeMap(std::filesystem::path cubeMapFile);
            TextureCubeMap(std::vector<std::filesystem::path> filePaths);


            /// @brief Get the six texture file paths.
            /// @return Texture file paths
            std::vector<std::filesystem::path> GetFilePaths();

            /// @brief Get the X+ texture of the cube map.
            /// @return X+ texture
            std::shared_ptr<Texture> GetThumbTex();


            /// @brief Serialize data in JSON format.
            /// @return Serialize data
            json ToJson() override;

            /// @brief Deserialize data from JSON format.
            /// @param data TextureCubeMap data in JSON format
            /// @return Deserialized TextureCubeMap
            static std::shared_ptr<TextureCubeMap> FromJson(const json& data);

    };

    
    class BakedCubeMap : public CubeMap{
        private:
            std::shared_ptr<Camera> _camera;
            std::shared_ptr<ColorDepthFramebuffer> _framebuffer;

            void initialize() override;

            static const glm::vec3 DIRECTIONS[6];
            static const glm::vec3 UP_VECTORS[6];

        public:
            BakedCubeMap(glm::vec3 position, int bufferSize = 1024);

            /// @brief Get the cube map center position.
            /// @return Center postion
            glm::vec3 GetPosition();

            /// @brief Get camera used to render the cube map.
            /// @return Render camera
            std::shared_ptr<Camera> GetCamera();

            /// @brief Get framebuffer used to render the cube map textures.
            /// @return Render framebuffer
            std::shared_ptr<ColorDepthFramebuffer> GetFramebuffer();


            /// @brief Set the index of the six possible cube map directions.
            /// @param index Cube map direction index
            void SetDirection(int index);

            /// @brief Set the cube map center postion.
            /// @param position Center position to set
            void SetPosition(glm::vec3 position);

            /// @brief Bind as the active framebuffer and set it as the specified index of the cube map.
            /// @param index Cube map direction index
            void BindBuffer(int index);

            /// @brief Unbind framebuffer
            void UnbindBuffer();


            /// @brief Serialize data to JSON format.
            /// @return Serialized data
            json ToJson() override;

            /// @brief Deserialize data from JSON format.
            /// @param data BakedCubeMap data in JSON format
            /// @return Deserialized BakedCubeMap
            static std::shared_ptr<BakedCubeMap> FromJson(const json& data);
    };
    
}

#endif //CUBE_MAP_HPP