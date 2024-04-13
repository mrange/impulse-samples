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

vec3 music(float time) {
  // The Amazing 't% funk' by lhphr (found at https://bytebeat.demozoo.org/)
  float musicTime = 32000.*time;
  float kickTime  = musicTime/16384.;
  float nkickTime = floor(kickTime);
  float kick      = 1.-(kickTime-nkickTime)*1.6;
  float wave      = 
    float(
        (int(mod(musicTime,float(int(musicTime)&int(musicTime)>>12))/pow(2.,mod(kickTime*16.,4.)-3.))&127)
      + (int(pow(8e3,kick))&64
      )&255)/255.;
  return vec3(wave, kick,nkickTime);
}

void main() {
  vec4 gf   = gl_FragCoord;
  float t   = (gf.x+gf.y*state.y)/44100.;
  vec3 m    = music(t);
  vec3 col  = vec3(m.x);
  fcol = vec4(col, 1.0);
}
