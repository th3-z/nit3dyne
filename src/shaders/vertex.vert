#version 330 core

layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

struct Material {
   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
   float shininess;
};

out vec3 lightColor;
out vec3 affineUv;

uniform vec3 sunPosition;
uniform vec3 sunColor;

uniform mat3 normalMat;
uniform mat4 modelView;
uniform mat4 mvp;

uniform Material material;


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

   // Light source intensities * material props
   vec3 amb = vec3(0.1f, 0.1f, 0.1f) * material.ambient;
   vec3 diff = vec3(0.7f, 0.75, 0.7f) * material.diffuse;
   vec3 spec = vec3(.5f, .5f, .5f) * material.specular;
   float shininess = material.shininess;

   // Ambient
   vec3 ambient = sunColor * amb; // Ambient component

   // Diffuse
   vec3 lightDir = normalize(sunPosition - vertPos);
   vec3 diffuse = sunColor * (max(dot(normal, lightDir), 0.0) * diff);

   // Specular
   vec3 viewDir = normalize(-vertPos);
   vec3 reflectDir = reflect(-lightDir, normal);
   vec3 specular = sunColor * (
      pow(max(dot(viewDir, reflectDir), 0.0), shininess) * spec
   );

   lightColor = ambient + diffuse + specular;

   // Affine texture map
   affineUv = vec3(inTexCoord.st * vertPos.z, vertPos.z);
}
