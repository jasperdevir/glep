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

#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GLEP/core/utility/file.hpp>
#include <GLEP/core/utility/print.hpp>

#include <GLEP/core/color.hpp>

#include <string>
#include <vector>
#include <filesystem>

#include <nlohmann/json.hpp>
#include <glad/glad.h>
#include "GLFW/glfw3.h"

namespace GLEP{

    using json = nlohmann::ordered_json;
    
    enum class TextureType{
        DIFFUSE,
        SPECULAR,
        NORMAL,
        DISP
    };

    enum class TextureWrap{
        REPEAT = 0x2901,
        MIRRORED_REPEAT = 0x8370,
        CLAMP_EDGE = 0x812F,
        CLAMP_BORDER = 0x812D
    };

    enum class TextureFilter{
        NEAREST = 0x2600,
        LINEAR = 0x2601,
        NEAREST_NEAREST = 0x2700,
        LINEAR_NEAREST = 0x2701,
        NEAREST_LINEAR = 0x2702,
        LINEAR_LINEAR = 0x2703
    };

    class Texture{
        private:
            unsigned int _ID;
            int _width;
            int _height;
            int _nrChannels;

            std::filesystem::path _filePath;
            TextureType _type;

            void initialize(unsigned char *data);

        public:

            Texture();
            Texture(std::filesystem::path filePath, TextureType type = TextureType::DIFFUSE);
            ~Texture();

            /// @brief Get width dimension in pixels
            /// @return Width
            int GetWidth();

            /// @brief Get height dimension in pixels
            /// @return Height
            int GetHeight();

            /// @brief Get the number of color channels.
            /// @return Color channels
            int GetNrChannels();

            /// @brief Get the image file path.
            /// @return Image file path
            std::filesystem::path GetFilePath();

            /// @brief Get the type of texture.
            /// @return Type
            TextureType GetType();

            /// @brief Get the buffer ID.
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


            /// @brief Bind as the active texture based on its type.
            void Bind();


            /// @brief Serialize data to JSON format.
            /// @return Serialized data
            json ToJson();

            /// @brief Deserialize data from JSON format.
            /// @param data Texture data in JSON format
            /// @return Deserialized Texture
            static std::shared_ptr<Texture> FromJson(const json& data);
    
    };

    struct TextureMap {
        std::shared_ptr<Texture> Diffuse;
        std::shared_ptr<Texture> Specular;
        std::shared_ptr<Texture> Normal;
        std::shared_ptr<Texture> Disp;

        TextureMap(std::shared_ptr<Texture> diffuse, std::shared_ptr<Texture> specular, std::shared_ptr<Texture> normal, std::shared_ptr<Texture> disp){
            Diffuse = diffuse;
            Specular = specular;
            Normal = normal;
            Disp = disp;
        }

        /// @brief Bind all textures as active texture based on their type.
        void Bind(){
            Diffuse->Bind();
            Specular->Bind();
            Normal->Bind();
            Disp->Bind();
        }
    };
    
}

#endif //TEXTURE_HPP