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

#include "GLEP/core/camera.hpp"

namespace GLEP {

    const glm::vec3 Camera::FRONT = glm::vec3(0.0f, 0.0f, 1.0f);
    const glm::vec3 Camera::RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
    const glm::vec3 Camera::UP = glm::vec3(0.0f, 1.0f, 0.0f);

    Camera::Camera(CameraType type){
        _front = Camera::FRONT;
        _right = Camera::RIGHT;
        _up = Camera::UP;

        _viewMatrix = glm::mat4(1.0f);
        _projectionMatrix = glm::mat4(1.0f);
    }

    void Camera::updateVectors(){
        _front = FRONT * _worldRotation;
        _right = RIGHT * _worldRotation;
        _up = UP * _worldRotation;
    }

    void Camera::updateViewMatrix(){ 
        glm::mat4 rotationMat = glm::mat4_cast(_worldRotation);
        glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), -_worldPosition);
        
        _viewMatrix = rotationMat * translateMat;
    }

    CameraType Camera::GetType(){ return _type; }

    glm::mat4 Camera::GetProjectionMatrix(){
        return _projectionMatrix;
    }

    glm::mat4 Camera::GetViewMatrix(){
        updateViewMatrix();
        return _viewMatrix;
    }

    glm::vec3 Camera::GetFront() {
        updateVectors();
        return _front;
    }

    glm::vec3 Camera::GetRight() {
        updateVectors();
        return _right;
    }

    glm::vec3 Camera::GetUp() {
        updateVectors();
        return _up;
    }

    float Camera::GetAspectRatio() { return _aspectRatio; }
    float Camera::GetNearPlane() { return _nearPlane; }
    float Camera::GetFarPlane() { return _farPlane; }

    void Camera::SetAspectRatio(float aspectRatio){
        _aspectRatio = aspectRatio;
        UpdateProjection();
    }

    void Camera::SetNearPlane(float nearPlane){
        _nearPlane = nearPlane;
        UpdateProjection();
    }

    void Camera::SetFarPlane(float farPlane){
        _farPlane = farPlane;
        UpdateProjection();
    }


    PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane)
    : Camera(PERSPECTIVE) {
        SetProjection(fov, aspectRatio, nearPlane, farPlane);
    }

    float PerspectiveCamera::GetFov() { return _fov; }

    void PerspectiveCamera::SetFov(float fov){
        _fov = fov;
        UpdateProjection();
    }

    void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane){
        _fov = fov;
        _aspectRatio = aspectRatio;
        _nearPlane = nearPlane;
        _farPlane = farPlane;

        UpdateProjection();
    }

    void PerspectiveCamera::UpdateProjection(){
        _projectionMatrix = glm::perspective(glm::radians(_fov), _aspectRatio, _nearPlane, _farPlane);
    }

    OrthographicCamera::OrthographicCamera(float size, float aspectRatio, float nearPlane, float farPlane)
    : Camera(ORTHOGRAPHIC){
        SetProjection(size, aspectRatio, nearPlane, farPlane);
    }

    float OrthographicCamera::GetSize() { return _size; }

    void OrthographicCamera::SetSize(float size){
        _size = size;
        UpdateProjection();
    }

    void OrthographicCamera::SetProjection(float size, float aspectRatio, float nearPlane, float farPlane){
        _size = size;
        _nearPlane = nearPlane;
        _farPlane = farPlane;
        _aspectRatio = aspectRatio;

        UpdateProjection();
    }

    void OrthographicCamera::UpdateProjection(){
        _projectionMatrix = glm::ortho(-_size * _aspectRatio, _size * _aspectRatio, -_size, _size, _nearPlane, _farPlane);
    }

}