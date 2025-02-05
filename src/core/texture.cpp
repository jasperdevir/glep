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

#include <GLEP/core/texture.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace GLEP {
    Texture::Texture(){}

    Texture::Texture(std::filesystem::path filePath, TextureType type){
        _filePath = filePath;
        _type = type;

        unsigned char *data = stbi_load(_filePath.string().c_str(), &_width, &_height, &_nrChannels, 0);
        if(!data) {
            Print(PrintCode::ERROR, "TEXTURE", "Failed to load texture at: " + _filePath.string() + " -  Loading default texture");
            _filePath = File::GLEP_DEFUALT_TEXTURE;
        }
        
        initialize(data);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &_ID);
    }

    void Texture::initialize(unsigned char *data){
        glGenTextures(1, &_ID);

        GLenum format = GL_RGB;
        if (_nrChannels == 1)
            format = GL_RED;
        else if (_nrChannels == 3)
            format = GL_RGB;
        else if (_nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, _ID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::SetWrap(TextureWrap wrap){
        glBindTexture(GL_TEXTURE_2D, _ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)wrap);
    }

    void Texture::SetWrap(TextureWrap s, TextureWrap t){
        glBindTexture(GL_TEXTURE_2D, _ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)t);
    }

    void Texture::SetBorderColor(Color color){
        glBindTexture(GL_TEXTURE_2D, _ID);
        float borderColor[4] = {color.r, color.g, color.b, color.a};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  
    }

    void Texture::SetFilter(TextureFilter filter){
        glBindTexture(GL_TEXTURE_2D, _ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)filter);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void Texture::SetFilter(TextureFilter min, TextureFilter mag){
        glBindTexture(GL_TEXTURE_2D, _ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)mag);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void Texture::Bind(){
        glActiveTexture(GL_TEXTURE0 + (int)_type);
        glBindTexture(GL_TEXTURE_2D, _ID);
    }

    int Texture::GetWidth(){
        return _width;
    }

    int Texture::GetHeight(){
        return _height;
    }

    int Texture::GetNrChannels(){
        return _nrChannels;
    }

    std::filesystem::path Texture::GetFilePath(){
        return _filePath;
    }

    TextureType Texture::GetType(){
        return _type;
    }

    unsigned int Texture::GetID(){
        return _ID;
    }

    json Texture::ToJson(){
        json j;
        j["path"] = _filePath;
        j["texture_type"] = _type;
        return j;
    }

    std::shared_ptr<Texture> Texture::FromJson(const json& data){
        if(data.is_null()) 
            return nullptr;

        return std::make_shared<Texture>(
            data["path"],
            (TextureType) data["texture_type"]
        );
    }
    
}