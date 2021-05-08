#version 330 core

in vec3 normal;
in vec3 position;
in vec2 texCoord;

uniform sampler2D tex;
uniform vec3 sunPosition;
uniform vec3 sunColor;

out vec4 fragColor;


void main() {
    float lum = max(dot(normal, normalize(sunPosition)), 0.0);
    fragColor = texture(tex, texCoord) * vec4((0.3 + 0.7 * lum) * sunColor, 1.0);
}
