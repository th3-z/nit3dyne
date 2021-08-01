#version 330 core

layout (location = 0) in vec3 inVertex;

out vec3 texCoord;

uniform mat4 proj;
uniform mat4 view;

void main() {
    gl_Position = vec4(proj * view * vec4(inVertex, 1.0)).xyww;
    texCoord = inVertex;
}
