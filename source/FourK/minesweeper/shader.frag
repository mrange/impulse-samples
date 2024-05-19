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
  CELLS       = 12
, STATE_SIZE  = 2
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

const float 
    BORDER_DIM  = .9
  , CELL_DIM    = BORDER_DIM/(CELLS*.5)
  , tcw         = .1
  , fz          = .25
  , tz          = .04
  , cz          = CELL_DIM
  , tr          =.75
  , deps        = .1
  ;

const vec2 
    ddim = vec2(.75, .075)
  ;

const int[16] ddigits = int[16](
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
// License: WTFPL, author: sam hocevar, found: https://stackoverflow.com/a/17897228/418488
const vec4 
      hsv2rgb_K = (vec4(3,2,1,9)/3)
    ;

// License: WTFPL, author: sam hocevar, found: https://stackoverflow.com/a/17897228/418488
vec3 hsv2rgb(vec3 c) {
  vec3 p = abs(fract(c.xxx + hsv2rgb_K.xyz) * 6 - hsv2rgb_K.www);
  return c.z * mix(hsv2rgb_K.xxx, clamp(p - hsv2rgb_K.xxx, 0, 1), c.y);
}

vec4 hsv2rgb(vec4 c) {
  return vec4(hsv2rgb(vec3(c.xyz)), c.w);
}

const vec3
    mouseCol      = hsv2rgb(vec3(.4, .5, 2E-3))
  , mouseVisitCol = hsv2rgb(vec3(.55, .5, 2E-3))
  , timeCol       = hsv2rgb(vec3(.95, .9, 1.)) 
  ;

const vec4[6] stateCol = vec4[6](
    hsv2rgb(vec4(0.0 , 0.0, 0.0  , 0.))    // covered_empty
  , hsv2rgb(vec4(0.55, 0.7, 1.0  , 0.125)) // covered_empty
  , hsv2rgb(vec4(0.40, 0.7, 1.0  , 0.5))   // covered_flag 
  , hsv2rgb(vec4(0.00, 0.0, 1.0  , 1.))    // uncovering   
  , hsv2rgb(vec4(0.00, 0.8, 1.0  , 1.))    // exploding    
  , hsv2rgb(vec4(0.00, 0.8, 0.25 , 0.5))   // exploded     
  );


// License: MIT OR CC-BY-NC-4.0, author: mercury, found: https://mercury.sexy/hg_sdf/
vec2 mod2(inout vec2 p, vec2 size) {
  vec2 c = floor((p + size*.5)/size);
  p = mod(p + size*.5,size) - size*.5;
  return c;
}


// License: MIT, author: Inigo Quilez, found: https://iquilezles.org/www/articles/distfunctions2d/distfunctions2d.htm
float box(vec2 p, vec2 b) {
  vec2 d = abs(p)-b;
  return length(max(d,0)) + min(max(d.x,d.y),0);
}

float dsegmentx(vec2 p, vec2 dim) {
  p.x = abs(p.x);
  float o = .5*max(dim.x-dim.y, 0);
  return (p.x < o ? abs(p.y) : length(p-vec2(o, 0)))-dim.y;  
}

vec3 digit(vec3 col, vec2 p, vec3 acol, vec3 icol, float aa, float n, float t) {

  vec2 
      ap = abs(p)
    , cp = p-.5
    , cn = round(cp)
    , p0 = p
    , p1 = p
    , n1 = sign(p1)
    , p2 = p
    ;
    
  if (ap.x > (.5+ddim.y+deps)||ap.y > (1+ddim.y+deps)) return col;

  p0.y -= 1;
  float n0 = round(p0.y);
  p0.y -= n0;

  p1 = abs(p1);
  p1 = p1.yx;
  p1 -= .5;
  
  p2.y  = abs(p.y);
  p2.y  -= .5;
  p2    = abs(p2);

  float 
      d0  = dsegmentx(p0, ddim)
    , d1  = dsegmentx(p1, ddim)
    , d2  = dot(normalize(vec2(1.0, -1.0)), p2)
    , d   = min(d0, d1)
    , sx  = .5*(n1.x+1) + n1.y+1
    , sy  = -n0
    , s   = d2 > 0 ? (3+sx) : sy
    , m   = floor(n)
    ;

  int digit = ddigits[int(m)];

  // Praying bit shift operations aren't TOO slow
  vec3 scol = ((digit & (1 << int(s))) == 0) ? icol : acol;  

  col = mix(col, scol, smoothstep(aa, -aa, d)*t);
  return col;
}

void main() {
  vec3 
      col = vec3(0)
    ;

  vec2  
      res = state[0].yz
    , p   = (-res+2*gl_FragCoord.xy)/res.yy
    , mp  = (-res+2*state[1].xy)/res.yy
    , p0  = p
    , cp  = p
    , tcp = p
    ;

  float 
      tm  = state[0].x
    , aa  = sqrt(2) / res.y
    , caa = aa/cz
    , taa = aa/tz
    , faa = aa/(fz*cz)
    ;

  mp.y     = -mp.y;

  cp /= cz;
  cp -= .5;

  tcp.x -= (-.5*tr)*tcw;
  tcp.y -= -.95;

  vec2 
      tnp = mod2(tcp, vec2(tr*tcw, tcw))
    , np = round(cp)
    ;

  cp -= np;
  np += CELLS*.5;

  tcp /= tz;

  int ci = int((np.x)+(np.y)*CELLS+STATE_SIZE);
  vec4 c = state[ci];

  float 
      cs  = c.x
    , mts = c.z
    , d0  = box(p0, vec2(BORDER_DIM))
    , d1  = box(cp, vec2(.4))-.05;
    ;

  if (d0 < 0) {
    col += mouseVisitCol*(1./max(dot(cp, cp)+smoothstep(mts+.125, mts+3., tm), 1E-3)); 
    if (cs < 0) {
      vec2 fcp = cp/fz;
      fcp.x += -fcp.y/8;
      vec3 
          acol = 0.5+0.5*sin(.5*vec3(-4,3,1)+0.33*cs-0.5*fcp.y)
        , icol = acol*.075
        ;
      col = digit(col, fcp, acol, icol, faa, -cs, 1);
    } else {
      vec4 scol = stateCol[int(cs)]; 
      col = mix(col, scol.xyz, smoothstep(caa, -caa, d1)*scol.w);      
    }
    d1 = abs(d1)-.0125;
    col = mix(col, vec3(1), smoothstep(caa, -caa, d1));
  }

  if (tnp.y == 0 && abs(tnp.x-.5) < 5) {
    float d = mod(tm*pow(10, tnp.x), 10);
    vec3 
        acol = timeCol 
      , icol = timeCol*.075
      ;
    col = digit(col, tcp, acol, icol, taa, d, 1);
  }

  col += mouseCol/max(length(p-mp), 1E-3); 

  col = sqrt(col);
  
  fcol = vec4(col, 1);
}
