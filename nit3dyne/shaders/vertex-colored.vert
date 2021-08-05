#version 330 core

layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inColor;

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

struct SLight {
   vec4 position;
   vec4 direction;
   float cutOff;
};

out vec3 lightColor;
out vec3 color;

uniform vec3 sunPosition;
uniform vec3 sunColor;

uniform mat3 normalMat;
uniform mat4 modelView;
uniform mat4 mvp;

uniform Material material;
uniform DLight dLight;
uniform SLight sLight;

#include "include/constant.glsl"

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


   // SpotLight
   vec3 sLightDir = normalize(sLight.position.xyz - vertPos);
   float theta = dot(sLightDir, normalize(-sLight.direction.xyz));

   vec3 sLightColor = vec3(0.0, 0.0, 0.0);
   if (theta > sLight.cutOff) {
      float dist = length(sLight.position.xyz - vertPos);
      float att =  1.0 / (dist/4 /* intensity */);
      sLightColor = vec3(.65/2, .6/2, .5/2) * att;
   }

   // Ambient
   vec3 ambient = dLight.ambient.xyz * material.ambient; // Ambient component

   // Diffuse
   vec3 diffuse = dLight.diffuse.xyz * (max(dot(normal, lightDir), 0.0) * material.diffuse);

   // Specular
   vec3 viewDir = normalize(-vertPos);
   vec3 reflectDir = reflect(-lightDir, normal);
   vec3 specular = dLight.specular.xyz * (
      pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * material.specular
   );

   lightColor = ambient + diffuse + specular + sLightColor;
   color = inColor;
}
