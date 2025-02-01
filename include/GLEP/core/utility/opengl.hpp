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

#ifndef OPENGL_HPP
#define OPENGL_HPP

namespace GLEP{
    /// @brief OpenGL major version.
    extern unsigned int GL_MAJ_VERSION;

    /// @brief OpenGL minor version.
    extern unsigned int GL_MIN_VERSION;

    /// @brief Anti-aliasing subsamples per screen coordinate.
    extern unsigned int GL_MULTISAMPLES;
}

#endif //OPENGL_HPP