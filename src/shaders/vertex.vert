#version 330 core

layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

out vec3 lightColor;
out vec2 texCoord;

uniform vec3 viewPosition;
uniform vec3 sunPosition;
uniform vec3 sunColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main() {
   gl_Position = projection * view * model * vec4(inVertex, 1.0);

   vec3 position = vec3(model * vec4(inVertex, 1.0));
   // TODO: Calculate normal and MVP on CPU
   vec3 normal = mat3(transpose(inverse(model))) * inNormal;

   // Ambient
   vec3 ambient = sunColor * 0.1;

   // Diffuse
   vec3 lightDir = normalize(sunPosition - position);
   vec3 diffuse = max(dot(normal, lightDir), 0.0) * sunColor;

   // Specular
   vec3 viewDir = normalize(viewPosition - position);
   vec3 reflectDir = reflect(-lightDir, normal);
   vec3 specular = 0.5 * pow(max(dot(viewDir, reflectDir), 0.0), 32) * sunColor;

   lightColor = ambient + diffuse + specular;
   texCoord = inTexCoord;
}
