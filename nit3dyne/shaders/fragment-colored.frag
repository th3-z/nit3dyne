#version 330 core

in vec3 lightColor;
in vec3 color;

out vec4 fragColor;

void main() {
    fragColor = vec4(color * lightColor, 1.0);
}
