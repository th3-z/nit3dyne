#version 330 core

layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in int inTexSampler;
layout (location = 3) in vec2 inTexCoord;

struct Material {
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float shininess;
};

struct DLight {
   vec4 direction;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

out vec3 lightColor;
out vec3 affineUv;
out vec2 perspectiveUv;

uniform mat3 normalMat;
uniform mat4 modelView;
uniform mat4 mvp;

uniform Material material;
uniform DLight dLight;

void main() {
   // Vertex snapping
   vec4 vertex = mvp * vec4(inVertex, 1.0);
   vertex.xyz = vertex.xyz / vertex.w;
   vertex.x = floor(160 * vertex.x) / 160;
   vertex.y = floor(120 * vertex.y) / 120;
   vertex.xyz *= vertex.w;
   gl_Position = vertex;

   vec3 normal = normalMat * inNormal;
   vec3 vertPos = vec3(modelView * vec4(inVertex, 1.0));
   vec3 lightDir = normalize(-dLight.direction.xyz);

   vec3 ambient = dLight.ambient.xyz * material.ambient;
   vec3 diffuse = dLight.diffuse.xyz * (max(dot(normal, lightDir), 0.0) * material.diffuse);
   lightColor = diffuse + ambient;

   // Affine texture map
   affineUv = vec3(inTexCoord.st * vertPos.z, vertPos.z);
   perspectiveUv = inTexCoord.st;
}
