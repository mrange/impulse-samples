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
    lcg_seed = GetTickCount();

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
    lcg_seed = (1664525U * lcg_seed + 1013904223U);
    return lcg_seed;
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

    for(auto y = 0; y < CELLS; ++y) {
      for(auto x = 0; x < CELLS; ++x) {
        auto off          = x + offy;
        auto & cell       = game.cells[off];
        auto rand         = lcg_rand_float();
        cell.x            = x;
        cell.y            = y;
        if (rand < 0.1F) {
          cell.has_bomb   = true;
          ++total_bombs;
        }
        cell.changed_time = time;
      }
      offy += CELLS;
    }
    game.total_bombs = total_bombs;

    for(auto i = 0; i < CELLS*CELLS; ++i) {
      auto & cell = game.cells[i];
      auto near_bombs   = 0;
      auto near_i       = 0;
      for (auto yy = -1; yy <= 1; ++yy) {
        auto near_y     = cell.y+yy;
        for (auto xx = -1; xx <= 1; ++xx) {
          auto near_x   = cell.x+xx;
          auto near_off = near_y*CELLS+near_x;
          if (xx != yy && near_y >= 0 && near_y < CELLS && near_x >= 0 && near_x < CELLS) {
            auto & near_cell        = game.cells[near_off];
            assert(near_off >= 0);
            assert(near_off < CELLS*CELLS);
            cell.near_cells[near_i] = &near_cell;
            ++near_i;
            if (near_cell.has_bomb) ++near_bombs;
          }
          assert(near_i < 8);
        }
      }
      cell.near_bombs = near_bombs;
    }
  }

  #pragma code_seg(".draw_game")
  void draw_game(float time) {
    const int size = sizeof(state)/sizeof(GLfloat);
    // Setup state
    GLfloat* s  = state;
    s[0]        = time-game.start_time;
    s[1]        = static_cast<GLfloat>(xres);
    s[2]        = static_cast<GLfloat>(yres);
    //  Jump to first cell
    s           = state+4*STATE_SIZE;
    // Setup cells
    for(auto i = 0; i < CELLS*CELLS; ++i) {
      auto & cell = game.cells[i];

      s[0] = static_cast<GLfloat>(-cell.near_bombs);
      s[1] = cell.changed_time;
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
  LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    // Let's ignore these
    if (uMsg == WM_SYSCOMMAND && (wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER))
      return 0;

    // It's time to stop!
    if (uMsg == WM_CLOSE || uMsg == WM_DESTROY || (uMsg == WM_KEYDOWN && wParam == VK_ESCAPE)) {
      PostQuitMessage(0);
      return 0;
    }

    // Resized the window? No problem!
    if (uMsg == WM_SIZE) {
      xres = LOWORD(lParam);
      yres = HIWORD(lParam);
      glViewport(0, 0, xres, yres);
    }

    // Another way to stop!
    if (uMsg == WM_CHAR || uMsg == WM_KEYDOWN) {
      if (wParam == VK_ESCAPE) {
        PostQuitMessage(0);
        return 0;
      }
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
  auto hinstance = GetModuleHandle(0);
  assert(hinstance);

  // Sets up a Rebecca purple brush that will be visible if the shader fails for some reason
  auto hbackground = CreateSolidBrush(RGB(0x66, 0x33, 0x99));
  assert(hbackground);

  // Setups the windows class
  windowClassSpecification.hInstance      = hinstance;
  windowClassSpecification.hbrBackground  = hbackground;

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

#define USE_SOUND_THREAD
#ifdef USE_SOUND_THREAD
  // Create the wave buffer in a separate thread so we don't have to wait for it
  auto hthread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)su_render_song, waveBuffer, 0, 0);
  assert(hthread);
#else
  // We don't mind waiting for the sound.
  su_render_song(waveBuffer);
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

