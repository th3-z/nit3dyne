#version 330 core

in vec3 lightColor;
in vec3 color;
//in vec3 affineUv;
//in vec2 perspectiveUv;

uniform sampler2D tex;

out vec4 fragColor;


void main() {
    fragColor = vec4(color, 1.0);// * vec4(lightColor, 1.0);
    //fragColor = texture(tex, perspectiveUv.st);
}
