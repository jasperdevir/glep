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

#include <GLEP/audio/audio_buffer.hpp>

namespace GLEP::Audio{
    AudioBuffer::AudioBuffer(uint32_t bufferID){
        _ID = bufferID;
    }

    AudioBuffer::AudioBuffer(std::filesystem::path filePath){
        _filePath = filePath;
        _ID = LoadSoundFile(_filePath);
    }

    AudioBuffer::~AudioBuffer(){
        alDeleteBuffers(1, &_ID);
    }

    std::filesystem::path AudioBuffer::GetFilePath(){ return _filePath; }
    uint32_t AudioBuffer::GetID(){ return _ID; }

    //Heavily inspired by OpenAL Soft example alplay.c and Code, Tech, and Tutorials - OpenAL Tutorial pt.1 | Init and Play Sound Effects
    uint32_t AudioBuffer::LoadSoundFile(std::filesystem::path filePath){
        ALenum err, format;
        uint32_t buffer;
        SNDFILE* sndfile;
        SF_INFO sfinfo;
        short* membuf;
        sf_count_t num_frames;
        ALsizei num_bytes;

        sndfile = sf_open(filePath.string().c_str(), SFM_READ, &sfinfo);
        if(!sndfile){
            Print(PrintCode::ERROR, "AUDIO_BUFFER", "Failed to open file: " + filePath.string());
            return 0;
        }

        if(sfinfo.frames < 1){
            Print(PrintCode::ERROR, "AUDIO_BUFFER", "Bad sample count in file: " + filePath.string());
            sf_close(sndfile);
            return 0;
        }

        format = AL_NONE;
        if(sfinfo.channels == 1){
            format = AL_FORMAT_MONO16;
        } else if(sfinfo.channels == 2){
            format = AL_FORMAT_STEREO16;
        } else {
            Print(PrintCode::ERROR, "AUDIO_BUFFER", "Unsupported channel count of " + std::to_string(sfinfo.channels) + " in file: " + filePath.string());
            sf_close(sndfile);
            return 0;
        }

        membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

        num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
        if(num_frames < 1){
            Print(PrintCode::ERROR, "AUDIO_BUFFER", "Failed to read samples in file: " + filePath.string());
            free(membuf);
            sf_close(sndfile);
            return 0;
        }

        num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

        buffer = 0;
        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

        free(membuf);
        sf_close(sndfile);

        err = alGetError();
        if(err != AL_NO_ERROR){
            Print(PrintCode::ERROR, "AUDIO_BUFFER", "Error loading alBuffer from file: " + filePath.string());
            if(buffer && alIsBuffer(buffer)) alDeleteBuffers(1, &buffer);
            return 0;
        }

        return buffer;

    }
}