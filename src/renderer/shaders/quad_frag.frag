#version 450 core

layout (location = 0) out vec4 fragColor;

layout (push_constant) uniform GridParamsFragGPU {
    vec2 offset;
	int numCells;
	float thickness;
	float scroll;
};

// in vec2 texCoord;

void main() {
    // Use GridParamsFragGPU members as needed
    fragColor = vec4(1.0); // Replace with your desired output color
}