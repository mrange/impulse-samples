// Generated with Shader Minifier 1.3.6 (https://github.com/laurentlb/Shader_Minifier/)
#ifndef SHADER_MINIFIER_IMPL
#ifndef SHADER_MINIFIER_HEADER
# define SHADER_MINIFIER_HEADER
# define VAR_fcol "d"
# define VAR_state "v"
#endif

#else // if SHADER_MINIFIER_IMPL

// C:\code\github\impulse-samples\source\FourK\onek-win-demo\shader.fx
"#version 430\n"
 "uniform vec4 v;"
 "out vec4 d;"
 "vec3 t(float v)"
 "{"
   "float f=3.2e4*v,i=f/16384.,d=floor(i),m=1.-(i-d)*1.6;"
   "return vec3(float((int(mod(f,float(int(f)&int(f)>>12))/pow(2.,mod(i*16.,4.)-3.))&127)+(int(pow(8e3,m))&64)&255)/255.,m,d);"
 "}"
 "void main()"
 "{"
   "vec4 f=gl_FragCoord;"
   "vec3 i=t((f.x+f.y*v.y)/44100.);"
   "d=vec4(vec3(i.x),1);"
 "}",

#endif
