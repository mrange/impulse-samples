/*
            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                    Version 2, December 2004

 Copyright (C) 2024 Mårten Rånge

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. You just DO WHAT THE FUCK YOU WANT TO.
*/

#pragma once

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define WINDOWS_IGNORE_PACKING_MISMATCH
#define SHADER_MINIFIER_IMPL

#include "assert.h"

#ifdef _DEBUG
#include <stdio.h>
#endif

#include <windows.h>
#include <winuser.h>
#include <mmsystem.h>
#include <mmreg.h>

#include <GL/gl.h>
#include "glext.h"

#define XRES 1600
#define YRES 900

#define SU_LENGTH_IN_SAMPLES  (XRES*YRES)
#define SU_SAMPLE_RATE        44100
#define SU_CHANNEL_COUNT      1
#define SU_BUFFER_LENGTH      (SU_LENGTH_IN_SAMPLES*SU_CHANNEL_COUNT)

using SUsample = char;

extern "C" {
  #pragma bss_seg(".mainbss")
  int       _fltused;
  SUsample  waveBuffer[SU_BUFFER_LENGTH];

  #pragma data_seg(".pixelFormatDescriptor")
  static PIXELFORMATDESCRIPTOR pixelFormatSpecification {
      sizeof(PIXELFORMATDESCRIPTOR)                           // nSize
    , 1                                                       // nVersion
    , PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER  // dwFlags
    , PFD_TYPE_RGBA                                           // iPixelType
    , 32                                                      // cColorBits
    , 0                                                       // cRedBits
    , 0                                                       // cRedShift
    , 0                                                       // cGreenBits
    , 0                                                       // cGreenShift
    , 0                                                       // cBlueBits
    , 0                                                       // cBlueShift
    , 8                                                       // cAlphaBits
    , 0                                                       // cAlphaShift
    , 0                                                       // cAccumBits
    , 0                                                       // cAccumRedBits
    , 0                                                       // cAccumGreenBits
    , 0                                                       // cAccumBlueBits
    , 0                                                       // cAccumAlphaBits
    , 32                                                      // cDepthBits
    , 0                                                       // cStencilBits
    , 0                                                       // cAuxBuffers
    , PFD_MAIN_PLANE                                          // iLayerType
    , 0                                                       // bReserved
    , 0                                                       // dwLayerMask
    , 0                                                       // dwVisibleMask
    , 0                                                       // dwDamageMask
  };

  #pragma data_seg(".waveFormatSpecification")
  WAVEFORMATEX waveFormatSpecification =
  {
    WAVE_FORMAT_PCM                                       // wFormatTag
  , SU_CHANNEL_COUNT                                      // nChannels
  , SU_SAMPLE_RATE                                        // nSamplesPerSec
  , SU_SAMPLE_RATE * sizeof(SUsample) * SU_CHANNEL_COUNT  // nAvgBytesPerSec
  , sizeof(SUsample) * SU_CHANNEL_COUNT                   // nBlockAlign
  , sizeof(SUsample) * 8                                  // wBitsPerSample
  , 0                                                     // cbSize
  };

  #pragma data_seg(".waveHeader")
  WAVEHDR waveHeader =
  {
    (LPSTR)waveBuffer                   // lpData
  , SU_BUFFER_LENGTH * sizeof(SUsample) // dwBufferLength
  , 0                                   // dwBytesRecorded
  , 0                                   // dwUser
  , 0                                   // dwFlags
  , 0                                   // dwLoops
  , 0                                   // lpNext
  , 0                                   // reserved
  };

  #pragma data_seg(".waveTime")
  MMTIME waveTime =
  {
    TIME_SAMPLES
  , 0
  };

  #pragma data_seg(".glCreateShaderProgramv")
  static const char nm_glCreateShaderProgramv[] = "glCreateShaderProgramv";

  #pragma data_seg(".glUseProgram")
  static const char nm_glUseProgram[] = "glUseProgram";

  #pragma data_seg(".glUniform4f")
  static const char nm_glUniform4f[] = "glUniform4f";

  #pragma data_seg(".fragmentShaderProgram")
  static GLint fragmentShaderProgram;

  #pragma data_seg(".fragmentShaders")
  static char const * fragmentShaders[] = {
    #include "shader.inl"
  };

}

