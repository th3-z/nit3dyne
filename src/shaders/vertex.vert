#version 330 core

layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

out vec3 normal;
out vec3 position;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main() {
   gl_Position = projection * view * model * vec4(inVertex, 1.0);
   position = gl_Position.xyz;
   normal = normalize(mat3(projection * view * model) * inNormal);
   texCoord = inTexCoord;
}
