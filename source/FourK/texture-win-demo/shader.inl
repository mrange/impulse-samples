// Generated with Shader Minifier 1.3.6 (https://github.com/laurentlb/Shader_Minifier/)
#ifndef SHADER_MINIFIER_IMPL
#ifndef SHADER_MINIFIER_HEADER
# define SHADER_MINIFIER_HEADER
# define VAR_fragColor "v"
# define VAR_state "h"
# define VAR_texture0 "l"
#endif

#else // if SHADER_MINIFIER_IMPL

// D:\code\github\impulse-samples\source\FourK\texture-win-demo\shader.fx
"#version 430\nprecision highp float;precision highp sampler2D;"
 "layout(location=0) uniform vec4 h;"
 "layout(location=1) uniform sampler2D l;"
 "out vec4 v;"
 "void main()"
 "{"
   "vec2 n=2.*gl_FragCoord.xy/h.yz;"
   "v=vec4(texture(l,n).xyz,1);"
 "}",

#endif
