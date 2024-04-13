// Generated with Shader Minifier 1.3.6 (https://github.com/laurentlb/Shader_Minifier/)
#ifndef SHADER_MINIFIER_IMPL
#ifndef SHADER_MINIFIER_HEADER
# define SHADER_MINIFIER_HEADER
# define VAR_fcol "m"
# define VAR_sample "i"
#endif

#else // if SHADER_MINIFIER_IMPL

// C:\code\github\impulse-samples\source\FourK\straight-fromt-the-book\shader.fx
"#version 430\n"
 "uniform int i;"
 "out vec4 m;"
 "mat2 s(float i)"
 "{"
   "return mat2(cos(i),sin(i),-sin(i),cos(i));"
 "}"
 "void main()"
 "{"
   "vec2 d=vec2(1920,1080),y=(-d+2*gl_FragCoord.xy)/d.yy;"
   "float v=(i==0?"
     "gl_FragCoord.x+d.x*gl_FragCoord.y:"
     "i+.5*gl_FragCoord.x)*320/441,x=v/16384,a=floor(x),g=1.-(x-a)*1.6,c=float((int(mod(v,int(v)&int(v)>>12)/pow(2,mod(x*16,4)-3))&127)+(int(pow(8e3,g))&64)&255)/255,f=length(y);"
   "vec3 o=vec3(0),l=vec3(.125,.25,.5);"
   "if(c+abs(y.y)>1)"
     "o.x+=.25;"
   "y*=(1.5+.5*sin(-4*(f+g)))*s(-a+.5*f);"
   "for(int e=10;e>0;--e)"
     "{"
       "ivec2 p=ivec2(s(-.1*g*e)*y*12);"
       "if(abs(p.x^p.y)%99%(43^int(a+20*-abs(y.x*y.y)*step(64,a)))<e)"
         "o+=l*exp(-.2*e);"
     "}"
   "o*=f*f;"
   "o+=.5*g*g/f*sqrt(l);"
   "m=vec4(smoothstep(91,89,x)*(i==0?"
     "vec3(c):"
     "o),1);"
 "}",

#endif
