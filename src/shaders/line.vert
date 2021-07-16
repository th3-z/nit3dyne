#version 330 core

layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inColor;

out vec3 color;

uniform mat4 mvp;


void main() {
   // Vertex snapping
   vec4 vertex = mvp * vec4(inVertex, 1.0);
   vertex.xyz = vertex.xyz / vertex.w;
   vertex.x = floor(160 * vertex.x) / 160;
   vertex.y = floor(120 * vertex.y) / 120;
   vertex.xyz *= vertex.w;
   gl_Position = vertex;


   color = inColor;
}
