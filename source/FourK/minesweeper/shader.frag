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
    BORDER_DIM      = .9
  , CELL_DIM        = BORDER_DIM/(CELLS*.5)
  , PI              = acos(-1)
  , TAU             = 2*PI
  , tcw             = .1
  , fz              = .25
  , tz              = .04
  , cz              = CELL_DIM
  , tr              = .75
  , deps            = .1
  ;

const vec2 
    ddim  = vec2(.75, .075)
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

float circle8(vec2 p, float r) {
  p *= p;
  p *= p;
  return pow(dot(p,p), 1./8)-r;
}

vec3 norm8(vec2 p, float r, float i) {
  vec2 p4 = p*p;
  p4 *= p4;
  float r8 = r*r;
  r8 *= r8;
  r8 *= r8;
  float z8 = r8-dot(p4, p4);
  if (z8 > 0) {
    float z = pow(z8, 1./8);
    vec3 cp = vec3(p, z*i);
    vec3 cp2 = cp*cp;
    vec3 cp7 = cp2*cp2;
    cp7 *= cp2*cp;
    vec3 cn = normalize(cp7);
    return normalize(cp7);
  } else {
    return vec3(0,0,1);
  }
}
  
  
float segmentx(vec2 p, vec2 dim) {
  p.x = abs(p.x);
  float o = .5*max(dim.x-dim.y, 0);
  return (p.x < o ? abs(p.y) : length(p-vec2(o, 0)))-dim.y;  
}

vec3 palette(float a) {
  return 1+sin(.5*vec3(-4,3,1)+a);
}


// License: MIT OR CC-BY-NC-4.0, author: mercury, found: https://mercury.sexy/hg_sdf/
vec2 mod2(inout vec2 p, vec2 size) {
  vec2 c = floor((p + size*.5)/size);
  p = mod(p + size*.5,size) - size*.5;
  return c;
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
      d0  = segmentx(p0, ddim)
    , d1  = segmentx(p1, ddim)
    , d2  = dot(normalize(vec2(1, -1)), p2)
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
  vec3 
      col = vec3(0.)
    , p3  = vec3(p, 0.)
    , mp3 = vec3(mp, 1.)
    , rd3 = normalize(p3-vec3(0,0,10.))
    , ld3 = normalize(mp3-p3)
    , ld0 = normalize(vec3(2.,3.,3.))
    ;


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

  float fi = (np.x)+(np.y)*CELLS+STATE_SIZE;
  vec4 c = state[int(fi)];

  float 
      cs  = c.x
    , mts = c.z
    , d1  = circle8(cp, 0.45);
    ;
  
    
  vec3 n = norm8(cp, 0.45-1/80., cs > 0 ? 1:1);

  if (max(abs(p0).x, abs(p0).y) < BORDER_DIM) {

    vec3 ccol = col*0.25; 

    float spe0 = pow(max(dot(ld0, reflect(rd3, n)), 0.), 40);
    float gd = length(cp);
    const vec2 states[6] = vec2[6](
      vec2(0  ,0)
    , vec2(0.5,0)
    , vec2(2  ,1)
    , vec2(10 ,0)
    , vec2(10 ,1)
    , vec2(2  ,0)
    );
    
    vec2 state = states[int(cs)];
    if (state.y > 0)  gd = min(abs(gd-0.1), gd);
    vec3 scol =(0.2+palette(2.-1.05*cs))*(state.x*5E-3/max(gd, 3E-3));
    
    ccol = mix(ccol, scol, smoothstep(caa, -caa, d1));      

    if (cs < 0) {
      vec2 fcp = cp/fz;
      fcp.x += -fcp.y/8;
      vec3 
          acol = palette(.33*cs-.5*fcp.y)
        , icol = acol*.075
        ;
      ccol += acol*1E-2/max(length(fcp), 5E-1);
      ccol = digit(ccol, fcp, acol, icol, faa, -cs, 1);
    }

    vec3 gcol = palette(3.5-p.y);

    ccol += spe0*step(1., cs);
    col = mix(col, ccol, smoothstep(caa, -caa, d1));
    d1 = abs(d1)-1/80.;
    col = mix(col, mix(vec3(1.), gcol/3, smoothstep(mts+1/8., mts+0.5, tm)), smoothstep(caa, -caa, d1));
  }

  if (tnp.y == 0 && abs(tnp.x-.5) < 5) {
    float d = mod(tm*pow(10, tnp.x), 10);
    vec3 
        acol = palette(-4.*p.y+(tnp.x < 1 ? 0:3))
      , icol = acol*.075
      ;
    col = digit(col, tcp, acol, icol, taa, d, 1);
  }

  col += palette(tm)*(1E-3/max(length(p-mp), 1E-3)); 

  fcol = vec4(sqrt(tanh(col)), 1);
}
