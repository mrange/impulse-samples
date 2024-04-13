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

// #define PFD_HACK
//#define WAVHDR_PREPARE_HACK
#define NO_FPU

//#define FULLSCREEN

#include "straight-from-the-book.h"

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
  __forceinline void draw_demo(DWORD sample) {
    // Use the previously compiled shader program
    ((PFNGLUSEPROGRAMPROC)wglGetProcAddress(nm_glUseProgram))(fragmentShaderProgram);
    // Sets shader parameters
    ((PFNGLUNIFORM1IPROC)wglGetProcAddress(nm_glUniform1i))(0, sample);
    // Draws a rect over the entire window with fragment shader providing the gfx
    glRects(-1, -1, 1, 1);
  }

}

#pragma code_seg(".main")
#ifdef USE_CRINKLER
_declspec(naked) void entrypoint() {
#else
int __cdecl main() {
#endif

#if _DEBUG
  auto dwStyle = WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_POPUP;

  // Create the window using the STATIC class
  auto hwnd = CreateWindowExA(
    0                                             // dwExStyle
  // Special name for STATIC window class
  , reinterpret_cast<LPCSTR>(ATOM_STATIC)         // lpClassName
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

    // Draw the demo
    draw_demo(currentSample);

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

#else
  _asm {
#ifndef NO_FPU
    fldcw[fcw]
#endif

    xor esi, esi

    push esi                                   // ExitProcess.uExitCode

    push 0x20                                  // waveOutWrite.cbwh
    push offset waveHeader                     // waveOutWrite.pwh

#ifndef WAVHDR_PREPARE_HACK
    push 0x20                                  // waveOutPrepareHeader.cbwh
    push offset waveHeader                     // waveOutPrepareHeader.pwh
#endif

    push esi                                   // waveOutOpen.fdwOpen
    push esi                                   // waveOutOpen.dwInstance
    push esi                                   // waveOutOpen.dwCallback
    push offset waveFormatSpecification        // waveOutOpen.pwfx
    push - 1                                   // waveOutOpen.uDeviceID
    push offset hwo                            // waveOutOpen.phwo

    push offset fragmentShaders                // glCreateShaderProgram.strings
    push 1                                     // glCreateShaderProgram.count
    push GL_FRAGMENT_SHADER                    // glCreateShaderProgram.type
    push offset nm_glCreateShaderProgramv      // wglGetProcAddress.procName

    push esi                                   // ShowCursor.bShow

    push esi                                   // CreateWindowExA.lpParam
    push esi                                   // CreateWindowExA.hInstance
    push esi                                   // CreateWindowExA.hMenu
    push esi                                   // CreateWindowExA.hWndParent
    push esi                                   // CreateWindowExA.nHeight
    push esi                                   // CreateWindowExA.nWidth
    push esi                                   // CreateWindowExA.Y
    push esi                                   // CreateWindowExA.X
    push WS_POPUP | WS_VISIBLE | WS_MAXIMIZE   // CreateWindowExA.dwStyle
    push esi                                   // CreateWindowExA.lpWindowName
    push ATOM_STATIC                           // CreateWindowExA.lpClassName
    push esi                                   // CreateWindowExA.dwExStyle

#ifdef FULLSCREEN
    push CDS_FULLSCREEN                        // ChangeDisplaySettingsA.dwFlags
    push offset devmode                        // ChangeDisplaySettingsA.lpDevMode


    call ChangeDisplaySettingsA
#endif
    call CreateWindowExA

    push eax                                   // GetDC.hWnd

    call GetDC

    mov edi, eax

    push edi                                   // wglCreateContext.hdc
    push esi                                   // SetPixelFormat.ppfd
#ifdef PFD_HACK
    push 8                                     // SetPixelFormat.format
#else
    push offset pixelFormatSpecification       // ChoosePixelFormat.ppfd
    push edi                                   // ChoosePixelFormat.hdc

    call ChoosePixelFormat

    push eax                                   // SetPixelFormat.format
#endif
    push edi                                   // SetPixelFormat.hdc

    call SetPixelFormat

    call wglCreateContext

    push eax                                   // wglMakeCurrent.glrc
    push edi                                   // wglMakeCurrent.hdc

    call wglMakeCurrent

    call ShowCursor

    call wglGetProcAddress

    call eax                                   // glCreateShaderProgram (indirect call)
    mov fragmentShaderProgram, eax

    call waveOutOpen

#ifndef WAVHDR_PREPARE_HACK
    push [hwo]                                 // waveOutPrepareHeader.hwo

    call waveOutPrepareHeader
#endif
    push [hwo]                                 // waveOutWrite.hwo

    call waveOutWrite

    mainloop:
      push VK_ESCAPE                           // GetAsyncKeyState.vKey

      push esi                                 // PeekMessage.wRemoveMsg
      push esi                                 // PeekMessage.wMsgFilterMax
      push esi                                 // PeekMessage.wMsgFilterMin
      push esi                                 // PeekMessage.hWnd
      push esi                                 // PeekMessage.lpMsg

      push edi                                 // SwapBuffers.hdc

      push 1                                   // glRects.y2
      push 1                                   // glRects.x2
      push -1                                  // glRects.y1
      push -1                                  // glRects.x1

      push 0xC                                 // waveOutGetPosition.cbmmt
      push offset waveTime                     // waveOutGetPosition.pmmt
      push [hwo]                               // waveOutGetPosition.hwo

      call waveOutGetPosition

      mov  ebx, dword ptr[waveTime.u.sample]

      push            ebx                                 // glUniform1i.v0
      push            esi                                 // glUniform1i.location

      push            offset nm_glUniform1i               // wglGetProcAddress.procName

      push            [fragmentShaderProgram]             // glUseProgram.pid

      push            offset nm_glUseProgram              // wglGetProcAddress.procName

      call            wglGetProcAddress

      call            eax                                 // glUseProgram (indirect call)

      call            wglGetProcAddress

      call            eax                                 // glUniform1i (indirect call)

      call            glRects

      test            ebx, ebx
      jnz             noread

      push offset     waveBuffer               // glReadPixels.data
      push            GL_UNSIGNED_BYTE         // glReadPixels.type
      push            GL_RED                   // glReadPixels.format
      push            YRES                     // glReadPixels.height
      push            XRES                     // glReadPixels.width
      push            esi                      // glReadPixels.y
      push            esi                      // glReadPixels.x

      call glReadPixels

    noread:

      call SwapBuffers

      call PeekMessageA

      call GetAsyncKeyState

      test ax, ax
      jne exit

      cmp ebx, SU_LENGTH_IN_SAMPLES
      jl mainloop

  exit:

    call ExitProcess
  }
#endif
}

