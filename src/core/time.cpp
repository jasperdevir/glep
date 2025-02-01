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

#include <GLEP/core/time.hpp>

namespace GLEP {

    double Time::elapsedTime = glfwGetTime();
    double Time::lastElapsedTime = elapsedTime;
    double Time::lastFpsUpdateTime = elapsedTime;
    double Time::deltaTime = 0.0f;
    int Time::frameCount = 0;
    int Time::fps = 0;

    void Time::Update(){
        lastElapsedTime = elapsedTime;
        elapsedTime = glfwGetTime();

        deltaTime = elapsedTime - lastElapsedTime;
        frameCount++;

        if (elapsedTime - lastFpsUpdateTime >= 1.0f) {
            fps = frameCount; 
            frameCount = 0;  
            lastFpsUpdateTime = elapsedTime;
        }
    }

    double Time::GetElapsedTime(){ return elapsedTime; }
    float Time::GetElapsedTimeF(){ return (float)elapsedTime; }
    double Time::GetDeltaTime(){ return deltaTime; }
    float Time::GetDeltaTimeF(){ return (float)deltaTime; }
    int Time::GetFPS(){ return fps; }

}