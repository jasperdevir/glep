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

#ifndef AUDIO_BUFFER_HPP
#define AUDIO_BUFFER_HPP

#include <GLEP/core/utility/print.hpp>

#include <filesystem>
#include <vector>

#include <AL/al.h>
#include <AL/alext.h>
#include <inttypes.h>
#include <sndfile/sndfile.h>

namespace GLEP::Audio{

    class AudioBuffer{
        private:
            uint32_t _ID;
            std::filesystem::path _filePath;

        public:
            AudioBuffer(uint32_t bufferID);
            AudioBuffer(std::filesystem::path filePath);
            ~AudioBuffer();

            /// @brief Get the audio file path.
            /// @return Audio file path
            std::filesystem::path GetFilePath();

            /// @brief Get the buffer ID.
            /// @return Buffer ID
            uint32_t GetID();


            /// @brief Load a sound file.
            /// @param filePath Sound file path
            /// @return Generated buffer ID
            static uint32_t LoadSoundFile(std::filesystem::path filePath);

    };

}

#endif //AUDIO_BUFFER_HPP