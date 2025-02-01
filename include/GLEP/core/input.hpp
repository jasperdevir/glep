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

#ifndef INPUT_HPP
#define INPUT_HPP

#include <GLEP/core/window.hpp>

#include <memory>
#include <functional>

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>

namespace GLEP{

    enum class InputState{
        RELEASE = 0,
        PRESS = 1
    };

    enum class KeyCode{
        SPACE = 32,
        APOSTROPHE = 39, /* , */
        KEY_1 = 49,
        KEY_2 = 50,
        KEY_3 = 51,
        KEY_4 = 52,
        KEY_5 = 53,
        KEY_6 = 54,
        KEY_7 = 55,
        KEY_8 = 56,
        KEY_9 = 57,
        SEMICOLON = 59, /* ; */
        EQUAL = 61, /* = */
        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,
        LEFT_BRACKET = 91, /* [ */
        BACKSLASH = 92, /* \ */
        RIGHT_BRACKET = 93, /* ] */
        GRAVE_ACCENT = 96, /* ` */
        ESCAPE = 256,
        ENTER = 257,
        TAB = 258,
        BACKSPACE = 259,
        INSERT = 260,
        DELETE = 261,
        RIGHT = 262,
        LEFT = 263,
        DOWN = 264,
        UP = 265,
        PAGE_UP = 266,
        PAGE_DOWN = 267,
        HOME = 268,
        END = 269,
        CAPS_LOCK = 280,
        SCROLL_LOCK = 281,
        NUM_LOCK = 282,
        PRINT_SCREEN = 283,
        PAUSE = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,
        KP_0 = 320,
        KP_1 = 321,
        KP_2 = 322,
        KP_3 = 323,
        KP_4 = 324,
        KP_5 = 325,
        KP_6 = 326,
        KP_7 = 327,
        KP_8 = 328,
        KP_9 = 329,
        KP_DECIMAL = 330,
        KP_DIVIDE = 331,
        KP_MULTIPLY = 332,
        KP_SUBTRACT = 333,
        KP_ADD = 334,
        KP_ENTER = 335,
        KP_EQUAL = 335,
        LEFT_SHIFT = 340,
        LEFT_CONTROL = 341,
        LEFT_ALT = 342,
        LEFT_SUPER = 343,
        RIGHT_SHIFT = 344,
        RIGHT_CONTROL = 345,
        RIGHT_ALT = 346,
        RIGHT_SUPER = 347,
        MENU = 348
    };

    enum class KeyMod{
        SHIFT = 0x0001,
        CONTROL = 0x0002,
        ALT = 0x0004,
        SUPER = 0x0008,
        CAPS_LOCK = 0x0010,
        NUM_LOCK = 0x0020
    };

    enum class MouseCode{
        LEFT = 0,
        RIGHT = 1,
        MIDDLE = 2,
        BUTTON_4 = 3,
        BUTTON_5 = 4,
        BUTTON_6 = 5,
        BUTTON_7 = 6,
        BUTTON_8 = 7
    };

    typedef void (*KeyFunc)(Window*, KeyCode, int, InputState, KeyMod);
    typedef void (*ScrollFunc)(Window*, double, double);

    enum class CursorState{
        NORMAL = 0x00034001,
        HIDDEN = 0x00034002,
        DISABLED = 0x00034003 
    };

    class Input{
        private:
            static bool firstFrame;
            static double lastMouseX;
            static double lastMouseY;
            static double mouseX;
            static double mouseY;
            static double mouseDeltaX;
            static double mouseDeltaY;

            static KeyFunc keyCallback;
            static ScrollFunc scrollCallback;

        public:
            /// @brief Get the current state of a specified key.
            /// @param window Active window
            /// @param key Key to query
            /// @return Current state
            static InputState GetInput(const std::shared_ptr<Window>& window, KeyCode key);

            /// @brief Get the current state of a specified mouse button.
            /// @param window Active window
            /// @param mouse Mouse button to query
            /// @return Current state
            static InputState GetInput(const std::shared_ptr<Window>& window, MouseCode mouse);

            /// @brief Get the current state of a mouse cursor within a window.
            /// @param window Active window
            /// @return Current state
            static CursorState GetCursorState(const std::shared_ptr<Window>& window);

            /// @brief Get the current state of a mouse cursor within a window.
            /// @param window Active window
            /// @return Current state
            static CursorState GetCursorState(Window* window);

            /// @brief Get the current mouse cursor position in screen space.
            /// @return Current mouse cursor postion
            static glm::vec2 GetMousePos();

            /// @brief Get the current mouse cursor delta.
            /// @return Current mouse cursor delta
            static glm::vec2 GetMouseDelta();


            /// @brief Set a function to be called when a key state changes.
            /// @param window Active window
            /// @param callback Callback function
            static void SetKeyCallback(const std::shared_ptr<Window>& window, KeyFunc callback);

            /// @brief Set a function to be called when the state of the mouse scroll wheel changes.
            /// @param window Active window
            /// @param callback Callback function
            static void SetScrollCallback(const std::shared_ptr<Window>& window, ScrollFunc callback);

            /// @brief Set the state of the mouse cursor within a window.
            /// @param window Active window
            /// @param state State to set
            static void SetCursorState(const std::shared_ptr<Window>& window, CursorState state);

            /// @brief Set the state of the mouse cursor within a window.
            /// @param window Active window
            /// @param state State to set
            static void SetCursorState(Window* window, CursorState state);


            /// @brief Update both the current cursor position and cursor delta (Ideally, should be called every frame).
            /// @param window Active window
            static void Update(const std::shared_ptr<Window>& window);
            
    };

}

#endif //INPUT_HPP