/*
            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                    Version 2, December 2004

 Copyright (C) 2024 Mårten Rånge

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. You just DO WHAT THE FUCK YOU WANT TO.
*/

#version 430

precision highp float;
precision highp sampler2D;

layout(location = 0) uniform vec4 state;
layout(location = 1) uniform sampler2D texture0;

out vec4 fragColor;

void main() {
  vec2 q = 2.*gl_FragCoord.xy/(state.yz);
  vec4 tcol = texture(texture0, q);
  fragColor = vec4(tcol.xyz, 1.0);
}
