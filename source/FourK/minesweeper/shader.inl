// Generated with Shader Minifier 1.3.6 (https://github.com/laurentlb/Shader_Minifier/)
#ifndef SHADER_MINIFIER_IMPL
#ifndef SHADER_MINIFIER_HEADER
# define SHADER_MINIFIER_HEADER
# define VAR_fcol "f"
# define VAR_state "v"
#endif

#else // if SHADER_MINIFIER_IMPL

// C:\code\github\impulse-samples\source\FourK\minesweeper\shader.frag
"#version 430\n"
 "layout(location=0) uniform vec4 v;"
 "layout(location=0) out vec4 f;"
 "vec2 t(inout vec2 v)"
 "{"
   "vec2 f=vec2(.15),n=floor((v+f*.5)/f);"
   "v=mod(v+f*.5,f)-f*.5;"
   "return n;"
 "}"
 "float t(vec2 v,vec2 f)"
 "{"
   "vec2 n=abs(v)-f;"
   "return length(max(n,0.))+min(max(n.x,n.y),0.);"
 "}"
 "void main()"
 "{"
   "vec2 n=gl_FragCoord.xy/v.yz,m=-1.+2.*n;"
   "m.x*=v.y/v.z;"
   "float l=sqrt(2.)/v.z;"
   "vec3 r=vec3(0);"
   "vec2 i=m,s=m;"
   "s+=.075;"
   "t(s);"
   "float o=t(i,vec2(.9)),y=t(s,vec2(.0675));"
   "y=abs(y)-.003;"
   "y=max(o,y);"
   "r=mix(r,vec3(1),smoothstep(l,-l,y));"
   "r=sqrt(r);"
   "f=vec4(r,1);"
 "}",

#endif
