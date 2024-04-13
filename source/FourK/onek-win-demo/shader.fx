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

void main() {
  vec4 gf   = gl_FragCoord;
  float t   = state.x+1E-3*gf.x/state.y;
  float tm  = (gf.x+gf.y*state.y)/44100;
  float time= state.x == 0 ? tm : t;

  // The Amazing 't% funk' by lhphr (found at https://bytebeat.demozoo.org/)
  float musicTime = 32E3*time;
  float kickTime  = musicTime/16384;
  float kick      = 1-(kickTime-floor(kickTime))*1.6;
  float wave      = 
    float(
        (int(mod(musicTime,float(int(musicTime)&int(musicTime)>>12))/pow(2.,mod(kickTime*16.,4.)-3.))&127)
      + (int(pow(8e3,kick))&64
      )&255)/255;

  
  float y   = -1+2*gf.y/state.z;
  float d   = abs(abs(y)-wave-.125);
  
  vec3 pcol = 1+sin(vec3(0,1,2)+t);
  vec3 col  = 
      pcol*1E-4/max(d*d, 1E-5)
    + kick*pcol.zxy*1E-1/max(abs(y), 1E-2)
    ;

  fcol = vec4(state.x == 0 ? vec3(wave): sqrt(tanh(col)), 1);
}
