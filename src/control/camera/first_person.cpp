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

#include <GLEP/control/camera/first_person.hpp>

namespace GLEP::Control{

    FirstPersonController::FirstPersonController(std::weak_ptr<Camera> camera, float moveSpeed, float lookSpeed, bool invertMouseX, bool invertMouseY){        
        _camera = camera;
        _moveSpeed = moveSpeed;
        _lookSpeed = lookSpeed;
        _invertMouseX = invertMouseX;
        _invertMouseY = invertMouseY;
        _yaw = 0.0f;
        _pitch = 0.0f;
        
        std::shared_ptr<Camera> cameraPtr = _camera.lock();
        if(cameraPtr){
            glm::vec3 euler = glm::eulerAngles(cameraPtr->Rotation);
            _yaw = glm::degrees(euler.y);

            //Pitch doesn't follow initial camera pitch
            _pitch = glm::degrees(euler.x);
            _pitch = glm::clamp(_pitch, -89.0f, 89.0f);
        }

    }

    CursorState FirstPersonController::GetCursorState(){ return _cursorState; }

    void FirstPersonController::FlipCursorState(const std::shared_ptr<Window>& window){
        _cursorState = _cursorState == CursorState::NORMAL ? CursorState::DISABLED : CursorState::NORMAL;
        Input::SetCursorState(window, _cursorState);
    }

    void FirstPersonController::processInput(const std::shared_ptr<Window>& window){
        std::shared_ptr<Camera> camera = _camera.lock();
        if(!camera) return;

        float moveSpeed = _moveSpeed;

        if(Input::GetInput(window, KeyCode::Q) == InputState::PRESS && !_isCameraLockDown)
            FlipCursorState(window);
            _isCameraLockDown = true;

        if(Input::GetInput(window, KeyCode::Q) == InputState::RELEASE)
            _isCameraLockDown = false;

        if(Input::GetInput(window, KeyCode::LEFT_SHIFT) == InputState::PRESS)
            moveSpeed *= 2.0f;

        if (Input::GetInput(window, KeyCode::W) == InputState::PRESS)
            camera->Position += -camera->GetFront() * moveSpeed * Time::GetDeltaTimeF();
        if (Input::GetInput(window, KeyCode::S) == InputState::PRESS)
            camera->Position += camera->GetFront() * moveSpeed * Time::GetDeltaTimeF();
        if (Input::GetInput(window, KeyCode::A) == InputState::PRESS)
            camera->Position += -camera->GetRight() * moveSpeed * Time::GetDeltaTimeF();
        if (Input::GetInput(window, KeyCode::D) == InputState::PRESS)
            camera->Position += camera->GetRight() * moveSpeed * Time::GetDeltaTimeF();

        glm::vec2 mouseDelta = Input::GetMouseDelta();
        mouseDelta.x = _invertMouseX ? -mouseDelta.x : mouseDelta.x;
        mouseDelta.y = _invertMouseY ? -mouseDelta.y : mouseDelta.y;
        processMouseMovement(camera, mouseDelta);
    }

    void FirstPersonController::processMouseMovement(const std::shared_ptr<Camera>& camera, glm::vec2 mouseDelta){
        if(_cursorState == CursorState::NORMAL) return;

        _yaw += mouseDelta.x * _lookSpeed;
        _pitch += -mouseDelta.y * _lookSpeed;

        _pitch = glm::clamp(_pitch, -89.0f, 89.0f);

        glm::quat yawQuat = glm::angleAxis(glm::radians(_yaw), Camera::UP);
        glm::quat pitchQuat = glm::angleAxis(glm::radians(_pitch), -Camera::RIGHT);

        camera->Rotation = pitchQuat * yawQuat;
    }

    void FirstPersonController::Update(const std::shared_ptr<Window>& window){
        processInput(window);
    }

}
