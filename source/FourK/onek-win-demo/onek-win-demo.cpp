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

#include "onek-win-demo.h"

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
  __forceinline void init_demo() {
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
  __forceinline void draw_demo(float time) {
    // Use the previously compiled shader program
    ((PFNGLUSEPROGRAMPROC)wglGetProcAddress(nm_glUseProgram))(fragmentShaderProgram);
    // Sets shader parameters
    ((PFNGLUNIFORM4FPROC)wglGetProcAddress(nm_glUniform4f))(
        0 // Uniform location
      , time
      , static_cast<GLfloat>(XRES)
      , static_cast<GLfloat>(YRES)
      , 0
      );
    // Draws a rect over the entire window with fragment shader providing the gfx
    glRects(-1, -1, 1, 1);
  }

}

#pragma code_seg(".main")
#ifdef USE_CRINKLER
void entrypoint() {
#else
int __cdecl main() {
#endif

  auto dwStyle = WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_POPUP;

  // Create the window using the STATIC class
  auto hwnd = CreateWindowExA(
    0                                             // dwExStyle
  // Special name for STATIC window class
  , reinterpret_cast<LPCSTR>(0xC019)              // lpClassName
  , nullptr                                       // lpWindowName
  , dwStyle                                       // dwStyle
  , 0                                             // nX
  , 0                                             // nY
  , XRES                                          // nWidth
  , YRES                                          // nHeight
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

    // If ESCAPE is pressed we are done
    if (GetAsyncKeyState(VK_ESCAPE)) {
      done = 1;
    }

    // Windows message handling done, let's draw some gfx

    // Get current wave position
    auto waveGetPosOk = waveOutGetPosition(hwo, &waveTime, sizeof(MMTIME));
    assert(waveGetPosOk == MMSYSERR_NOERROR);

    // Have we passed the end sample? If so then we are done
    auto currentSample = waveTime.u.sample;
    if (currentSample >= SU_LENGTH_IN_SAMPLES) {
      done = 1;
    }

    // Compute the demoTime from the current sample position
    auto demoTime = currentSample/((float)SU_SAMPLE_RATE);

    // Draw the demo
    draw_demo(demoTime);

    if (currentSample == 0) {
      // The shader computes the music as a it's first frame
      // Thus we copy it right into sound buffer
      //  At 1600x900 it's about 33 seconds of music
      glReadPixels(0, 0, XRES, YRES, GL_RED, GL_UNSIGNED_BYTE, waveBuffer);
    }

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

