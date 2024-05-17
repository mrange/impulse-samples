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
layout(location=0) uniform vec4 state;
// The result of the shader
layout(location=0) out vec4 fcol;

// ----------------------------------------------------------------------------
// The Shader
//  The entrypoint is: void main()
//  gl_FragCoord is the input fragment position
//  fcol is the output fragment color

// License: MIT OR CC-BY-NC-4.0, author: mercury, found: https://mercury.sexy/hg_sdf/
vec2 mod2(inout vec2 p, vec2 size) {
  vec2 c = floor((p + size*0.5)/size);
  p = mod(p + size*0.5,size) - size*0.5;
  return c;
}

// License: MIT, author: Inigo Quilez, found: https://iquilezles.org/www/articles/distfunctions2d/distfunctions2d.htm
float box(vec2 p, vec2 b) {
  vec2 d = abs(p)-b;
  return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

void main() {
  // Test
  vec2 q = gl_FragCoord.xy/state.yz;
  vec2 p = -1. + 2. * q;
  p.x *= state.y/state.z;

  float aa = sqrt(2.) / state.z;

  vec3 col = vec3(0.);
  const float bw = 0.9;
  const float sw = bw/6.;
  vec2 p0 = p;
  vec2 p1 = p;
  p1 += sw*0.5;
  mod2(p1, vec2(sw));
  
  float d0 = box(p0, vec2(bw));
  float d1 = box(p1, vec2(sw*.45));
  d1 = abs(d1)-sw*0.02;
  d1 = max(d0, d1);
  col = mix(col, vec3(1.), smoothstep(aa, -aa, d1));
  
  col = sqrt(col);
  
  fcol = vec4(col, 1.);
}
