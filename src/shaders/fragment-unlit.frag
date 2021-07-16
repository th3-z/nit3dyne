#version 330 core

in vec3 lightColor;
in vec3 affineUv;
in vec2 perspectiveUv;

uniform sampler2D tex;

out vec4 fragColor;


void main() {
    fragColor = texture(tex, affineUv.st / affineUv.p);
    //fragColor = texture(tex, perspectiveUv.st);
}
