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
uniform int sample;
// The result of the shader
out vec4 fcol;

// ----------------------------------------------------------------------------
// The Shader
//  The entrypoint is: void main()
//  gl_FragCoord is the input fragment position
//  fcol is the output fragment color

mat2 rot(float a) {
  return mat2(cos(a),sin(a),-sin(a),cos(a));
}

void main() {
  vec2 
    resolution = vec2(1920, 1080)
  , p = (-resolution+2*gl_FragCoord.xy)/resolution.yy
  ;

  float 
    musicTime = (sample==0?(gl_FragCoord.x+resolution.x*gl_FragCoord.y):(sample+.5*gl_FragCoord.x))*320/441
  , kickTime  = musicTime/16384
  , nkickTime = floor(kickTime)
  , kick      = 1.-(kickTime-nkickTime)*1.6
  , wave      = float((int(mod(musicTime,int(musicTime)&int(musicTime)>>12)/pow(2,mod(kickTime*16,4)-3))&127)+(int(pow(8e3,kick))&64)&255)/255
  , l         = length(p)
  ;
  
  vec3
    col = vec3(0.)
  , bcol =vec3(.125, .25, .5)
  ;
  
  if (wave+abs(p.y) > 1) col.x += .25;


  p *= (1.5+.5*sin(-4*(l+kick)))*rot(-nkickTime+.5*l);

  for (int i = 10; i > 0; --i) {
    ivec2 ip = ivec2(rot(-.1*kick*i)*p*12);
    if (abs(ip.x^ip.y)%99%(43^int(nkickTime+20*-abs(p.x*p.y)*step(64,nkickTime)))<i)
      col += bcol*exp(-.2*i);
  }
  col *= l*l;
  col += .5*kick*kick/l*sqrt(bcol);
  fcol = vec4(smoothstep(91, 89, kickTime)*(sample==0?vec3(wave):col), 1);
}
