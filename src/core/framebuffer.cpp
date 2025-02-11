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
    }

    Framebuffer::~Framebuffer(){
        glDeleteFramebuffers(1, &_framebuffer);
    }

    unsigned int Framebuffer::GetBufferID(){ return _framebuffer; }
    int Framebuffer::GetWidth(){ return _width; }
    int Framebuffer::GetHeight(){ return _height; }


    void Framebuffer::SetResolution(glm::vec2 resolution){
        _width = (int)resolution.x;
        _height = (int)resolution.y;

        initialize();
    }

    void Framebuffer::Unbind(){
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    ColorFramebuffer::ColorFramebuffer(){}

    ColorFramebuffer::ColorFramebuffer(glm::vec2 resolution)
    : Framebuffer(resolution){
        initialize();
    }

    ColorFramebuffer::~ColorFramebuffer(){
        glDeleteTextures(1, &_colorBufferID);
    }

    void ColorFramebuffer::initialize(){
        glGenFramebuffers(1, &_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
        
        glGenTextures(1, &_colorBufferID);
        glBindTexture(GL_TEXTURE_2D, _colorBufferID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorBufferID, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Print(PrintCode::ERROR, "COLOR_FRAMEBUFFER", "Framebuffer is not complete.");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    unsigned int ColorFramebuffer::GetColorBufferID(){
        return _colorBufferID;
    }

    void ColorFramebuffer::OverrideColorBuffer(unsigned int colorBuffer){
        _colorBufferID = colorBuffer;
    }

    void ColorFramebuffer::Bind(){
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    }

    void ColorFramebuffer::BindResult(){
        glActiveTexture(GL_TEXTURE0 + 4);
        glBindTexture(GL_TEXTURE_2D, _colorBufferID);
    }

    DepthFramebuffer::DepthFramebuffer(){}

    DepthFramebuffer::DepthFramebuffer(glm::vec2 resolution)
    : Framebuffer(resolution){
        initialize();
    }

    DepthFramebuffer::~DepthFramebuffer(){
        glDeleteTextures(1, &_depthBufferID);
    }

    void DepthFramebuffer::initialize(){
        glGenFramebuffers(1, &_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

        glGenTextures(1, &_depthBufferID);
        glBindTexture(GL_TEXTURE_2D, _depthBufferID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthBufferID, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Print(PrintCode::ERROR, "FRAMEBUFFER", "Framebuffer is not complete.");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    unsigned int DepthFramebuffer::GetDepthBufferID(){
        return _depthBufferID;
    }

    void DepthFramebuffer::OverrideDepthBuffer(unsigned int depthBuffer){
        _depthBufferID = depthBuffer;
    }

    void DepthFramebuffer::Bind(){
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    }

    void DepthFramebuffer::BindResult(){
        glActiveTexture(GL_TEXTURE0 + 5);
        glBindTexture(GL_TEXTURE_2D, _depthBufferID);
    }

    ColorDepthFramebuffer::ColorDepthFramebuffer(){}

    ColorDepthFramebuffer::ColorDepthFramebuffer(glm::vec2 resolution)
    : Framebuffer(resolution){
        initialize();
    }

    ColorDepthFramebuffer::~ColorDepthFramebuffer(){
        glDeleteTextures(1, &_colorBufferID);
        glDeleteTextures(1, &_depthBufferID);
    }

    void ColorDepthFramebuffer::initialize(){
        glGenFramebuffers(1, &_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
        
        // COLOR
        glGenTextures(1, &_colorBufferID);
        glBindTexture(GL_TEXTURE_2D, _colorBufferID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorBufferID, 0);

        // DEPTH
        glGenTextures(1, &_depthBufferID);
        glBindTexture(GL_TEXTURE_2D, _depthBufferID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthBufferID, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            Print(PrintCode::ERROR, "FRAMEBUFFER", "Framebuffer is not complete.");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    unsigned int ColorDepthFramebuffer::GetColorBufferID(){
        return _colorBufferID;
    }

    unsigned int ColorDepthFramebuffer::GetDepthBufferID(){
        return _depthBufferID;
    }

    void ColorDepthFramebuffer::OverrideColorBuffer(unsigned int colorBuffer){
        _colorBufferID = colorBuffer;
    }

    void ColorDepthFramebuffer::OverrideDepthBuffer(unsigned int depthBuffer){
        _depthBufferID = depthBuffer;
    }

    void ColorDepthFramebuffer::Bind(){
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

        GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT };
        glDrawBuffers(2, attachments);
    }

    void ColorDepthFramebuffer::BindResult(){
        glActiveTexture(GL_TEXTURE0 + 4);
        glBindTexture(GL_TEXTURE_2D, _colorBufferID);

        glActiveTexture(GL_TEXTURE0 + 5);
        glBindTexture(GL_TEXTURE_2D, _depthBufferID);
    }

    GBuffer::GBuffer(){}

    GBuffer::GBuffer(glm::vec2 resolution)
    : Framebuffer(resolution){
        initialize();
    }

    void GBuffer::initialize(){
        glGenFramebuffers(1, &_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
        
        // POSITION
        glGenTextures(1, &_positionBufferID);
        glBindTexture(GL_TEXTURE_2D, _positionBufferID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _width, _height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _positionBufferID, 0);

        // NORMAL
        glGenTextures(1, &_normalBufferID);
        glBindTexture(GL_TEXTURE_2D, _normalBufferID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _width, _height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _normalBufferID, 0);

        // DIFFUSE
        glGenTextures(1, &_diffuseBufferID);
        glBindTexture(GL_TEXTURE_2D, _diffuseBufferID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, _diffuseBufferID, 0);

        // SPECULAR
        glGenTextures(1, &_specularBufferID);
        glBindTexture(GL_TEXTURE_2D, _specularBufferID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _width, _height, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, _specularBufferID, 0);


        unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glDrawBuffers(4, attachments);
    }

    unsigned int GBuffer::GetPositionBufferID(){
        return _positionBufferID;
    }

    unsigned int GBuffer::GetNormalBufferID(){
        return _normalBufferID;
    }

    unsigned int GBuffer::GetDiffuseBufferID(){
        return _diffuseBufferID;
    }

    unsigned int GBuffer::GetSpecularBufferID(){
        return _specularBufferID;
    }

    void GBuffer::OverridePositionBufferID(unsigned int positionBuffer){
        _positionBufferID = positionBuffer;
    }

    void GBuffer::OverrideNormalBufferID(unsigned int normalBuffer){
        _normalBufferID = normalBuffer;
    }

    void GBuffer::OverrideDiffuseBufferID(unsigned int diffuseBuffer){
        _diffuseBufferID = diffuseBuffer;
    }

    void GBuffer::OverrideSpecularBufferID(unsigned int specularBuffer){
        _specularBufferID = specularBuffer;
    }
    
    void GBuffer::Bind(){
        glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    }

    void GBuffer::BindResult(){
        glActiveTexture(GL_TEXTURE0 + 4);
        glBindTexture(GL_TEXTURE_2D, _positionBufferID);

        glActiveTexture(GL_TEXTURE0 + 5);
        glBindTexture(GL_TEXTURE_2D, _normalBufferID);

        glActiveTexture(GL_TEXTURE0 + 6);
        glBindTexture(GL_TEXTURE_2D, _diffuseBufferID);

        glActiveTexture(GL_TEXTURE0 + 7);
        glBindTexture(GL_TEXTURE_2D, _specularBufferID);
    }

        
}