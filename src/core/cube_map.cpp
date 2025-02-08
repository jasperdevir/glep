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

#include <GLEP/core/cube_map.hpp>

#include "stb/stb_image.h"

namespace GLEP{
    CubeMap::CubeMap() {}

    CubeMap::~CubeMap() {
        glDeleteTextures(1, &_ID);
    }

    void CubeMap::Bind(){
        glActiveTexture(GL_TEXTURE0 + 6);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _ID);
    }

    unsigned int CubeMap::GetID(){
        return _ID;
    }

    int CubeMap::GetWidth(){
        return _width;
    }

    int CubeMap::GetHeight(){
        return _height;
    }

    int CubeMap::GetNrChannels(){
        return _nrChannels;
    }

    void CubeMap::SetWrap(TextureWrap wrap){
        glBindTexture(GL_TEXTURE_CUBE_MAP, _ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)wrap);
    }

    void CubeMap::SetWrap(TextureWrap s, TextureWrap t){
        glBindTexture(GL_TEXTURE_CUBE_MAP, _ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (int)s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (int)t);
    }

    void CubeMap::SetBorderColor(Color color){
        glBindTexture(GL_TEXTURE_CUBE_MAP, _ID);
        float borderColor[4] = {color.r, color.g, color.b, color.a};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  
    }

    void CubeMap::SetFilter(TextureFilter filter){
        glBindTexture(GL_TEXTURE_CUBE_MAP, _ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)filter);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    void CubeMap::SetFilter(TextureFilter min, TextureFilter mag){
        glBindTexture(GL_TEXTURE_CUBE_MAP, _ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (int)min);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (int)mag);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    std::shared_ptr<CubeMap> CubeMap::FromJson(const json& data){
        std::shared_ptr<CubeMap> result;
        if(data["type"] == "texture_cube_map"){
            result = TextureCubeMap::FromJson(data);
        } else if (data["type"] == "baked_cube_map"){
            result = BakedCubeMap::FromJson(data);
        } else {
            throw std::invalid_argument("[Invalid JSON format]-> Unknown CubeMap type: " + data["type"]);
        }

        return result;
    }

    TextureCubeMap::TextureCubeMap(std::filesystem::path cubeMapFilePath){
        bool success = true;
        std::ifstream cubeMapFile;
        cubeMapFile.open(cubeMapFilePath);

        std::string line;
        while (std::getline(cubeMapFile, line)) {
            if (line.empty()) continue;;

            std::filesystem::path path = File::DIRECTORY / line;

            if (!std::filesystem::exists(path)) {
                Print(PrintCode::ERROR, "CUBE_MAP", "Failed to import file at " + path.string());
                success = false;
                break;
            }

            _filePaths.push_back(path);
        }

        if (cubeMapFile.bad()) {
            Print(PrintCode::ERROR, "CUBE_MAP", "Failed to import CubeMap at " + cubeMapFilePath.string());
            success = false;
        }

        if(success) initialize();
    }

    TextureCubeMap::TextureCubeMap(std::vector<std::filesystem::path> filePaths){
        _filePaths = filePaths;

        initialize();
    }

    void TextureCubeMap::initialize(){
        glGenTextures(1, &_ID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _ID);

        if(_filePaths.size() != 6){
            Print(PrintCode::ERROR, "CUBE_MAP", "Cube map does not contain exactly 6 textures.");
            return;
        }

        for (unsigned int i = 0; i < _filePaths.size(); i++)
        {
            unsigned char *data = stbi_load(_filePaths[i].string().c_str(), &_width, &_height, &_nrChannels, 0);
            if (data)
            {
                GLenum format = GL_RGB;
                if (_nrChannels == 1)
                    format = GL_RED;
                else if (_nrChannels == 3)
                    format = GL_RGB;
                else if (_nrChannels == 4)
                    format = GL_RGBA;

                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);

                stbi_image_free(data);
            }
            else
            {
                Print(PrintCode::ERROR, "CUBE_MAP", "Cube map texture failed to load at path: " + _filePaths[i].string());
                stbi_image_free(data);
            }
        }

        #ifndef __APPLE__
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        #endif

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        
        if(_filePaths.size() > 0){
            _thumbTex = std::make_shared<Texture>(_filePaths[0]);
        }
    }

    std::vector<std::filesystem::path> TextureCubeMap::GetFilePaths(){
        return _filePaths;
    }

    std::shared_ptr<Texture> TextureCubeMap::GetThumbTex(){
        return _thumbTex;
    }

    json TextureCubeMap::ToJson(){
        json j;
        j["type"] = "texture_cube_map";
        j["paths"] = json::array();
        for(auto& p : _filePaths){
            j["paths"].push_back(p);
        }
        return j;
    }

    std::shared_ptr<TextureCubeMap> TextureCubeMap::FromJson(const json& data){
        std::vector<std::filesystem::path> paths;
        for(auto& p : data["paths"]){
            paths.push_back(p);
        }
        return std::make_shared<TextureCubeMap>(
            paths
        );
    }

    const glm::vec3 BakedCubeMap::DIRECTIONS[6] = {
        glm::vec3( 1,  0,  0),  
        glm::vec3(-1,  0,  0), 
        glm::vec3( 0,  1,  0), 
        glm::vec3( 0, -1,  0), 
        glm::vec3( 0,  0,  1),
        glm::vec3( 0,  0, -1)
    };

    const glm::vec3 BakedCubeMap::UP_VECTORS[6] = {
        Camera::UP,
        Camera::UP,
        -Camera::FRONT,
        Camera::FRONT,
        Camera::UP,
        Camera::UP
    };

    BakedCubeMap::BakedCubeMap(glm::vec3 position, int bufferSize){
        _camera = std::make_shared<PerspectiveCamera>(-90.0f, 1.0f, 0.01f, 100.0f);
        _camera->Position = position;
        _framebuffer = std::make_shared<ColorDepthFramebuffer>(glm::vec2((float)bufferSize));

        _width = bufferSize;
        _height = bufferSize;

        initialize();
    }

    void BakedCubeMap::initialize(){
        glGenTextures(1, &_ID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _ID);

        for (int i = 0; i < 6; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    void BakedCubeMap::SetDirection(int index){
        if(index < 0 || index > 5) return;
        
        glm::mat4 lookAt = glm::lookAt(glm::vec3(0.0f), DIRECTIONS[index], UP_VECTORS[index]);
        glm::mat3 rotationMatrix = glm::mat3(lookAt); 
        _camera->Rotation = glm::quat(rotationMatrix);

        _camera->UpdateTransformVectors();
    }   

    void BakedCubeMap::BindBuffer(int index){
        _framebuffer->Bind();
        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _ID, 0, index);
    }

    void BakedCubeMap::UnbindBuffer(){
        _framebuffer->Unbind();

        glBindTexture(GL_TEXTURE_CUBE_MAP, _ID);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    void BakedCubeMap::SetPosition(glm::vec3 position){
        _camera->Position = position;
    }

    glm::vec3 BakedCubeMap::GetPosition(){ return _camera->Position; }
    std::shared_ptr<Camera> BakedCubeMap::GetCamera(){ return _camera; }
    std::shared_ptr<ColorDepthFramebuffer> BakedCubeMap::GetFramebuffer(){ return _framebuffer; }

    json BakedCubeMap::ToJson(){
        json j;
        j["type"] = "baked_cube_map";
        j["position"] = Math::ToJson(GetPosition());
        j["buffer_size"] = GetFramebuffer()->GetWidth();
        return j;
    }

    std::shared_ptr<BakedCubeMap> BakedCubeMap::FromJson(const json& data){
        return std::make_shared<BakedCubeMap>(
            Math::Vec3FromJson(data["position"]),
            data["buffer_size"]
        );
    }

    DepthBakedCubeMap::DepthBakedCubeMap(glm::vec3 position, int bufferSize){
        _width = bufferSize;
        _height = bufferSize;

        _camera = std::make_shared<PerspectiveCamera>(90.0f, (float)_width / (float)_height, 0.01f, 25.0f);
        _framebuffer = std::make_shared<DepthFramebuffer>(glm::vec2(bufferSize));

        initialize();
    }

    void DepthBakedCubeMap::initialize(){
        glGenTextures(1, &_ID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _ID);

        for (int i = 0; i < 6; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer->GetBufferID());
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _ID, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0); 
    }

    std::shared_ptr<Camera> DepthBakedCubeMap::GetCamera(){
        return _camera;
    }

    std::shared_ptr<DepthFramebuffer> DepthBakedCubeMap::GetFramebuffer(){
        return _framebuffer;
    }

    void DepthBakedCubeMap::BindBuffer(){
        _framebuffer->Bind();
    }

    void DepthBakedCubeMap::UnbindBuffer(){
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindTexture(GL_TEXTURE_CUBE_MAP, _ID);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    json DepthBakedCubeMap::ToJson(){
        json j;
        return j;
    }
}