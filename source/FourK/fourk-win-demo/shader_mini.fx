#version 430

uniform vec4 v;
out vec4 f;
const float s=acos(-1),w=.5*s,x=2*s;
const int c=max(8,0);
const float m=.25*float(12-c);
float t()
{
  return v.x;
}
vec2 n()
{
  return v.yz;
}
mat2 n(float v)
{
  float f=cos(v),s=sin(v);
  return mat2(f,s,-s,f);
}
vec4 n(vec4 s,vec4 v)
{
  float f=v.w+s.w*(1.-v.w);
  return f>0.?
    vec4((v.xyz*v.w+s.xyz*s.w*(1.-v.w))/f,f):
    vec4(0);
}
vec3 n(vec3 v,vec4 f)
{
  return mix(v,f.xyz,f.w);
}
float t(float v)
{
  float f=v*v;
  return clamp(v*(27.+f)/(27.+9.*f),-1.,1.);
}
vec3 p(float v)
{
  float f=v*.5;
  return vec3(-.25*(vec2(cos(f),sin(f*sqrt(2.)))+vec2(cos(f*sqrt(.75)),sin(f*sqrt(.6)))),v);
}
vec3 h(float v)
{
  return(p(v+.05)-p(v-.05))/.1;
}
vec3 e(float v)
{
  return(h(v+.05)-h(v-.05))/.1;
}
vec3 e(vec3 v,vec2 f,float s,float c)
{
  vec2 m=f,w=f;
  for(float r=0.;r<4.;r++)
    {
      w=fract(w*2.+.0125*s)-.5;
      float z=length(w)*exp(-length(m));
      vec3 n=(1.+cos(vec3(0,1,2)+x*(length(m)+r*.4+s*.2)))*.5;
      z=sin(z*8.+s)/8.;
      z=abs(z);
      z-=.0025;
      z=max(z,.005);
      z=.0125/z;
      z*=z;
      v+=n*z;
    }
  return.5*v;
}
vec3 e(vec2 v,float f,float s)
{
  vec3 z=vec3(0);
  vec2 r=v;
  r+=.5*sin(vec2(1,sqrt(.5))*f*.21);
  z=e(z,r,f,s);
  z-=.75*vec3(0,1,2).zyx*dot(v,v);
  return clamp(z,0.,4.);
}
vec4 e(vec3 v,vec3 f,vec3 z,vec3 s,float m,float c)
{
  vec2 r=(z-s*vec3(1,1,0)).xy;
  float w=length(r);
  r*=mix(.5,.75,.5+.5*sin(c*.071));
  float x=.0625*t()+.125*c;
  r*=n(-x);
  float p=smoothstep(.1,.15,w);
  if(p<.05)
    return vec4(0);
  vec4 i=vec4(0);
  i.xyz=e(r,x,c);
  float h=max(max(i.x,i.y),i.z)*.75;
  i.w=t(.5+w+max(h,0.))*p;
  return i;
}
float r(float v)
{
  return.5+.5*cos(v);
}
vec3 e(vec3 v,vec3 f,vec3 s,vec3 z,vec2 m)
{
  float w=length(m);
  vec2 i=m+1./n().xy;
  float x=1.75+.75*pow(w,1.5)*t(w+.9*r(10.*m.x)*r(10.*m.y));
  vec3 h=normalize(m.x*f+m.y*s+x*v),y=normalize(i.x*f+i.y*s+x*v);
  float d=floor(z.z/.25);
  vec4 l=vec4(0);
  bool C=false;
  float b=0.;
  for(int o=1;o<=12;++o)
    {
      float F=.25*d+.25*float(o),a=(F-z.z)/h.z;
      if(a>0.&&l.w<.975)
        {
          vec3 g=z+h*a;
          b=a;
          vec3 D=p(g.z);
          vec4 B=e(z,h,g,D,3.*length(g-z-y*a),d+float(o));
          float A=g.z-z.z;
          B.w*=smoothstep(0.,.025,A)*smoothstep(.25*float(12),.25*float(c),A);
          B=clamp(B,0.,1.);
          l=n(B,l);
        }
      else
        {
          C=true;
          l.w=l.w>.975?
            1.:
            l.w;
          break;
        }
    }
  return n(vec3(0),l);
}
vec3 e(vec2 v,vec2 f)
{
  float z=.25*t();
  vec3 s=p(z),w=h(z),x=e(z),c=normalize(w),m=normalize(cross(normalize(vec3(0,1,0)+x),c)),r=e(c,m,cross(c,m),s,v);
  r*=smoothstep(0.,4.,t());
  r=clamp(r,0.,1.);
  return sqrt(r);
}
void main()
{
  vec2 v=n(),s=gl_FragCoord.xy/v.xy,z=-1.+2.*s,w=z;
  z.x*=v.x/v.y;
  vec3 m=e(z,w);
  f=vec4(m,1);
}