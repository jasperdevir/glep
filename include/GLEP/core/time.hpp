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

#ifndef TIME_HPP
#define TIME_HPP

#include <glad/glad.h>
#include "GLFW/glfw3.h"

namespace GLEP{

    class Time{
        private:
            static double lastElapsedTime;
            static double elapsedTime;
            static double deltaTime;
            static double timeScale;
            static int frameCount;
            static double lastFpsUpdateTime;
            static int fps;

        public:
            /// @brief Get the total elapsed time the application has been running.
            /// @return Total elapsed time
            static double GetElapsedTime();

            /// @brief Get the total elapsed time the application has been running.
            /// @return Total elapsed time
            static float GetElapsedTimeF();

            /// @brief Get the total time since the last update.
            /// @return Delta time
            static double GetDeltaTime();

            /// @brief Get the total time since the last update.
            /// @return Delta time
            static float GetDeltaTimeF();

            /// @brief Get the current amount of update frames per second.
            /// @return Frames per second
            static int GetFPS();


            /// @brief Update both delta time and FPS (Ideally, should be called every frame).
            static void Update();
    };

}



#endif //TIME_HPP