#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

layout (location = 0) out vec2 outTextCoords;

layout (std140, binding = 0) uniform PerFrameData
{
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 proj;
};

void main() {
	gl_Position = proj * view * model * vec4(inPos, 1.0);

	outTextCoords = inTexCoords;
}