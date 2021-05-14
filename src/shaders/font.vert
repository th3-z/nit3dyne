#version 330 core
layout (location = 0) in vec2 inVertex;
layout (location = 1) in vec2 inTexCoord;

out vec2 texCoord;

void main() {
    gl_Position = vec4(inVertex.x/80, inVertex.y/30, 0.0, 1.0);
    texCoord = inTexCoord;
}