#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

layout(push_constant) uniform Model
{
	mat4 u_model;
};


layout(binding = 0) uniform Camera
{
	mat4 u_view;
	mat4 u_proj;
	mat4 u_viewProj;
};

// layout(location = 0) out vec3 fragColor;
layout(location = 0) out vec2 fragTexCoord;

void main() {
    gl_Position = u_viewProj * u_model * vec4(inPosition, 1.0);
    // fragColor = inColor;
    fragTexCoord = inTexCoord;
}