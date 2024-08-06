#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Push {
  mat4 transform;
  mat4 modelMatrix;
} push;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout (binding = 1) uniform UboInstance {
	mat4 model; 
} uboInstance;


const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0, -3.0, -1.0));
const float AMBIENT = 0.3;

void main() {

  gl_Position = ubo.view * push.modelMatrix * vec4(position, 1.0);

  // temporary code
  // vec3 normalWorldSpace = normalize(mat3(push.modelMatrix) * normal);
  // mat3 normalMatrix = transpose(inverse(mat3(push.modelMatrix)));
  // vec3 normalWorldSpace = normalize(normalMatrix * normal);

  vec3 normalWorldSpace = normalize(mat3(push.modelMatrix) * normal);


  float lightIntensity = AMBIENT + max(dot(normalWorldSpace, DIRECTION_TO_LIGHT), 0);

  fragColor = lightIntensity * color;
}
