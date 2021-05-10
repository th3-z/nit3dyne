#version 330 core

in vec3 lightColor;
in vec2 texCoord;
in vec3 affineUv;

uniform sampler2D tex;

out vec4 fragColor;


void main() {
    int colors = 32;
    fragColor = texture(tex, affineUv.st / affineUv.p) * vec4(lightColor, 1.0);

    fragColor *= colors;
    fragColor = floor(fragColor);
    fragColor /= colors;
}
