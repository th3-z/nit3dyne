#version 330 core

out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D tex;
uniform sampler2D texDither;
uniform float grainSeed;

float rand(vec2 p) {
    vec2 k1 = vec2(
        23.14069263277926, // e^pi (Gelfond's constant)
        2.665144142690225 // 2^sqrt(2) (Gelfond–Schneider constant)
    );
    return fract(
        cos(dot(p, k1)) * 12345.6789
    );
}

void main()
{
    int colorDepth = 32;
    vec2 dithSize = vec2(textureSize(texDither,0));
    vec2 bufSize = vec2(textureSize(tex, 0));

    fragColor = texture(tex, texCoord);

    vec3 dith = texture(texDither, texCoord*(bufSize / dithSize)).rgb;
    dith -= 0.5;

    fragColor.rgb = floor(fragColor.rgb * colorDepth + (1.0*dith)) / colorDepth;

    // Noise
    float strength = 0.0777;
    vec2 uvNoise = texCoord;
    uvNoise.y *= rand(vec2(uvNoise.y, grainSeed));
    vec4 grain = vec4(rand(uvNoise)*strength);

    fragColor += grain;
}
