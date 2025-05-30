#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

uniform mat4 MVP;
uniform vec3 COL[15000];
out vec3 vert_color;

void main() {
   // TODO: implement uniforms for the transform matrix
   // gl_Position = vec4(pos, 1.0);

   vert_color = color;
   gl_Position = MVP * vec4(pos, 1.0);
   
}