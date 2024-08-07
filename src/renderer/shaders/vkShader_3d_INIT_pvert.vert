#version 450

layout(binding = 0) uniform UniformBufferObject_x {
    mat4 model;
    mat4 view;
    mat4 proj;
    mat4 viewProj;
} ubo;


layout(binding = 3) uniform UniformBufferObjectDynamic {
    vec3 modelPos;
} uboD;


layout(push_constant) uniform Push {
  mat4 modelMatrix;
} push;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 3) in mat4 inMatModel;
layout(location = 7) in vec3 matPos;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
  // mat4 inMatModel = mat4(inMatModel_0, inMatModel_1, inMatModel_2, inMatModel_3);
    
    // gl_Position = ubo.proj * ubo.view * inMatModel * vec4(1.0, 1.0, 1.0, 1.0);
    gl_Position = ubo.proj * ubo.view * inMatModel * vec4(inPosition, 1.0);
    // gl_Position = ubo.proj * ubo.view * inMatModel * vec4(uboD.modelPos, 1.0);
    // gl_Position = ubo.proj * ubo.view * inMatModel * vec4(matPos, 1.0);
    // gl_Position = ubo.proj * ubo.view * push.modelMatrix * vec4(inPosition, 1.0);
    // gl_Position = ubo.viewProj * ubo.model * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
}