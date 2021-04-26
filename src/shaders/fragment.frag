#version 330 core
out vec4 FragColor;
in vec3 color;
in vec2 texCoord;

uniform float colorHue;
uniform sampler2D texSampler;

void main() {
    FragColor = texture(texSampler, texCoord);
    //FragColor = vec4(color / colorHue, 1.f);
}
