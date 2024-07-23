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

#include <stdint.h>

#ifdef _DEBUG
#include <stdio.h>
#endif

#include <windows.h>
#include <winuser.h>
#include <mmsystem.h>
#include <mmreg.h>

#include <GL/gl.h>
#include "glext.h"
#include "wglext.h"

#define XRES 1600
#define YRES 900

extern "C" {
  #pragma bss_seg(".mainbss")
  int       _fltused  ;
  uint32_t  lcg_state ;
  GLuint    tex       ;

  static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  #pragma data_seg(".xres")
  static int xres = XRES;
  #pragma data_seg(".yres")
  static int yres = YRES;

  #pragma data_seg(".windowRect")
  static RECT windowRect {
    0
  , 0
  , XRES
  , YRES
  };

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
    , 24                                                      // cDepthBits
    , 8                                                       // cStencilBits
    , 0                                                       // cAuxBuffers
    , PFD_MAIN_PLANE                                          // iLayerType
    , 0                                                       // bReserved
    , 0                                                       // dwLayerMask
    , 0                                                       // dwVisibleMask
    , 0                                                       // dwDamageMask
  };

  #pragma data_seg(".windowClassSpecification")
  static WNDCLASSA windowClassSpecification {
        CS_OWNDC | CS_HREDRAW | CS_VREDRAW  // style
      , &WndProc                            // lpfnWndProc
      , 0                                   // cbClsExtra
      , 0                                   // cbWndExtra
      , 0                                   // hInstance
      , 0                                   // hIcon
      , 0                                   // hCursor
      , 0                                   // hbrBackground
      , 0                                   // lpszMenuName
      , "W4K"                               // lpszClassName
  };

  #pragma data_seg(".glActiveTexture")
  static const char nm_glActiveTexture[] = "glActiveTexture";

  #pragma data_seg(".glCreateShaderProgramv")
  static const char nm_glCreateShaderProgramv[] = "glCreateShaderProgramv";

  #pragma data_seg(".glUseProgram")
  static const char nm_glUseProgram[] = "glUseProgram";

  #pragma data_seg(".glUniform1i")
  static const char nm_glUniform1i[] = "glUniform1i";

  #pragma data_seg(".glUniform4f")
  static const char nm_glUniform4f[] = "glUniform4f";

  #pragma data_seg(".fragmentShaderProgram")
  static GLint fragmentShaderProgram;

  #pragma data_seg(".fragmentShaders")
  static char const * fragmentShaders[] = {
    #include "shader.inl"
  };

}

