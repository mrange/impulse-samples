// Generated with Shader Minifier 1.3.6 (https://github.com/laurentlb/Shader_Minifier/)
#ifndef SHADER_MINIFIER_IMPL
#ifndef SHADER_MINIFIER_HEADER
# define SHADER_MINIFIER_HEADER
# define VAR_fcol "f"
# define VAR_state "v"
#endif

#else // if SHADER_MINIFIER_IMPL

// C:\code\github\impulse-samples\source\FourK\minesweeper\shader.fx
"#version 430\n"
 "uniform vec4 v;"
 "out vec4 f;"
 "vec2 x(inout vec2 v)"
 "{"
   "vec2 f=vec2(.15),z=floor((v+f*.5)/f);"
   "v=mod(v+f*.5,f)-f*.5;"
   "return z;"
 "}"
 "float x(vec2 v,vec2 f)"
 "{"
   "vec2 z=abs(v)-f;"
   "return length(max(z,0.))+min(max(z.x,z.y),0.);"
 "}"
 "void main()"
 "{"
   "vec2 z=gl_FragCoord.xy/v.yz,m=-1.+2.*z;"
   "m.x*=v.y/v.z;"
   "float i=sqrt(2.)/v.z;"
   "vec3 r=vec3(0);"
   "vec2 l=m,o=m;"
   "o+=.075;"
   "x(o);"
   "float C=x(l,vec2(.9)),s=x(o,vec2(.0675));"
   "s=abs(s)-.003;"
   "s=max(C,s);"
   "r=mix(r,vec3(1),smoothstep(i,-i,s));"
   "r=sqrt(r);"
   "f=vec4(r,1);"
 "}",

#endif
