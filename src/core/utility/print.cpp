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

#include <GLEP/core/utility/print.hpp>

namespace GLEP{

    std::string AddPrintColor(const std::string& str, const char* color){
        if(GLEP_PRINT_COLOR_ENABLED)
            return color + str + GLEP_PRINT_CODE_RESET;
        else
            return str;
    }

    std::ostream& operator<<(std::ostream& os, PrintCode code) {
        os << "[" << AddPrintColor("GLEP", GLEP_PRINT_TITLE_COLOR_CODE) << "]::";
        switch (code) {
            case PrintCode::INFO:
                os << "[" << AddPrintColor("INFO", GLEP_PRINT_INFO_COLOR_CODE) << "]";
                break;
            case PrintCode::ERROR:
                os << "[" << AddPrintColor("ERROR", GLEP_PRINT_ERROR_COLOR_CODE) << "]";
                break;
            case PrintCode::CRITICAL:
                os << "[" << AddPrintColor("CRITICAL", GLEP_PRINT_CRITICAL_COLOR_CODE) << "]";
                break;
            default:
                os << "[UNKNOWN]";
                break;
        }
        return os;
    }

    bool CheckPrintEnabled(PrintCode code){
        return (int)code <= GLEP_PRINT_ENABLED;
    }

    void Print(PrintCode code, const std::string& msg){
        if(!CheckPrintEnabled(code)) 
            return;

        std::stringstream ss;
        ss << code << "-> " << msg << std::endl;

        if(code == PrintCode::INFO){
            std::cout << ss.str();
        } else {
            std::cerr << ss.str();
        }
    }

    void Print(PrintCode code, const std::string& location, const std::string& msg){
        if(!CheckPrintEnabled(code)) 
            return;
        
        std::stringstream ss;
        ss << code << "::[" << AddPrintColor(location, GLEP_PRINT_LOCATION_COLOR_CODE) << "]-> " << msg << std::endl; 

        if(code == PrintCode::INFO){
            std::cout << ss.str();
        } else {
            std::cerr << ss.str();
        }
    }

}