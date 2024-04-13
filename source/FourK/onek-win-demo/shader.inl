// Generated with Shader Minifier 1.3.6 (https://github.com/laurentlb/Shader_Minifier/)
#ifndef SHADER_MINIFIER_IMPL
#ifndef SHADER_MINIFIER_HEADER
# define SHADER_MINIFIER_HEADER
# define VAR_fcol "f"
# define VAR_state "v"
#endif

#else // if SHADER_MINIFIER_IMPL

// C:\code\github\impulse-samples\source\FourK\onek-win-demo\shader.fx
"#version 430\n"
 "uniform vec4 v;"
 "out vec4 f;"
 "vec3 t(float v)"
 "{"
   "float f=3.2e4*v,x=f/16384.,m=floor(x),y=1.-(x-m)*1.6;"
   "return vec3(float((int(mod(f,float(int(f)&int(f)>>12))/pow(2.,mod(x*16.,4.)-3.))&127)+(int(pow(8e3,y))&64)&255)/255.,y,m);"
 "}"
 "void main()"
 "{"
   "vec4 x=gl_FragCoord;"
   "float m=v.x+.001*x.x/v.y;"
   "vec3 a=t(v.x==0?"
     "(x.x+x.y*v.y)/44100:"
     "m);"
   "float i=-1+2*x.y/v.z,y=abs(abs(i)-a.x-.125);"
   "vec3 g=1+sin(vec3(0,1,2)+m),o=vec3(0);"
   "o+=g*1e-4/max(y*y,1e-5);"
   "o+=a.y*g.zxy*.1/max(abs(i),.01);"
   "o=tanh(o);"
   "o=sqrt(o);"
   "f=vec4(v.x==0?"
     "vec3(a.x):"
     "o,1);"
 "}",

#endif
