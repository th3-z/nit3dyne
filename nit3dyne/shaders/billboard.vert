#version 330 core
layout (location = 0) in vec2 inVertex;

// TODO: pre-multiply these
uniform mat4 vp;
uniform vec3 camera_right;
uniform vec3 camera_up;
uniform vec3 position;
uniform vec2 size;
uniform bool viewScale;

out vec2 texCoord;

void main() {
    vec3 vertex = vec3(inVertex, 0.f);

    if (viewScale) {
        vertex = position
        + camera_right * vertex.x * size.x
        + camera_up * vertex.y * size.y;

        gl_Position = vp * vec4(vertex, 1.f);
    } else {
        gl_Position = vp * vec4(vertex, 1.f);
        gl_Position /= gl_Position.w;
        gl_Position.xy += inVertex;
    }

    texCoord = inVertex + vec2(0.5, 0.5);
}