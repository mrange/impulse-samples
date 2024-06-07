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

const float 
  STATE_SLEEP = 1./8
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
  , textChars[12]   = float[](
        5   // S
      , 12  // C
      , 0   // O
      , 17  // R
      , 14  // E
      , 19  // BLANK
      , 16  // L
      , 14  // E
      , 15  // F
      , 18  // T
      , 19  // BLANK
      , 19  // BLANK
      )
  ;

vec2
    ddim        = vec2(.75, .075)
  , bstates[6]  = vec2[](
      vec2(0  ,0)
    , vec2(.5 ,0)
    , vec2(2  ,2)
    , vec2(10 ,1)
    , vec2(10 ,2)
    , vec2(4  ,1)
    )
  ;

int ddigits[20] = int[](
    0x7D // 0, O
  , 0x50 // 1
  , 0x4F // 2
  , 0x57 // 3
  , 0x72 // 4
  , 0x37 // 5, S
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
  , 0x2C // L
  , 0xFB // R
  , 0x29 // T
  , 0x00 // BLANK
  )
  ;

vec3 palette(float a) {
  return 1+sin(vec3(-4,3,1)/2+a);
}


// License: MIT OR CC-BY-NC-4.0, author: mercury, found: https://mercury.sexy/hg_sdf/
vec2 mod2(inout vec2 p, vec2 size) {
  vec2 c = floor((p + size/2)/size);
  p = mod(p + size/2,size) - size/2;
  return c;
}

float circle8(vec2 p, float r) {
  p *= p;
  p *= p;
  return pow(dot(p,p), 1./8)-r;
}

vec3 norm8(vec2 p, float r) {
  vec2 p4 = p*p;
  p4 *= p4;
  float r8 = r*r;
  r8 *= r8;
  r8 *= r8;
  float z8 = r8-dot(p4, p4);
  if (z8 > 0) {
    float z = pow(z8, 1./8);
    vec3 cp = vec3(p, z);
    vec3 cp2 = cp*cp;
    vec3 cp7 = cp2*cp2;
    cp7 *= cp2*cp;
    return normalize(cp7);
  } else {
    return vec3(0,0,1);
  }
}


float segmentx(vec2 p, vec2 dim) {
  p.x = abs(p.x);
  float o = max(dim.x-dim.y, 0)/2;
  return (p.x < o ? abs(p.y) : length(p-vec2(o, 0)))-dim.y;
}

vec3 digit(vec2 p, vec3 acol, vec3 icol, float aa, float n) {
  vec2
      ap = abs(p)
    , cp = p-.5
    , cn = round(cp)
    , p0 = p
    , p1 = p
    , n1 = sign(p1)
    , p2 = p
    ;

  if (ap.x > (.5+ddim.y+deps)||ap.y > (1+ddim.y+deps)) return vec3(0);

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
    , sx  = (n1.x+1)/2 + n1.y+1
    , sy  = -n0
    , s   = d2 > 0 ? (3+sx) : sy
    , m   = floor(n)
    ;

  int digit = ddigits[int(m)];

  // Praying bit shift operations aren't TOO slow
  vec3 scol = ((digit & (1 << int(s))) == 0) ? icol : acol;

  return scol*smoothstep(aa, -aa, d);
}

// License: Unknown, author: Unknown, found: don't remember
float hash(vec2 co) {
  return fract(sin(dot(co.xy ,vec2(12.9898,58.233))) * 13758.5453);
}

vec3 background(vec2 p, float tm) {
  vec3
      col   = vec3(0)
    , ro    = vec3(0,0,tm)
    , rd    = normalize(vec3(p,2))
    , ard   = abs(rd)
    ;
  float
      maxDist = 1E3
    , srdx    = sign(rd.x)
    ;

  for (int i = 1; i < 10; ++i) {
    float tw = -(ro.x-6*sqrt(i))/ard.x;

    vec3 wp = ro+rd*tw;

    vec2
        wp2 = wp.yz*2E-2
      , wn2 = round(wp2)
      , wc2 = wp2 - wn2
      ;

    if (hash(wn2+i+.5*sign(rd.x)) < .5) {
      wc2 = vec2(wc2.y, -wc2.x);
    }

    float
        fo = smoothstep(-.7, 1., sin(.1*wp.z+tm+i+srdx))
      , wd0 = length(wc2+.5)-.5
      , wd1 = length(wc2-.5)-.5
      , wd  = abs(min(wd0, wd1))-.025
      ;

    col += palette(5E-2*tw+tm)*exp(-3E-3*tw*tw)*25E-4/max(abs(wd), 3E-3*fo)*fo;
  }

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
    , gtm = state[0].w
    , sco = state[1].z
    , rem = state[1].w
    , aa  = sqrt(2) / res.y
    , caa = aa/cz
    , taa = aa/tz
    , faa = aa/(fz*cz)
    , sty = sign(tcp.y)
    ;

  mp.y     = -mp.y;
  vec3
      col = background(p, tm*.5)
    , p3  = vec3(p, 0)
    , mp3 = vec3(mp, 1)
    , rd3 = normalize(p3-vec3(0,0,10))
    , ld3 = normalize(mp3-p3)
    , ld0 = normalize(vec3(2,3,3))
    , mouseCol  = sqrt(palette(tm))
    ;

  cp /= cz;
  cp -= .5;

  tcp.x -= -tcw*tr/2;
  tcp.y = abs(tcp.y);
  tcp.y -= .95;

  vec2
      tnp = mod2(tcp, vec2(tr*tcw, tcw))
    , np = round(cp)
    ;
  tcp.y *= sty;
  cp -= np;
  np += CELLS/2;

  tcp /= tz;

  float fi = (np.x)+(np.y)*CELLS+STATE_SIZE;

  if (tnp.y == 0 && abs(tnp.x-.5) < 6) {
    float
      v = sty > 0 ? rem : sco
    , d = tnp.x > 0 ? mod(v*pow(10, tnp.x-6), 10) : textChars[int(tnp.x+5+3*(sty+1.))];
    vec3
        acol = palette(2.5+1.5*sty+0.4*tcp.y+(tnp.x < 1 ? 0:3))
      , icol = acol*.075
      ;
    col += digit(tcp, acol, icol, taa, d);
  }

  if (max(abs(p0).x, abs(p0).y) < BORDER_DIM) {
    vec4 c = state[int(fi)];

    float
        cts = c.z
      , mts = c.w
      , d1  = circle8(cp, 0.45)
      , mfo = smoothstep(mts+1./2, mts+1./8, tm)
      , sfo = smoothstep(cts, cts+STATE_SLEEP, tm)
      ;

    vec3
        n     = norm8(cp, 0.45-1./80-mfo/40)
      , ccol  = tanh(8*col)/8
      ;
    float fre = 1+dot(n, rd3);



    float
        spe0 = pow(max(dot(ld0, reflect(rd3, n)), 0.), 22)
      , spe3 = pow(max(dot(ld3, reflect(rd3, n)), 0.), 44)
      ;

    for (int i = 0; i < 2; ++i) {
      float cs    = i == 0?c.y:c.x;
      float m     = i == 0?1-sfo:sfo;
      vec2 bstate = bstates[int(cs)];
      float gd = abs(length(cp)-.1*mfo);
      for (float yy = 0; yy < bstate.y; ++yy) {
        gd = min(abs(gd-.1), gd);
      }
      vec3 scol =(.2+palette(2-cs))*(bstate.x*5E-3/max(gd, 3E-3));


      if (cs < 1) {
        vec2 fcp = cp/fz;
        fcp.x += -fcp.y/8;
        vec3
            acol = palette(cs/2-fcp.y/2)
          , icol = acol/20
          ;
        if (cs < 0) {
  //        ccol += acol*1E-2/max(length(fcp), 5E-1);
          ccol += digit(fcp, acol, icol, faa, -cs);
        }
      } else {
        ccol   = mix(ccol, scol,m*smoothstep(caa, -caa, d1));
      }
      ccol += m*(spe0/4+spe3*mouseCol)*fre*16*step(1, cs);
    }


    col = mix(col, ccol, smoothstep(caa, -caa, d1));
    d1 = abs(d1)-1./80;
    col = mix(col, mix(palette(3.+p.y)/4,vec3(1), mfo), smoothstep(caa, -caa, d1));
  }

  col += mouseCol*(1E-3/max(length(p-mp), 1E-3));

  fcol = vec4(sqrt(tanh(col)), 1);
}
