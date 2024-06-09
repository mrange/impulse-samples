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

#define NOCRT

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#define WINDOWS_IGNORE_PACKING_MISMATCH
#define SHADER_MINIFIER_IMPL

#define LCG_A       1664525
#define LCG_C       1013904223
//#define LCG_M       4294967296 // 2^32

#define XRES        1600
#define YRES        900

#define INIT_MUSIC
#define USE_SOUND_THREAD

#define CELLS           12
#define BOMBS_PER_BOARD 16
#define STATE_SIZE      2
#define TOTAL_STATE     4*(CELLS*CELLS+STATE_SIZE)

#define BORDER_DIM      0.825F
#define CELL_DIM        (BORDER_DIM/(CELLS*0.5F))
#define STATE_SLEEP     0.125F
#define CLEAR_DEADLINE  60.F

#define SU_RESTART_POS  (SU_BUFFER_LENGTH/2)

#define GAME_SPEED      1.F

#ifdef _DEBUG
#include "assert.h"
#include <stdio.h>
#else
#define assert(x)
#endif

#include <math.h>

#include <windows.h>
#include <winuser.h>
#include <mmsystem.h>
#include <mmreg.h>

#include <GL/gl.h>
#include "glext.h"

#include "music.h"


enum class cell_state {
  uncovered     = 0
, covered_empty = 1
, covered_flag  = 2
, uncovering    = 3
, exploding     = 4
, exploded      = 5
, initial       = 6
};

struct cell {
  bool        has_bomb            ;
  int         near_bombs          ;

  float       changed_time        ;
  float       mouse_time          ;
  cell_state  prev_state          ;
  cell_state  state               ;
  cell_state  next_state          ;

  cell*       near_cells[9]       ;
};

enum class game_state {
  resetting_game  = 0
, playing         = 1
, resetting_board = 2
, game_over       = 3
};

struct board {
  int         uncovered           ;
  cell        cells[CELLS*CELLS]  ;
};

struct game {
  float       start_time          ;
  float       next_state_advance  ;
  float       boards_cleared      ;
  float       lock_time           ;
  float       locked_score        ;
  float       last_score          ;
  game_state  game_state          ;

  board       board               ;
};

extern "C" {
  #pragma bss_seg(".mainbss")
  int                 _fltused                      ;
  uint32_t            lcg_state                     ;
  int                 res_x                         ;
  int                 res_y                         ;
  int                 mouse_x                       ;
  int                 mouse_y                       ;
  int                 mouse_left_button_previous    ;
  int                 mouse_left_button             ;
  int                 mouse_right_button_previous   ;
  int                 mouse_right_button            ;
  float               application_start_time        ;
  struct game         game                          ;
  GLfloat             state[TOTAL_STATE]            ;
  HWAVEOUT            waveOut                       ;
  WAVEHDR             waveHeader                    ;
  SUsample            waveBuffer[SU_BUFFER_LENGTH]  ;

  LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  #pragma data_seg(".waveTime")
  MMTIME waveTime =
  {
    TIME_SAMPLES
  , 0
  };

  #pragma data_seg(".pixelFormatDescriptor")
  PIXELFORMATDESCRIPTOR pixelFormatSpecification {
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

  #pragma data_seg(".windowClassSpecification")
  WNDCLASSA windowClassSpecification {
        CS_OWNDC | CS_HREDRAW | CS_VREDRAW  // style
      , &WndProc                            // lpfnWndProc
      , 0                                   // cbClsExtra
      , 0                                   // cbWndExtra
      , 0                                   // hInstance
      , 0                                   // hIcon
      , 0                                   // hCursor
      , 0                                   // hbrBackground
      , 0                                   // lpszMenuName
      , "MS2024"                            // lpszClassName
  };

  #pragma data_seg(".waveFormatSpecification")
  WAVEFORMATEX waveFormatSpecification {
    WAVE_FORMAT_IEEE_FLOAT                                // wFormatTag
  , SU_CHANNEL_COUNT                                      // nChannels
  , SU_SAMPLE_RATE                                        // nSamplesPerSec
  , SU_SAMPLE_RATE * sizeof(SUsample) * SU_CHANNEL_COUNT  // nAvgBytesPerSec
  , sizeof(SUsample) * SU_CHANNEL_COUNT                   // nBlockAlign
  , sizeof(SUsample) * 8                                  // wBitsPerSample
  , 0                                                     // cbSize
  };

  #pragma data_seg(".glCreateShaderProgramv")
  char const nm_glCreateShaderProgramv[] = "glCreateShaderProgramv";

  #pragma data_seg(".glUseProgram")
  char const nm_glUseProgram[] = "glUseProgram";

  #pragma data_seg(".glUniform4fv")
  char const nm_glUniform4fv[] = "glUniform4fv";

  #pragma data_seg(".fragmentShaderProgram")
  GLint fragmentShaderProgram;

  #pragma data_seg(".fragmentShaders")
  char const * fragmentShaders[] = {
    #include "shader.inl"
  };

}

