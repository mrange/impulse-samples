// Generated with Shader Minifier 1.3.6 (https://github.com/laurentlb/Shader_Minifier/)
#ifndef SHADER_MINIFIER_IMPL
#ifndef SHADER_MINIFIER_HEADER
# define SHADER_MINIFIER_HEADER
# define VAR_fcol "v"
# define VAR_state "s"
#endif

#else // if SHADER_MINIFIER_IMPL

// D:\code\github\impulse-samples\source\FourK\minesweeper\shader.frag
"#version 430\n"
 "const float y=1./8;"
 "out vec4 v;"
 "uniform vec4[12*12+2] s;"
 "const float a=.825/6,f=acos(-1),m=2*f,x=.825/6,w=a,r[12]=float[](5,12,0,17,14,19,16,14,15,18,19,19);"
 "vec2 d=vec2(.75,.075),n[7]=vec2[](vec2(0),vec2(.5,0),vec2(4,2),vec2(10,1),vec2(10,2),vec2(4,1),vec2(10,1));"
 "int i[20]=int[](125,80,79,87,114,55,63,81,127,119,123,62,45,94,47,43,44,251,41,0);"
 "vec3 t(float y)"
 "{"
   "return 1+sin(vec3(-4,3,1)/2+y);"
 "}"
 "vec2 t(inout vec2 v,vec2 y)"
 "{"
   "vec2 x=floor((v+y/2)/y);"
   "v=mod(v+y/2,y)-y/2;"
   "return x;"
 "}"
 "float p(vec2 v)"
 "{"
   "v*=v;"
   "v*=v;"
   "return pow(dot(v,v),1./8)-.45;"
 "}"
 "vec3 p(vec2 v,float y)"
 "{"
   "vec2 f=v*v;"
   "f*=f;"
   "float s=y*y;"
   "s*=s;"
   "s*=s;"
   "float x=s-dot(f,f);"
   "if(x<0)"
     "return vec3(0,0,1);"
   "vec3 a=vec3(v,pow(x,1./8)),m=a*a,i=m*m;"
   "i*=m*a;"
   "return normalize(i);"
 "}"
 "float e(vec2 v,vec2 y)"
 "{"
   "v.x=abs(v.x);"
   "float x=max(y.x-y.y,0)/2;"
   "return(v.x<x?"
     "abs(v.y):"
     "length(v-vec2(x,0)))-y.y;"
 "}"
 "vec3 e(vec2 v,vec3 y,vec3 x,float f,float a)"
 "{"
   "vec2 m=abs(v),r=v,s=v,t=sign(s),w=v;"
   "if(m.x>.5+d.y+.1||m.y>1+d.y+.1)"
     "return vec3(0);"
   "r.y-=1;"
   "float n=round(r.y);"
   "r.y-=n;"
   "s=abs(s);"
   "s=s.yx;"
   "s-=.5;"
   "w.y=abs(v.y);"
   "w.y-=.5;"
   "w=abs(w);"
   "float z=e(r,d),p=e(s,d),l=dot(normalize(vec2(1,-1)),w);"
   "return((i[int(floor(a))]&1<<int(l>0?"
     "(t.x+1)/2+t.y+1+3:"
     "-n))==0?"
     "x:"
     "y)*smoothstep(f,-f,min(z,p));"
 "}"
 "void main()"
 "{"
   "vec2 m=s[0].yz,a=(-m+2*gl_FragCoord.xy)/m.yy,f=(-m+2*s[1].xy)/m.yy,d=a,i=a,z=a;"
   "float l=s[0].x,C=s[0].w,F=s[1].z,I=s[1].w,H=sqrt(2)/m.y,G=H/w,E=H/.05,D=H/(.25*w),g=sign(z.y);"
   "f.y=-f.y;"
   "vec3 c=vec3(0),A=vec3(a,0),B=vec3(f,1),J=normalize(A-vec3(0,0,10)),K=normalize(B-A),L=normalize(vec3(2,3,3)),M=sqrt(t(l)),N=vec3(0,0,l*.5),O=normalize(vec3(a,2));"
   "for(int o=1;o<10;++o)"
     "{"
       "float P=(-N.x+6*sqrt(o))/abs(O).x;"
       "vec3 Q=N+O*P;"
       "vec2 R=Q.yz*.02,S=round(R),T=R-S;"
       "if(fract(sin(dot((S+o+.5*sign(O.x)).xy,vec2(12.9898,58.233)))*13758.5453)<.5)"
         "T=vec2(T.y,-T.x);"
       "float U=smoothstep(-.7,1.,sin(.1*Q.z+l*.5+o+sign(O.x))),V=length(T+.5)-.5,W=length(T-.5)-.5;"
       "c+=t(.05*P+l*.5)*exp(-.003*P*P)*.0025/max(abs(abs(min(V,W))-.025),.003*U)*U;"
     "}"
   "i=i/w-.5;"
   "z.x-=-x*.75/2+a.y/5;"
   "z.y=abs(z.y)-.9;"
   "vec2 T=t(z,vec2(.75*x,x)),o=round(i);"
   "z.y*=g;"
   "i-=o;"
   "o+=6;"
   "z/=.05;"
   "float R=o.x+o.y*12+2,P=length(f-a);"
   "if(T.y==0&&abs(T.x-.5)<6)"
     "{"
       "vec3 V=t(2.5+1.5*g+.4*z.y+(T.x<1?"
         "0:"
         "3));"
       "c+=e(z,V,V*.075,E,T.x>0?"
         "mod((g<0?"
           "I:"
           "F)*pow(10,T.x-6),10):"
         "r[int(T.x+5+3*(1-g))]);"
     "}"
   "if(max(abs(d).x,abs(d).y)<.825)"
     "{"
       "vec4 Q=s[int(R)];"
       "float V=Q.z,S=Q.w,W=p(i),U=smoothstep(S+1./2,S+1./8,C),X=smoothstep(V,V+y,C),Y=exp(-2*fract(C-P*P/8));"
       "vec3 Z=p(i,.45-1./80-U/40),u=tanh(8*c)/8;"
       "float _=1+dot(Z,J),b=pow(max(dot(L,reflect(J,Z)),0),22),h=pow(max(dot(K,reflect(J,Z)),0),44);"
       "for(int j=0;j<2;++j)"
         "{"
           "float k=j==0?"
             "Q.y:"
             "Q.x,q=j==0?"
             "1-X:"
             "X;"
           "vec2 at=n[int(k)];"
           "float ay=abs(length(i)-.1*max(U,Y));"
           "for(float ax=0;ax<at.y;++ax)"
             "ay=min(abs(ay-.1),ay);"
           "vec3 ax=(.2+t(2-k))*(at.x*.005/max(ay,.003));"
           "if(k<1)"
             "{"
               "vec2 ab=i/.25;"
               "ab.x+=-ab.y/8;"
               "vec3 av=t(k/2-ab.y/2),aa=av/20;"
               "if(k<0)"
                 "u+=e(ab,av,aa,D,-k);"
             "}"
           "else"
             " u=mix(u,ax,q*smoothstep(G,-G,W));"
           "u+=q*(b/4+h*M)*_*16*step(1,k);"
         "}"
       "c=mix(c,u,smoothstep(G,-G,W));"
       "W=abs(W)-1./80;"
       "c=mix(c,mix(t(3.+a.y)/4,vec3(1),U),smoothstep(G,-G,W));"
     "}"
   "v=vec4(sqrt(tanh(c)),1);"
 "}",

#endif
