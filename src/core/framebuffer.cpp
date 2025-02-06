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

#include <GLEP/core/framebuffer.hpp>

namespace GLEP {
    
    Framebuffer::Framebuffer(){}

    Framebuffer::Framebuffer(glm::vec2 resolution){
        _width = (int)resolution.x;
        _height = (int)resolution.y;

        initialize();
    }

    Framebuffer::~Framebuffer(){
        glDeleteFramebuffers(1, &_framebuffer);
        glDeleteTextures(1, &_colorBuffer);
        glDeleteTextures(1, &_depthBuffer);
    }

    void Framebuffer::initialize(){
        glGenFramebuffers(1, &_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
        
        glGenTextures(1, &_colorBuffer);
        glBindTexture(GL_TEXTURE_2D, _colorBuffer);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorBuffer, 0);
                
        glGenTextures(1, &_depthBuffer);
        glBindTexture(GL_TEXTURE_2D, _depthBuffer);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthBuffer, 0);
    
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Print(PrintCode::ERROR, "FRAMEBUFFER", "Framebuffer is not complete.");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    unsigned int Framebuffer::GetBufferID(){ return _framebuffer; }
    unsigned int Framebuffer::GetColorBufferID(){ return _colorBuffer; }
    unsigned int Framebuffer::GetDepthBufferID(){ return _depthBuffer; }
    int Framebuffer::GetWidth(){ return _width; }
    int Framebuffer::GetHeight(){ return _height; }

    void Framebuffer::SetDepthBufferID(unsigned int depthBuffer){
        _depthBuffer = depthBuffer;
    }

    void Framebuffer::SetResolution(glm::vec2 resolution){
        _width = (int)resolution.x;
        _height = (int)resolution.y;

        initialize();
    }

    void Framebuffer::Bind(){
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

        GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT };
        glDrawBuffers(2, attachments);
    }

    void Framebuffer::Unbind(){
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::BindResult(){
        glActiveTexture(GL_TEXTURE0 + 4);
        glBindTexture(GL_TEXTURE_2D, _colorBuffer);

        glActiveTexture(GL_TEXTURE0 + 5);
        glBindTexture(GL_TEXTURE_2D, _depthBuffer);
    }

    DepthFramebuffer::DepthFramebuffer(glm::vec2 resolution){
        _width = (int)resolution.x;
        _height = (int)resolution.y;

        initialize();
    }

    DepthFramebuffer::~DepthFramebuffer(){
        glDeleteFramebuffers(1, &_framebuffer);
        glDeleteTextures(1, &_depthBuffer);
    }

    void DepthFramebuffer::initialize(){
        glGenFramebuffers(1, &_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

        glGenTextures(1, &_depthBuffer);
        glBindTexture(GL_TEXTURE_2D, _depthBuffer);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthBuffer, 0);
    
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Print(PrintCode::ERROR, "FRAMEBUFFER", "Framebuffer is not complete.");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    int DepthFramebuffer::GetWidth(){
        return _width;
    }

    int DepthFramebuffer::GetHeight(){
        return _height;
    }

    unsigned int DepthFramebuffer::GetBufferID(){
        return _framebuffer;
    }

    unsigned int DepthFramebuffer::GetDepthBufferID(){
        return _depthBuffer;
    }

    void DepthFramebuffer::Bind(){
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    }

    void DepthFramebuffer::Unbind(){
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void DepthFramebuffer::BindResult(){
        glActiveTexture(GL_TEXTURE0 + 5);
        glBindTexture(GL_TEXTURE_2D, _depthBuffer);
    }
        
}