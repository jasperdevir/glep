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

#ifndef OBJECT_COMPONENT_HPP
#define OBJECT_COMPONENT_HPP

#include <GLEP/core/scene_object.hpp>

#include <nlohmann/json.hpp>
#include <memory>

namespace GLEP{

    using json = nlohmann::ordered_json;

    class SceneObject;

    class ObjectComponent{
        protected:
            std::weak_ptr<SceneObject> _parent;

        public:
            ObjectComponent();

            /// @brief Attach a parent object to this component.
            /// @param parent Parent to attach
            void Initialize(std::weak_ptr<SceneObject> parent);


            /// @brief Get the parent object.
            /// @return Parent object
            std::weak_ptr<SceneObject> GetParent();


            /// @brief Function to be called on the first frame of rendering a scene.
            virtual void Start();

            /// @brief Function to be called each frame after the first while rendering a scene.
            virtual void Update();


            /// @brief Serialize data to JSON format.
            /// @return Serialized data
            virtual json ToJson() = 0;
    };
    
}


#endif //OBJECT_COMPONENT_HPP