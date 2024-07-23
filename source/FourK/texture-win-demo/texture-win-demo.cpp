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

#include "texture-win-demo.h"

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

  #pragma code_seg(".lcg_rand_uint32")
  uint32_t lcg_rand_uint32(uint32_t max) {
    lcg_state = (1664525U * lcg_state + 1013904223U);
    uint64_t v = static_cast<uint64_t>(lcg_state)*max;
    return static_cast<uint32_t>(v >> 32);
  }

  #pragma code_seg(".init_demo")
  void init_demo() {
    lcg_state = 0x19740531;
    // Bit of debugging info during debug builds
    //  Don't want to waste bytes on that in Release mode
#ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    ((PFNGLDEBUGMESSAGECALLBACKPROC)wglGetProcAddress("glDebugMessageCallback"))(debugCallback, 0);
#endif

  // Compiles the provided fragment shader into a shader program
  fragmentShaderProgram = ((PFNGLCREATESHADERPROGRAMVPROC)wglGetProcAddress(nm_glCreateShaderProgramv))(GL_FRAGMENT_SHADER, 1, fragmentShaders);

  glGenTextures(1, &tex);

  glBindTexture(GL_TEXTURE_2D, tex);

  GLubyte data[64*64*4];
  for (auto i = 0; i < 64*64*4; ++i) {
    data[i] = static_cast<GLubyte>(lcg_rand_uint32(256));
  }
  
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA8, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

#ifdef _DEBUG
  ((PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog"))(fragmentShaderProgram, sizeof(debugLog), NULL, debugLog);
  printf(debugLog);
  glDisable(GL_DEBUG_OUTPUT);
#endif
  }

  #pragma code_seg(".draw_demo")
  void draw_demo(float time) {
    ((PFNGLACTIVETEXTUREPROC)wglGetProcAddress(nm_glActiveTexture))(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


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
    ((PFNGLUNIFORM1IPROC)wglGetProcAddress(nm_glUniform1i))(
        1 // Uniform location
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

  // Create dummy OpenGL Context
  auto dummy_hglrc = wglCreateContext(hdc);
  assert(dummy_hglrc);

  // Sets the dummy OpenGL context as the current one
  auto dummy_makeOk = wglMakeCurrent(hdc, dummy_hglrc);
  assert(dummy_makeOk);

  // Load WGL extensions
  auto fp_wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

  // Reset the current context
  auto dummy_unmakeOk = wglMakeCurrent(nullptr, nullptr);
  assert(dummy_unmakeOk);

  // Delete dummy OpenGL Context
  auto dummy_deleteOk = wglDeleteContext(dummy_hglrc);
  assert(dummy_deleteOk);

  // WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB

  // Create OpenGL Context
  int const attrib_list[] = {
      WGL_CONTEXT_MAJOR_VERSION_ARB , 4,
      WGL_CONTEXT_MINOR_VERSION_ARB , 5,
      WGL_CONTEXT_PROFILE_MASK_ARB  , WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
      0  // End of attributes list
  };

  auto hglrc = fp_wglCreateContextAttribsARB(hdc,0, attrib_list);
  assert(hglrc);

  // Sets the OpenGL context as the current one
  auto makeOk = wglMakeCurrent(hdc, hglrc);
  assert(makeOk);



  // Init our demo
  init_demo();

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

    auto demoTime = 0.F;

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

