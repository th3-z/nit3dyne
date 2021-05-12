#version 330 core

out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D tex;

void main()
{
    fragColor = vec4(vec3(1.0 - texture(tex, texCoord)), 1.0);
}