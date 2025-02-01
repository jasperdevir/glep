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

#include <GLEP/core/object_component.hpp>

namespace GLEP{

    ObjectComponent::ObjectComponent(){}

    void ObjectComponent::Initialize(std::weak_ptr<SceneObject> parent){
        _parent = parent;
    }

    std::weak_ptr<SceneObject> ObjectComponent::GetParent(){ return _parent; }

    void ObjectComponent::Start(){}
    void ObjectComponent::Update(){}

}