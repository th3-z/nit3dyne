#version 330 core
layout (location = 0) in vec2 inVertex;
layout (location = 1) in vec2 inTexCoord;

uniform mat4 view;
uniform mat4 perspective;
uniform vec3 position;
uniform vec2 size;

out vec2 texCoord;

void main() {
    vec3 camera_right = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 camera_up = vec3(view[0][1], view[1][1], view[2][1]);

    vec3 vertex = vec3(inVertex, 0.f);
    vertex = position
        + camera_right * vertex.x * size.x
        + camera_up * vertex.y * size.y;

    gl_Position = perspective * view * vec4(vertex, 1.f);

    texCoord = inTexCoord;
}