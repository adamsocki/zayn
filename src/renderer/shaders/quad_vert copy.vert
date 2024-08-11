#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;

layout (push_constant) uniform GridParamsVertGPU {
    	mat4 model;
};

void main() {
    gl_Position = vec4(pos, 1.0);
}