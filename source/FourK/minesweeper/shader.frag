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

const int 
  cells       = 12
, state_size  = 2
;

// The result of the shader
layout(location=0) out vec4 fcol;
// Set by draw_game
layout(location=0) uniform vec4[12*12+2] state;

// ----------------------------------------------------------------------------
// The Shader
//  The entrypoint is: void main()
//  gl_FragCoord is the input fragment position
//  fcol is the output fragment color

// License: WTFPL, author: sam hocevar, found: https://stackoverflow.com/a/17897228/418488
const vec4 hsv2rgb_K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
vec3 hsv2rgb(vec3 c) {
  vec3 p = abs(fract(c.xxx + hsv2rgb_K.xyz) * 6.0 - hsv2rgb_K.www);
  return c.z * mix(hsv2rgb_K.xxx, clamp(p - hsv2rgb_K.xxx, 0.0, 1.0), c.y);
}


// License: MIT OR CC-BY-NC-4.0, author: mercury, found: https://mercury.sexy/hg_sdf/
// License: MIT, author: Inigo Quilez, found: https://iquilezles.org/www/articles/distfunctions2d/distfunctions2d.htm
float box(vec2 p, vec2 b) {
  vec2 d = abs(p)-b;
  return length(max(d,0.0)) + min(max(d.x,d.y),0.0);
}

float dsegmentx(vec2 p, vec2 dim) {
  p.x = abs(p.x);
  float o = 0.5*max(dim.x-dim.y, 0.0);
  if (p.x < o) {
    return abs(p.y) - dim.y;
  }
  return length(p-vec2(o, 0.0))-dim.y;
}

vec3 digit(vec3 col, vec2 p, vec3 acol, vec3 icol, float aa, float n, float t) {
  const int[16] digits = int[16](
    0x7D // 0
  , 0x50 // 1
  , 0x4F // 2
  , 0x57 // 3
  , 0x72 // 4
  , 0x37 // 5
  , 0x3F // 2
  , 0x51 // 7
  , 0x7F // 8
  , 0x77 // 9
  , 0x7B // A
  , 0x3E // B
  , 0x2D // C
  , 0x5E // D
  , 0x2F // E
  , 0x2B // F
  ); 
  const vec2 dim = vec2(0.75, 0.075);
  const float eps = 0.1;
  vec2 ap = abs(p);
  if (ap.x > (0.5+dim.y+eps)||ap.y > (1.0+dim.y+eps)) return col;
  float m = mod(floor(n), 16.0);
  int digit = digits[int(m)];

  vec2 cp = (p-0.5);
  vec2 cn = round(cp);

  vec2 p0 = p;
  p0.y -= 0.5;
  p0.y = p0.y-0.5;
  float n0 = round(p0.y);
  p0.y -= n0;
  float d0 = dsegmentx(p0, dim);

  vec2 p1 = p;
  vec2 n1 = sign(p1); 
  p1 = abs(p1);
  p1 -= 0.5;
  p1 = p1.yx;
  float d1 = dsegmentx(p1, dim);
  
  vec2 p2 = p;
  p2.y = abs(p.y);
  p2.y -= 0.5;
  p2 = abs(p2);
  float d2 = dot(normalize(vec2(1.0, -1.0)), p2);

  float d = d0;
  d = min(d, d1);

  float sx = 0.5*(n1.x+1.0) + (n1.y+1.0);
  float sy = -n0;
  float s  = d2 > 0.0 ? (3.0+sx) : sy;
  // Praying bit shift operations aren't TOO slow
  vec3 scol = ((digit & (1 << int(s))) == 0) ? icol : acol;  

  col = mix(col, scol, smoothstep(aa, -aa, d)*t);
  return col;
}

// License: MIT OR CC-BY-NC-4.0, author: mercury, found: https://mercury.sexy/hg_sdf/
vec2 mod2(inout vec2 p, vec2 size) {
  vec2 c = floor((p + size*0.5)/size);
  p = mod(p + size*0.5,size) - size*0.5;
  return c;
}


void main() {
  const float bw = 0.9;
  const float sw = bw/(cells*0.5);

  float tm = state[0].x;
  vec2 res = state[0].yz;

  vec2 q = gl_FragCoord.xy/res;
  vec2 p = -1. + 2. * q;

  p.x *= res.x/res.y;

  float aa = sqrt(2.) / res.y;

  vec3 col = vec3(0.);
  vec2 p0 = p;
  vec2 cp = p;
  const float cz = sw;
  float caa = aa/cz;
  cp /= cz;
  cp -= 0.5;
  vec2 np = round(cp);
  cp -= np;
  np += cells*0.5;

  const float tcw = 0.1;
  const float tz  = 0.04;
  vec2 tcp = p;
  tcp.x -= -0.5*0.75*tcw;
  tcp.y -= -0.95;
  vec2 tnp = mod2(tcp, vec2(0.75*tcw, tcw));
  tcp /= tz;
  float taa = aa/(tz);

  int ci = clamp(int((np.x)+(np.y)*cells+state_size), 2, state.length()-1);
  vec4 c = state[ci];

  float cs = c.x;
  
  float d0 = box(p0, vec2(bw));
  if (d0 < 0.) {
    float d1 = box(cp, vec2(0.4))-0.05;
    d1 = abs(d1)-0.0125;
    float fz = 0.25;
    vec2 fcp = cp/fz;
    float faa = aa/fz;
    if (cs < 0.) {
      vec3 acol = hsv2rgb(vec3(0.3+0.3*cs/9, 0.5, 1.));
      vec3 icol = acol*0.1;
      col = digit(col, fcp, acol, icol, faa, -cs, 1);
    }
    col = mix(col, vec3(1.), smoothstep(caa, -caa, d1));
  }

  if (tnp.y == 0 && abs(tnp.x-0.5) < 5.) {
    float d = mod(tm*pow(10, tnp.x), 10.);
    vec3 acol = hsv2rgb(vec3(0.95, 0.9, 1.));
    vec3 icol = acol*0.1;
    col = digit(col, tcp, acol, icol, taa, d, 1);
  }

  col = sqrt(col);
  
  fcol = vec4(col, 1.);
}
