// Generated with Shader Minifier 1.3.6 (https://github.com/laurentlb/Shader_Minifier/)
#ifndef SHADER_MINIFIER_IMPL
#ifndef SHADER_MINIFIER_HEADER
# define SHADER_MINIFIER_HEADER
# define VAR_fcol "v"
# define VAR_state "a"
#endif

#else // if SHADER_MINIFIER_IMPL

// C:\code\github\impulse-samples\source\FourK\minesweeper\shader.frag
"#version 430\n"
 "layout(location=0) out vec4 v;"
 "layout(location=0) uniform vec4[12*12+2] a;"
 "const vec4 m=vec4(1,2./3.,1./3.,3);"
 "vec3 s(vec3 v)"
 "{"
   "return v.z*mix(m.xxx,clamp(abs(fract(v.xxx+m.xyz)*6.-m.www)-m.xxx,0.,1.),v.y);"
 "}"
 "float s(vec2 v,vec2 y)"
 "{"
   "vec2 m=abs(v)-y;"
   "return length(max(m,0.))+min(max(m.x,m.y),0.);"
 "}"
 "float t(vec2 v,vec2 m)"
 "{"
   "v.x=abs(v.x);"
   "float x=.5*max(m.x-m.y,0.);"
   "return v.x<x?"
     "abs(v.y)-m.y:"
     "length(v-vec2(x,0))-m.y;"
 "}"
 "vec3 s(vec3 v,vec2 m,vec3 x,vec3 y,float f,float i)"
 "{"
   "const vec2 a=vec2(.75,.075);"
   "vec2 l=abs(m);"
   "if(l.x>.5+a.y+.1||l.y>1.+a.y+.1)"
     "return v;"
   "vec2 r=m;"
   "r.y-=.5;"
   "r.y=r.y-.5;"
   "float s=round(r.y);"
   "r.y-=s;"
   "float n=t(r,a);"
   "vec2 c=m,d=sign(c);"
   "c=abs(c);"
   "c-=.5;"
   "c=c.yx;"
   "float o=t(c,a);"
   "vec2 u=m;"
   "u.y=abs(m.y);"
   "u.y-=.5;"
   "u=abs(u);"
   "float C=dot(normalize(vec2(1,-1)),u),z=n;"
   "z=min(z,o);"
   "return mix(v,(int[16](125,80,79,87,114,55,63,81,127,119,123,62,45,94,47,43)[int(mod(floor(i),16.))]&1<<int(C>0.?"
     ".5*(d.x+1.)+(d.y+1.)+3.:"
     "-s))==0?"
     "y:"
     "x,smoothstep(f,-f,z));"
 "}"
 "vec2 t(inout vec2 v)"
 "{"
   "vec2 m=vec2(.075,.1),y=floor((v+m*.5)/m);"
   "v=mod(v+m*.5,m)-m*.5;"
   "return y;"
 "}"
 "void main()"
 "{"
   "float m=a[0].x;"
   "vec2 r=a[0].yz,y=gl_FragCoord.xy/r,f=-1.+2.*y;"
   "f.x*=r.x/r.y;"
   "float c=sqrt(2.)/r.y;"
   "vec3 x=vec3(0);"
   "vec2 l=f,i=f;"
   "const float n=.9/(12*.5);"
   "float u=c/n;"
   "i/=n;"
   "i-=.5;"
   "vec2 d=round(i);"
   "i-=d;"
   "d+=12*.5;"
   "vec2 z=f;"
   "z.x-=-.0375;"
   "z.y-=-.95;"
   "vec2 o=t(z);"
   "z/=.04;"
   "float C=c/.04;"
   "int F=clamp(int(d.x+d.y*12+2),2,a.length()-1);"
   "float p=a[F].x,H=s(l,vec2(.9));"
   "if(H<0.)"
     "{"
       "float w=s(i,vec2(.4))-.05;"
       "w=abs(w)-.0125;"
       "vec2 E=i/.25;"
       "float D=c/.25;"
       "if(p<0.)"
         "{"
           "vec3 B=s(vec3(.3+.3*p/9,.5,1));"
           "x=s(x,E,B,B*.1,D,-p);"
         "}"
       "x=mix(x,vec3(1),smoothstep(u,-u,w));"
     "}"
   "if(o.y==0&&abs(o.x-.5)<5.)"
     "{"
       "vec3 w=s(vec3(.95,.9,1));"
       "x=s(x,z,w,w*.1,C,mod(m*pow(10,o.x),10.));"
     "}"
   "x=sqrt(x);"
   "v=vec4(x,1);"
 "}",

#endif
