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

#version 430

// ----------------------------------------------------------------------------
// Required prelude

// Set by draw_demo
uniform vec4 state;
// The result of the shader
out vec4 fcol;

// ----------------------------------------------------------------------------
// The Shader
//  The entrypoint is: void main()
//  gl_FragCoord is the input fragment position
//  fcol is the output fragment color

// state.x  = time in seconds
// state.yx = resolution

void main() {
  vec4 gf   = gl_FragCoord;
  // The shader time but also add the x coord to
  //  make the time vary a bit on the x axis to
  //  produce a wave form
  float t   = state.x+1E-3*gf.x/state.y;

  // Music time for first frame, each pixel
  //  will be a PCM sample in the sound buffer
  float tm  = (gf.x+gf.y*state.y)/44100;

  // If time is 0 we will compute the first audi frame
  float time= state.x == 0 ? tm : t;

  // --------------------------------------------------------------------------
  // The Amazing 't% funk' by lhphr (found at https://bytebeat.demozoo.org/)
  //  Ported to GLSL
  float musicTime = 32E3*time;
  float kickTime  = musicTime/16384;
  float kick      = 1-(kickTime-floor(kickTime))*1.6;
  float wave      =
    float(
        (int(mod(musicTime,float(int(musicTime)&int(musicTime)>>12))/pow(2.,mod(kickTime*16.,4.)-3.))&127)
      + (int(pow(8e3,kick))&64
      )&255)/255;
  // --------------------------------------------------------------------------

  // y is in -1 to 1
  float y   = -1+2*gf.y/state.z;
  // Hack to produce a crummy wave from
  float d   = abs(abs(y)-wave-.125);

  // Palette generating function
  vec3 pcol = 1+sin(vec3(0,1,2)+t);
  // Compute the col for all frame but the first one
  vec3 col  =
      // The crummy wave form
      pcol*1E-4/max(d*d, 1E-5)
      // The kick
    + kick*pcol.zxy*1E-1/max(abs(y), 1E-2)
    ;

  // If time is 0 we return the audio frame
  //  otherwise "cool" graphics
  fcol = vec4(state.x == 0 ? vec3(wave): sqrt(tanh(col)), 1);
}
