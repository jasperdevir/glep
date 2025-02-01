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

#ifndef PRINT_HPP
#define PRINT_HPP

#include <iostream>
#include <string>
#include <sstream>

/*
    GLEP_PRINT_ENABLED

    3 - All GLEP Print Messages Enabled
    2 - Only GLEP CRITICAL and ERROR Print Messages Enabled
    1 - Only GLEP CRITICAL Messages Enabled
    0 - No GLEP Messages Enabled
*/
#define GLEP_PRINT_ENABLED 3

#define GLEP_PRINT_COLOR_ENABLED 1

#define GLEP_PRINT_CODE_RESET "\033[0m"
#define GLEP_PRINT_CODE_RED "\033[31m"
#define GLEP_PRINT_CODE_GREEN "\033[32m"
#define GLEP_PRINT_CODE_YELLOW "\033[33m"
#define GLEP_PRINT_CODE_BLUE "\033[34m"
#define GLEP_PRINT_CODE_MAGENTA "\033[35m"
#define GLEP_PRINT_CODE_CYAN "\033[36m"
#define GLEP_PRINT_CODE_WHITE "\033[37m"

#define GLEP_PRINT_TITLE_COLOR_CODE  GLEP_PRINT_CODE_GREEN
#define GLEP_PRINT_LOCATION_COLOR_CODE GLEP_PRINT_CODE_MAGENTA
#define GLEP_PRINT_CRITICAL_COLOR_CODE GLEP_PRINT_CODE_RED
#define GLEP_PRINT_ERROR_COLOR_CODE GLEP_PRINT_CODE_YELLOW
#define GLEP_PRINT_INFO_COLOR_CODE GLEP_PRINT_CODE_CYAN

namespace GLEP{

    enum class PrintCode{
        CRITICAL = 1,
        ERROR = 2,
        INFO = 3
    };

    /// @brief Add a ANSI color code to a String.
    /// @param str String to add print color to
    /// @param color ANSI color code to add
    /// @return The String with added the color code
    std::string AddPrintColor(const std::string& str, const char* color);

    /// @brief Check if PrintCode is currently enabled.
    /// @param code PrintCode to check
    /// @return If PrintCode is currently enabled
    bool CheckPrintEnabled(PrintCode code);

    /// @brief Print a GLEP formated message to the console.
    /// @param code PrintCode to tag the message with
    /// @param msg Message to print
    void Print(PrintCode code, const std::string& msg);

    /// @brief Print a GLEP formated message to the console.
    /// @param code PrintCode to tag the message with
    /// @param location Context location to tag the message with
    /// @param msg Message to print
    void Print(PrintCode code, const std::string& location, const std::string& msg);

}

#endif //PRINT_HPP