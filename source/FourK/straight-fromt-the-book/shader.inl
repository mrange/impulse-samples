// Generated with Shader Minifier 1.3.6 (https://github.com/laurentlb/Shader_Minifier/)
#ifndef SHADER_MINIFIER_IMPL
#ifndef SHADER_MINIFIER_HEADER
# define SHADER_MINIFIER_HEADER
# define VAR_fcol "v"
# define VAR_state "m"
#endif

#else // if SHADER_MINIFIER_IMPL

// C:\code\github\impulse-samples\source\FourK\straight-fromt-the-book\shader.fx
"#version 430\n"
 "uniform vec4 m;"
 "out vec4 v;"
 "void main()"
 "{"
   "vec4 i=gl_FragCoord;"
   "float s=m.x+.001*i.x/m.y,a=3.2e4*(m.x==0?"
     "(i.x+i.y*m.y)/44100:"
     "s),y=a/16384,x=1-(y-floor(y))*1.6,e=float((int(mod(a,float(int(a)&int(a)>>12))/pow(2.,mod(y*16.,4.)-3.))&127)+(int(pow(8e3,x))&64)&255)/255,f=-1+2*i.y/m.z,p=abs(abs(f)-e-.125);"
   "vec3 t=1+sin(vec3(0,1,2)+s);"
   "v=vec4(m.x==0?"
     "vec3(e):"
     "sqrt(tanh(t*1e-4/max(p*p,1e-5)+x*t.zxy*.1/max(abs(f),.01))),1);"
 "}",

#endif
