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

#include "minesweeper.h"

#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))

#include <math.h>

extern "C" {

#ifdef _DEBUG
  void APIENTRY debugCallback(
      GLenum          source
    , GLenum          type
    , GLuint          id
    , GLenum          severity
    , GLsizei         length
    , GLchar const *  message
    , void const *    userParam
    )
  {
    printf(message);
    printf("\n");
  }
  char debugLog[0xFFFF];
#endif

  #pragma code_seg(".init_game")
  void init_game() {
//    lcg_seed = GetTickCount();
    lcg_state = 19740531;

    // Bit of debugging info during debug builds
    //  Don't want to waste bytes on that in Release mode
#ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    ((PFNGLDEBUGMESSAGECALLBACKPROC)wglGetProcAddress("glDebugMessageCallback"))(debugCallback, 0);
#endif

  // Compiles the provided fragment shader into a shader program
  fragmentShaderProgram = ((PFNGLCREATESHADERPROGRAMVPROC)wglGetProcAddress(nm_glCreateShaderProgramv))(GL_FRAGMENT_SHADER, 1, fragmentShaders);

#ifdef _DEBUG
  ((PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog"))(fragmentShaderProgram, sizeof(debugLog), NULL, debugLog);
  printf(debugLog);
  glDisable(GL_DEBUG_OUTPUT);
#endif
  }

  #pragma code_seg(".lcg_rand_uint32")
  uint32_t lcg_rand_uint32() {
    lcg_state = (1664525U * lcg_state + 1013904223U);
    return lcg_state;
  }

  #pragma code_seg(".lcg_rand_float")
  float lcg_rand_float() {
    const double inv = 0.25/(1<<30);
    double v = inv*lcg_rand_uint32();
    assert(v >= 0.);
    assert(v <= 1.);
    return static_cast<float>(v);
  }

  #pragma code_seg(".reset_game")
  void reset_game(float time) {
    memset(&game, 0, sizeof(game));
    game.game_state = game_state::playing;
    game.start_time = time;

    auto total_bombs  = 0;
    auto offy         = 0;

    for (auto y = 0; y < CELLS; ++y) {
      for (auto x = 0; x < CELLS; ++x) {
        auto off          = x + offy;
        auto & cell       = game.cells[off];
        auto rand         = lcg_rand_float();
        cell.x            = x;
        cell.y            = y;
        cell.state        = cell_state::covered_empty;
        cell.next_state   = cell_state::covered_empty;
        if (rand < 0.1F) {
          cell.has_bomb   = true;
          ++total_bombs;
        }
        cell.changed_time = time;
      }
      offy += CELLS;
    }
    game.total_bombs = total_bombs;

    for (auto & cell : game.cells) {
      auto near_bombs   = 0;
      auto near_i       = 0;
      for (auto yy = -1; yy <= 1; ++yy) {
        auto near_y     = cell.y+yy;
        for (auto xx = -1; xx <= 1; ++xx) {
          auto near_x   = cell.x+xx;
          auto near_off = near_y*CELLS+near_x;
          auto hit_mid = xx == 0 && yy == 0;
          if (!hit_mid && near_y >= 0 && near_y < CELLS && near_x >= 0 && near_x < CELLS) {
            auto & near_cell        = game.cells[near_off];
            assert(near_off >= 0);
            assert(near_off < CELLS*CELLS);
            cell.near_cells[near_i] = &near_cell;
            ++near_i;
            if (near_cell.has_bomb) ++near_bombs;
          }
          assert(near_i <= 8);
        }
      }
      cell.near_bombs = near_bombs;
    }
  }

  #pragma code_seg(".draw_game")
  void draw_game(float time) {
    const int size = sizeof(state)/sizeof(GLfloat);
    auto g_t        = time-game.start_time;
    auto r_x        = static_cast<GLfloat>(res_x);
    auto r_y        = static_cast<GLfloat>(res_y);
    auto m_x        = static_cast<GLfloat>(mouse_x);
    auto m_y        = static_cast<GLfloat>(mouse_y);

    // Setup state
    GLfloat* s  = state;
    s[0]        = g_t;
    s[1]        = r_x;
    s[2]        = r_y;
    s[4]        = m_x;
    s[5]        = m_y;

    auto mp_x   = (-res_x+2.F*m_x)/res_y;
    auto mp_y   = -(-res_y+2.F*m_y)/res_y;

    auto mcp_x  = mp_x;
    auto mcp_y  = mp_y;

    mcp_x       /= CELL_DIM;
    mcp_y       /= CELL_DIM;

    mcp_x       -= 0.5F;
    mcp_y       -= 0.5F;

    auto mnp_x  = floor(mcp_x+0.5F);
    auto mnp_y  = floor(mcp_y+0.5F);

    static_assert(CELLS%2 == 0, "Expected cells to be even");

    mnp_x       += CELLS*0.5F;
    mnp_y       += CELLS*0.5F;

    auto ci      = static_cast<int>(mnp_x+mnp_y*CELLS);

    if (mnp_x >= 0 && mnp_x < CELLS && mnp_y >= 0 && mnp_y < CELLS) {
      assert(ci >= 0 && ci < CELLS*CELLS);
      auto & cell = game.cells[ci];
      cell.mouse_time = g_t;

      if (cell.state == cell.next_state && game.game_state == game_state::playing) {
        // React on mouse click if state is up to date and we are playing

        if (mouse_left_button == 0 && mouse_left_button_previous == 1) {
          // Left button released
          switch (cell.state) {
            case cell_state::covered_empty:
            case cell_state::covered_flag:
              cell.state = cell.next_state = cell_state::uncovering;
              cell.changed_time = g_t;
              break;
          }
        }

        if (mouse_right_button == 0 && mouse_right_button_previous == 1) {
          // Right button released
          switch (cell.state) {
            case cell_state::covered_empty:
            case cell_state::covered_flag:
              // Toggle flag tile
              cell.state = cell.next_state = cell.state == cell_state::covered_empty 
                ? cell_state::covered_flag
                : cell_state::covered_empty
                ;
              cell.changed_time = g_t;
              break;
          }
        }
      }
    }

    if (g_t >= next_state_advance) {
      next_state_advance = g_t + STATE_SLEEP;

      for (auto & cell : game.cells) {
        switch (cell.state) {
          case cell_state::uncovering:
            if (cell.has_bomb) {
              cell.next_state = cell_state::exploding;
              game.game_state = game_state::game_over;
            } else {
              cell.next_state = cell_state::uncovered;
              if (cell.near_bombs == 0) {
                for(auto near_cell : cell.near_cells) {
                  if (near_cell) {
                    switch(near_cell->state) {
                      case cell_state::covered_empty:
                      case cell_state::covered_flag:
                        near_cell->next_state = cell_state::uncovering;
                        break;
                    }
                  }
                }
              }
            }
            break;
          case cell_state::exploding:
            cell.next_state = cell_state::exploded;
            for(auto near_cell : cell.near_cells) {
              if (near_cell) {
                switch(near_cell->state) {
                  case cell_state::exploding:
                  case cell_state::exploded:
                    break;
                  default:
                    near_cell->next_state = cell_state::exploding;
                    break;
                }
              }
            }
            break;
        }
      }

      for (auto & cell : game.cells) {
        if (cell.state != cell.next_state) {
          cell.state        = cell.next_state;
          cell.changed_time = g_t;
        }
      }
    }

    mouse_left_button_previous  = mouse_left_button;
    mouse_right_button_previous = mouse_right_button;

    //  Jump to first cell
    s           = state+4*STATE_SIZE;
    // Setup cells
    for (auto & cell : game.cells) {
      s[0] = cell.state != cell_state::uncovered ? static_cast<GLfloat>(cell.state) : static_cast<GLfloat>(-cell.near_bombs);
      s[1] = cell.changed_time;
      s[2] = cell.mouse_time;
      s += 4;
    }

    // Use the previously compiled shader program
    ((PFNGLUSEPROGRAMPROC)wglGetProcAddress(nm_glUseProgram))(fragmentShaderProgram);
    // Sets shader parameters
    ((PFNGLUNIFORM4FVPROC)wglGetProcAddress(nm_glUniform4fv))(
        0 // Uniform location
      , size
      , state
      );
    // Draws a rect over the entire window with fragment shader providing the gfx
    glRects(-1, -1, 1, 1);
  }

  #pragma code_seg(".WndProc")
  LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
      // To be ignored
      case WM_SYSCOMMAND:
        if (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER) 
          return 0;
        break;
      // Mouse moved
      case WM_MOUSEMOVE:
        mouse_x = GET_X_LPARAM(lParam);
        mouse_y = GET_Y_LPARAM(lParam);
        break;
      // Resized the window? No problem!
      case WM_SIZE:
        res_x = LOWORD(lParam);
        res_y = HIWORD(lParam);
        glViewport(0, 0, res_x, res_y);
        break;
      // Capture mouse buttons
      case WM_LBUTTONDOWN:
      case WM_LBUTTONUP:
        mouse_left_button = uMsg == WM_LBUTTONDOWN;
        break;
      case WM_RBUTTONDOWN:
      case WM_RBUTTONUP:
        mouse_right_button = uMsg == WM_RBUTTONDOWN;
        break;
      // It's time to stop!
      case WM_CLOSE:
      case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
      case WM_CHAR:
      case WM_KEYDOWN:
        // Done on Escape
        if (wParam == VK_ESCAPE) {
          PostQuitMessage(0);
          return 0;
        }
        break;
    }

    // Apply default window message handling
    return(DefWindowProcA(hWnd, uMsg, wParam, lParam));
  }
}

#pragma code_seg(".main")
#ifdef USE_CRINKLER
void entrypoint() {
#else
int __cdecl main() {
#endif
/*
  auto dpiAware = SetProcessDPIAware();
  assert(dpiAware);
*/
  auto hinstance    = GetModuleHandle(0);
  assert(hinstance);

  // Sets up a Rebecca purple brush that will be visible if the shader fails for some reason
  auto hbackground  = CreateSolidBrush(RGB(0x66, 0x33, 0x99));
  assert(hbackground);

  // Use default arrow cursor
  auto hcursor      = LoadCursor(nullptr, IDC_ARROW);

  // Setups the windows class
  windowClassSpecification.hInstance      = hinstance   ;
  windowClassSpecification.hbrBackground  = hbackground ;
  windowClassSpecification.hCursor        = hcursor     ;

  // Registers the windows class
  auto regOk = RegisterClassA(&windowClassSpecification);
  assert(regOk);

  auto dwStyle = WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_POPUP;

  // Adjust the window rect so that the client rect gets the desired size
  auto rectOk = AdjustWindowRect(&windowRect, dwStyle, 0);
  assert(rectOk);

  auto width  = windowRect.right  - windowRect.left;
  auto height = windowRect.bottom - windowRect.top;

  // Create the window using the class we registered
  auto hwnd = CreateWindowExA(
    0                                             // dwExStyle
  , windowClassSpecification.lpszClassName        // lpClassName
  , nullptr                                       // lpWindowName
  , dwStyle                                       // dwStyle
  // Advanced math to compute top left corner of window
  , (GetSystemMetrics(SM_CXSCREEN) - width) >> 1  // nX
  , (GetSystemMetrics(SM_CYSCREEN) - height) >> 1 // nY
  , width                                         // nWidth
  , height                                        // nHeight
  , nullptr                                       // hWndParent
  , nullptr                                       // hMenu
  , nullptr                                       // hInstance
  , nullptr                                       // lpParam
  );
  assert(hwnd);

  // We need the Device Context to do Windows graphics
  auto hdc = GetDC(hwnd);
  assert(hdc);

  // Find a pixel format that is compatible with OpenGL
  auto pixelFormat = ChoosePixelFormat(
    hdc
  , &pixelFormatSpecification
  );
  assert(pixelFormat);

  // Set the pixel format on the Device Context to prepare it for OpenGL
  auto setOk = SetPixelFormat(
    hdc
  , pixelFormat
  , nullptr
  );
  assert(setOk);

  // Create OpenGL Context
  auto hglrc = wglCreateContext(hdc);
  assert(hglrc);

  // Sets the OpenGL context as the current one
  auto makeOk = wglMakeCurrent(hdc, hglrc);
  assert(makeOk);

  // Init our game
  init_game();

#ifdef INIT_MUSIC
  // Now init the music.
  //  The way sointu works is that we call su_render_song which writes samples
  //  to a wave buffer
  //  Then we just ask Windows to play it for us

  // Version v0.3.0 of sointu has an issue in that the EBX register is not restored
  //  So save it with some inline assembler
  //  Fix coming: https://github.com/vsariola/sointu/issues/130
  _asm {
    push ebx
  }
  // Load gmdls sound
  su_load_gmdls();
  // And restore the ebx register
  _asm {
    pop ebx
  }

#ifdef USE_SOUND_THREAD
  // Create the wave buffer in a separate thread so we don't have to wait for it
  auto hthread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)su_render_song, waveBuffer, 0, 0);
  assert(hthread);
#else
  // We don't mind waiting for the sound.
  su_render_song(waveBuffer);
#endif
#endif

  // Play the sound buffer

  HWAVEOUT hwo;
  auto waveOpenOk = waveOutOpen(
    &hwo
  , WAVE_MAPPER
  , &waveFormatSpecification
  , NULL
  , 0
  , CALLBACK_NULL
  );
  assert(waveOpenOk == MMSYSERR_NOERROR);

  auto wavePrepareOk = waveOutPrepareHeader(
    hwo
  , &waveHeader
  , sizeof(waveHeader)
  );
  assert(wavePrepareOk == MMSYSERR_NOERROR);

  auto waveWriteOk = waveOutWrite(
    hwo
  , &waveHeader
  , sizeof(waveHeader)
  );
  assert(waveWriteOk == MMSYSERR_NOERROR);

  auto done = false;

  // Loop until done
  while(!done) {
    MSG msg;
    // The classic window message pump
    while (PeekMessageA(&msg, 0, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) done = 1;
      // Result intentionally ignored
      TranslateMessage(&msg);
      // Result intentionally ignored
      DispatchMessageA(&msg);
    }

    auto time = GetTickCount() / 1000.F;
    if (game.game_state == game_state::reset) {
      // Resets the game state
      reset_game(time);
    }

    // Windows message handling done, let's draw some gfx

    // Get current wave position
    auto waveGetPosOk = waveOutGetPosition(hwo, &waveTime, sizeof(MMTIME));
    assert(waveGetPosOk == MMSYSERR_NOERROR);

    // Have we passed the end sample? If so then restart music
    auto currentSample = waveTime.u.sample;
    if (currentSample >= SU_LENGTH_IN_SAMPLES) {
      // TODO: Restart music
    }

    // Draw the game
    draw_game(time);

    // Swap the buffers to present the gfx
    auto swapOk = SwapBuffers(hdc);
    assert(swapOk);

  }

  // We are done, just exit. No need to waste bytes on cleaning
  //  up resources. Windows will do it for us.

#ifdef USE_CRINKLER
  ExitProcess(0);
#else
  return 0;
#endif
}

