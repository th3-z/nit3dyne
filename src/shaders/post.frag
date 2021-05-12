#version 330 core

out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D tex;
uniform sampler2D texDither;

void main()
{
    int colorDepth = 32;
    vec2 dithSize = vec2(textureSize(texDither,0));
    vec2 bufSize = vec2(textureSize(tex, 0));

    fragColor = texture(tex, texCoord);
    vec3 dith = texture(texDither, texCoord*(bufSize / dithSize)).rgb;
    dith -= 0.5;

    fragColor.rgb = floor(fragColor.rgb * colorDepth + (1.0*dith)) / colorDepth;
}