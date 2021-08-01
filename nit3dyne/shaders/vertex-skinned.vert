#version 330 core

layout (location = 0) in vec3 inVertex;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in ivec4 inJoints;
layout (location = 4) in vec4 inWeights;

const int MAX_JOINTS = 25;
const int MAX_WEIGHTS = 4;

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
out vec3 affineUv;
out vec2 perspectiveUv;

uniform vec3 sunPosition;
uniform vec3 sunColor;

uniform mat3 normalMat;
uniform mat4 modelView;
uniform mat4 mvp;
uniform mat4 jointTransforms[MAX_JOINTS];

uniform Material material;
uniform DLight dLight;
uniform SLight sLight;


void main() {
    //skinning
    vec4 localVertex = vec4(0.0);
    vec4 localNormal = vec4(0.0);

    for (int i = 0; i < MAX_WEIGHTS; i++) {
        mat4 jointTransform = jointTransforms[inJoints[i]];
        vec4 posePos = jointTransform * vec4(inVertex, 1.0);
        localVertex += posePos * inWeights[i];

        vec4 poseNormal = jointTransform * vec4(inNormal, 0.0);
        localNormal += poseNormal * inWeights[i];
    }

   // Vertex snapping
   vec4 vertex = mvp * localVertex;
   vertex.xyz = vertex.xyz / vertex.w;
   vertex.x = floor(160 * vertex.x) / 160;
   vertex.y = floor(120 * vertex.y) / 120;
   vertex.xyz *= vertex.w;
   gl_Position = vertex;

   vec3 normal = vec3(normalMat * localNormal.xyz);
   vec3 vertPos = vec3(modelView * localVertex);

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

   // Affine texture map
   affineUv = vec3(inTexCoord.st * vertPos.z, vertPos.z);
   perspectiveUv = inTexCoord.st;
}
