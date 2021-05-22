#version 330 core

in vec3 texCoord;

out vec4 fragColor;

uniform samplerCube skybox;

void main() {
    fragColor = texture(skybox, texCoord);
}
