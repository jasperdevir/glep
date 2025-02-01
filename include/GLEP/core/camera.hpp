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

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLEP/core/scene_object.hpp>

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GLEP{

    enum CameraType{
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

    class Camera : public SceneObject{
        protected:
            CameraType _type;

            glm::mat4 _projectionMatrix;
            glm::mat4 _viewMatrix;

            glm::vec3 _front;
            glm::vec3 _right;
            glm::vec3 _up;

            float _aspectRatio;
            float _nearPlane;
            float _farPlane;

            void updateVectors();
            void updateViewMatrix();

        public:
            static const glm::vec3 FRONT;
            static const glm::vec3 RIGHT;
            static const glm::vec3 UP;

            Camera(CameraType type);

            /// @brief Get the camera type.
            /// @return Type
            CameraType GetType();

            /// @brief Get the projection matrix.
            /// @return Projection matrix
            glm::mat4 GetProjectionMatrix();

            /// @brief Get the view matrix.
            /// @return View matrix
            glm::mat4 GetViewMatrix();

            /// @brief Get the current front facing vector.
            /// @return Current front facing vector
            glm::vec3 GetFront();

            /// @brief Get the current right facing vector.
            /// @return Current right facing vector
            glm::vec3 GetRight();

            /// @brief Get the current up facing vector.
            /// @return Current up facing vector
            glm::vec3 GetUp();

            /// @brief Get if the cursor is currently locked.
            /// @return If the cursor is currently locked
            bool GetCursorLock();

            /// @brief Get the aspect ratio.
            /// @return Aspect ratio
            float GetAspectRatio();

            /// @brief Get the near plane.
            /// @return Near plane
            float GetNearPlane();

            /// @brief Get the far plane.
            /// @return Far plane
            float GetFarPlane();


            /// @brief Set the aspect ratio and update the projection matrix.
            /// @param aspectRatio Aspect ratio to set
            void SetAspectRatio(float aspectRatio);
            
            /// @brief Set the near plane and update the projection matrix.
            /// @param nearPlane Near plane to set
            void SetNearPlane(float nearPlane);
            
            /// @brief Set the far plane and update the projection matrix.
            /// @param farPlane Far plane to set
            void SetFarPlane(float farPlane);

            /// @brief Update the projection matrix.
            virtual void UpdateProjection() = 0;
    };

    class PerspectiveCamera : public Camera{
        private:
            float _fov;

        public:
            PerspectiveCamera(float fov, float aspectRatio, float nearPlane, float farPlane);

            /// @brief Get the field of view.
            /// @return Field of view
            float GetFov();

            /// @brief Set the field of view.
            /// @param fov Field of view to set
            void SetFov(float fov);


            /// @brief Set all the projection values and update the projection matrix.
            /// @param fov Field of view to set
            /// @param aspectRatio Aspect ratio to set
            /// @param nearPlane Near plane to set
            /// @param farPlane Far plane to set
            void SetProjection(float fov, float aspectRatio, float nearPlane, float farPlane);

            /// @brief Update the projection matrix.
            void UpdateProjection();
    };

    class OrthographicCamera : public Camera{
        private:
            float _size;

        public:

            OrthographicCamera(float size, float aspectRatio, float nearPlane, float farPlane);

            /// @brief Get the projection size.
            /// @return Projection size
            float GetSize();
            
            /// @brief Set the projection size.
            /// @param size Projection size
            void SetSize(float size);


            /// @brief Set all projection values and update the projection matrix.
            /// @param size Projection size to set
            /// @param aspectRatio Aspect ratio to set
            /// @param nearPlane Near plane to set
            /// @param farPlane Far plane to set
            void SetProjection(float size, float aspectRatio, float nearPlane, float farPlane);

            /// @brief Update the projection matrix.
            void UpdateProjection();
    };

}

#endif //CAMERA_HPP

