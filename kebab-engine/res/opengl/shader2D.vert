#version 460 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inTexCoords;

layout (location = 0) out vec2 outTextCoords;

layout (std140, binding = 0) uniform PerFrameData
{
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 proj;
};

void main() {
	//gl_Position = proj * view * model * vec4(inPos, 1.0);
	gl_Position = vec4(inPos, 0.0, 1.0);

	outTextCoords = inTexCoords;
}
