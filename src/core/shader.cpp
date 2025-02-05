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

#include <GLEP/core/shader.hpp>

namespace GLEP {

    Shader::Shader(){}

    Shader::Shader(std::filesystem::path fsFilePath){
        _vsFilePath = File::GLEP_DEFAULT_VERTEX_SHADER;
        _fsFilePath = fsFilePath;

        initialize();
    }

    Shader::Shader(std::filesystem::path vsFilePath, std::filesystem::path fsFilePath){
        _vsFilePath = vsFilePath;
        _fsFilePath = fsFilePath;

        initialize();
    }

    Shader::Shader(std::filesystem::path vsFilePath, std::filesystem::path gsFilePath, std::filesystem::path fsFilePath){
        _vsFilePath = vsFilePath;
        _fsFilePath = fsFilePath;
        _gsFilePath = gsFilePath;

        initialize();
    }

    Shader::~Shader(){
        glDeleteProgram(_ID);
    }

    bool Shader::readFiles(){
        std::string vertexCode;
        std::string geometryCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream gShaderFile;
        std::ifstream fShaderFile;
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try 
        {
            vShaderFile.open(_vsFilePath.string());
            fShaderFile.open(_fsFilePath.string());

            std::stringstream vShaderStream, fShaderStream;

            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
                

            vShaderFile.close();
            fShaderFile.close();

            vertexCode   = vShaderStream.str();
            fragmentCode = fShaderStream.str();

            if(!_gsFilePath.empty()){
                gShaderFile.open(_gsFilePath.string());
                std::stringstream gShaderStream;
                gShaderStream << gShaderFile.rdbuf();
                gShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::ifstream::failure& e)
        {
            Print(PrintCode::ERROR, "SHADER", std::string("Failed to read file: ") + e.what());
            return false;
        }
        _vsSrc = vertexCode;
        _fsSrc = fragmentCode;
        _gsSrc = geometryCode;

        return true;
    }

    bool Shader::checkCompileErrors(unsigned int shader, std::string type){
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                
                std::filesystem::path path;
                if(type == "VERTEX") path = _vsFilePath;
                else if (type == "FRAGMENT") path = _fsFilePath;
                else path = _gsFilePath;

                Print(PrintCode::ERROR, "SHADER", "Failed to compile " + type + " shader at " + path.string() + ": \n" + infoLog + "\n -- --------------------------------------------------- -- ");
                return false;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                Print(PrintCode::ERROR, "SHADER", "Failed to link shader " + type + ": \n" + infoLog + "\n -- --------------------------------------------------- -- ");
                return false;
            }
        }
        return true;
    }


    bool Shader::initialize(){
        readFiles();

        unsigned int vertex, fragment, geometry;
        
        /* VERTEX SHADER */
        vertex = glCreateShader(GL_VERTEX_SHADER);
        const char* vsSource = _vsSrc.c_str();
        glShaderSource(vertex, 1, &vsSource, NULL);
        glCompileShader(vertex);
        if(!checkCompileErrors(vertex, "VERTEX")) {
            return false;
        }

        /* FRAGEMENT SHADER */
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fsSource = _fsSrc.c_str();
        glShaderSource(fragment, 1, &fsSource, NULL);
        glCompileShader(fragment);
        if(!checkCompileErrors(fragment, "FRAGMENT")) {
            return false;
        }

        /* GEOMETRY SHADER */
        if(!_gsFilePath.empty()){
            geometry = glCreateShader(GL_GEOMETRY_SHADER);
            const char* gsSource = _gsSrc.c_str();
            glShaderSource(geometry, 1, &gsSource, NULL);
            glCompileShader(geometry);
            if(!checkCompileErrors(geometry, "GEOMETRY")){
                return false;
            }
        }

        /* SHADER PROGRAM*/
        _ID = glCreateProgram();
        glAttachShader(_ID, vertex);
        glAttachShader(_ID, fragment);
        if(!_gsFilePath.empty())
            glAttachShader(_ID, geometry);

        glLinkProgram(_ID);
        if(!checkCompileErrors(_ID, "PROGRAM")) {
            return false;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if(!_gsFilePath.empty())
            glDeleteShader(geometry);

        glUseProgram(_ID);

        return true;
    }

    void Shader::Use(){
        glUseProgram(_ID);
    }

    unsigned int Shader::GetID(){
        return _ID;
    }

    std::filesystem::path Shader::GetVsPath(){
        return _vsFilePath;
    }

    std::filesystem::path Shader::GetFsPath(){
        return _fsFilePath;
    }

    std::filesystem::path Shader::GetGsPath(){
        return _gsFilePath;
    }
}