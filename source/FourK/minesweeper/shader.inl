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
 "const float m=.9/(12*.5),y=m;"
 "const vec2 s=vec2(.75,.075);"
 "const int[16] l=int[16](125,80,79,87,114,55,63,81,127,119,123,62,45,94,47,43);"
 "const vec4 f=vec4(3,2,1,9)/3;"
 "vec3 t(vec3 v)"
 "{"
   "return v.z*mix(f.xxx,clamp(abs(fract(v.xxx+f.xyz)*6-f.www)-f.xxx,0,1),v.y);"
 "}"
 "vec4 t(vec4 v)"
 "{"
   "return vec4(t(vec3(v)),v.w);"
 "}"
 "const vec3 i=t(vec3(.4,.5,.002)),n=t(vec3(.55,.5,.002)),x=t(vec3(.95,.9,1));"
 "const vec4[6] r=vec4[6](t(vec4(0)),t(vec4(.55,.7,1,.125)),t(vec4(.4,.7,1,.5)),t(vec4(0,0,1,1)),t(vec4(0,.8,1,1)),t(vec4(0,.8,.25,.5)));"
 "vec2 d(inout vec2 v)"
 "{"
   "vec2 y=vec2(.075,.1),m=floor((v+y*.5)/y);"
   "v=mod(v+y*.5,y)-y*.5;"
   "return m;"
 "}"
 "float d(vec2 v,vec2 y)"
 "{"
   "vec2 f=abs(v)-y;"
   "return length(max(f,0))+min(max(f.x,f.y),0);"
 "}"
 "float t(vec2 v,vec2 f)"
 "{"
   "v.x=abs(v.x);"
   "float y=.5*max(f.x-f.y,0);"
   "return(v.x<y?"
     "abs(v.y):"
     "length(v-vec2(y,0)))-f.y;"
 "}"
 "vec3 d(vec3 v,vec2 y,vec3 x,vec3 f,float m,float i)"
 "{"
   "vec2 n=abs(y),c=y,a=y,d=sign(a),r=y;"
   "if(n.x>.5+s.y+.1||n.y>1+s.y+.1)"
     "return v;"
   "c.y-=1;"
   "float w=round(c.y);"
   "c.y-=w;"
   "a=abs(a);"
   "a=a.yx;"
   "a-=.5;"
   "r.y=abs(y.y);"
   "r.y-=.5;"
   "r=abs(r);"
   "float z=t(c,s),C=t(a,s),F=dot(normalize(vec2(1,-1)),r);"
   "return mix(v,(l[int(floor(i))]&1<<int(F>0?"
     ".5*(d.x+1)+d.y+1+3:"
     "-w))==0?"
     "f:"
     "x,smoothstep(m,-m,min(z,C)));"
 "}"
 "void main()"
 "{"
   "vec3 f=vec3(0);"
   "vec2 m=a[0].yz,t=(-m+2*gl_FragCoord.xy)/m.yy,c=(-m+2*a[1].xy)/m.yy,s=t,z=t;"
   "float l=a[0].x,C=sqrt(2)/m.y,F=C/y,w=C/.04,J=C/(.25*y);"
   "c.y=-c.y;"
   "s/=y;"
   "s-=.5;"
   "z.x-=-.0375;"
   "z.y-=-.95;"
   "vec2 o=d(z),H=round(s);"
   "s-=H;"
   "H+=12*.5;"
   "z/=.04;"
   "int u=int(H.x+H.y*12+2);"
   "vec4 E=a[u];"
   "float g=E.x,B=E.z,A=d(t,vec2(.9)),e=d(s,vec2(.4))-.05;"
   "if(A<0)"
     "{"
       "f+=n*(1./max(dot(s,s)+smoothstep(B+.125,B+3.,l),.001));"
       "if(g<0)"
         "{"
           "vec2 p=s/.25;"
           "p.x+=-p.y/8;"
           "vec3 D=.5+.5*sin(.5*vec3(-4,3,1)+.33*g-.5*p.y);"
           "f=d(f,p,D,D*.075,J,-g);"
         "}"
       "else"
         "{"
           "vec4 p=r[int(g)];"
           "f=mix(f,p.xyz,smoothstep(F,-F,e)*p.w);"
         "}"
       "e=abs(e)-.0125;"
       "f=mix(f,vec3(1),smoothstep(F,-F,e));"
     "}"
   "if(o.y==0&&abs(o.x-.5)<5)"
     "f=d(f,z,x,x*.075,w,mod(l*pow(10,o.x),10));"
   "f+=i/max(length(t-c),.001);"
   "f=sqrt(f);"
   "v=vec4(f,1);"
 "}",

#endif
