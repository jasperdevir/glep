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

#include <GLEP/core/utility/export.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

namespace GLEP{

    bool Export::SceneToFile(std::filesystem::path targetPath, const std::shared_ptr<Scene>& scene){
        if (!scene) {
            Print(PrintCode::ERROR, "EXPORT", "Scene attempted to export but is null.");
            return false;
        }

        std::ofstream outFile(targetPath); 
        
        if (!outFile) {
            Print(PrintCode::ERROR, "EXPORT", "Failed to open file for Scene export at " + targetPath.string());
            return false;
        }

        Print(PrintCode::INFO, "EXPORT", "Exporting Scene to " + targetPath.string());

        json j = scene->ToJson();

        outFile << std::setw(2) << j << std::endl;

        outFile.close();

        Print(PrintCode::INFO, "EXPORT", "Scene export complete to " + targetPath.string());

        return true;
    }

    void Export::ColorBufferToJPG(std::filesystem::path targetPath, const std::shared_ptr<Framebuffer>& framebuffer,  int quality){
        if(!targetPath.has_extension()) targetPath += ".jpg";

        framebuffer->Bind();

        int width = framebuffer->GetWidth();
        int height = framebuffer->GetHeight();
        GLubyte *pixels = new GLubyte[width * height * 4];
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        if(stbi_write_jpg(targetPath.string().c_str(), width, height, 4, pixels, quality) == 0){
            Print(PrintCode::ERROR, "EXPORT", "Failed to export Color Buffer to " + targetPath.string());
        } else {
            Print(PrintCode::INFO, "EXPORT", "Successfully exported Color Buffer to " + targetPath.string());
        }

        framebuffer->Unbind();
        delete[] pixels;
    }
}