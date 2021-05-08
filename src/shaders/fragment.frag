#version 330 core

in vec3 lightColor;
in vec2 texCoord;

uniform sampler2D tex;

out vec4 fragColor;


void main() {
    fragColor = texture(tex, texCoord) * vec4(lightColor, 1.0);
}
