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

#include "fourk-win-demo.h"

#include <math.h>

template<typename T>
__forceinline INT32 i32(T && v)
{
  return static_cast<INT32>(v);
}

__forceinline float fractf(float v)
{
  return v-floorf(v);
}

template<typename T>
__forceinline INT16 i16(T && v)
{
  return static_cast<INT32>(v);
}

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

  #pragma code_seg(".init_demo")
  void init_demo() {
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

  #pragma code_seg(".draw_demo")
  void draw_demo(float time) {
    // Use the previously compiled shader program
    ((PFNGLUSEPROGRAMPROC)wglGetProcAddress(nm_glUseProgram))(fragmentShaderProgram);
    // Sets shader parameters
    ((PFNGLUNIFORM4FPROC)wglGetProcAddress(nm_glUniform4f))(
        0 // Uniform location
      , time
      , static_cast<GLfloat>(xres)
      , static_cast<GLfloat>(yres)
      , 0
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

  using sample_type = UINT8;

  #pragma code_seg(".render_song")
  void render_song(SampleType * waveBuffer) {
    auto pi   = 3.141592654F;
    auto tau  = 2*pi;

    for (auto i = 0; i < LENGTH_IN_SAMPLES; ++i) {
      auto t = ((float)i)/SAMPLE_RATE;

      float musicTime = 32768.F*t;
      // 120 BPM
      float kickTime  = 2.F*t;
      float kickTime4 = 4.F*kickTime;

      // Can be cast to i16 it seems
      INT32 time0     = i32(musicTime);
      INT32 tune0     = i32(time0&(time0>>12));
      // TODO: Problematic
      float tune1     = fmodf(musicTime,tune0);
      float tune2     = 4.F*(fractf(kickTime4))-3.F;
      // Precalculate powf
      float tune3     = powf(2.F,-tune2);
      // TODO: Problematic
      auto tunew      = i32(tune1*tune3)&127;
      // Precalculate powf
      auto kickw      = i32(powf(8192.F,1.F-(fractf(kickTime))*16384.F/1e4))&64;
      auto wave       = (tunew+kickw)&255;


      auto sample = wave/256.F-1.F;

      for(auto j = 0; j < CHANNEL_COUNT; ++j) {
        *waveBuffer = sample;
        ++waveBuffer;
      }
    }
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

  // Init our demo
  init_demo();

  // Now init the music.
  //  The way sointu works is that we call su_render_song which writes samples
  //  to a wave buffer
  //  Then we just ask Windows to play it for us

//#define USE_SOUND_THREAD
#ifdef USE_SOUND_THREAD
  // Create the wave buffer in a separate thread so we don't have to wait for it
  auto hthread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)render_song, waveBuffer, 0, 0);
  assert(hthread);
#else
  // We don't mind waiting for the sound.
  render_song(waveBuffer);
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

    // Windows message handling done, let's draw some gfx

    // Get current wave position
    auto waveGetPosOk = waveOutGetPosition(hwo, &waveTime, sizeof(MMTIME));
    assert(waveGetPosOk == MMSYSERR_NOERROR);

    // Have we passed the end sample? If so then we are done
    auto currentSample = waveTime.u.sample;
    if (currentSample >= LENGTH_IN_SAMPLES) {
      done = 1;
    }

    // Compute the demoTime from the current sample position
    auto demoTime = currentSample/((float)SAMPLE_RATE);

    // Draw the demo
    draw_demo(demoTime);

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

