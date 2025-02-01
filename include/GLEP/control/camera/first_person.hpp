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

#ifndef FIRST_PERSON_HPP
#define FIRST_PERSON_HPP

#include <GLEP/core/camera.hpp>
#include <GLEP/core/time.hpp>
#include <GLEP/core/input.hpp>

#include <memory>

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>

namespace GLEP::Control{

    class FirstPersonController {
        private:
            std::weak_ptr<Camera> _camera;
            float _moveSpeed;
            float _lookSpeed;
            bool _invertMouseX;
            bool _invertMouseY;

            float _pitch = 0.0f;
            float _yaw = 0.0f;

            bool _isCameraLockDown = false;
            CursorState _cursorState = CursorState::NORMAL;

            void processInput(const std::shared_ptr<Window>& window);
            void processMouseMovement(const std::shared_ptr<Camera>& camera, glm::vec2 mouseDelta);

        public:
            FirstPersonController(std::weak_ptr<Camera> camera, float moveSpeed, float lookSpeed, bool invertMouseX = false, bool invertMouseY = false);

            /// @brief Get the current cursor state.
            /// @return Current cursor state
            CursorState GetCursorState();
            

            /// @brief Flip the current cursor state (CursorState::NORMAL or CursorState::DISABLED).
            /// @param window Active window
            void FlipCursorState(const std::shared_ptr<Window>& window);


            /// @brief Update the controls.
            /// @param window Active window
            void Update(const std::shared_ptr<Window>& window);

    };

}



#endif //FIRST_PERSON_HPP