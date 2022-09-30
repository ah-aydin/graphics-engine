#version 460 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inTexCoords;

layout (location = 0) out vec2 outTextCoords;

layout (std140, binding = 0) uniform PerFrameData
{
	uniform mat4 proj;
	uniform mat4 view;
};

layout (binding = 1) uniform PerObjectData
{
	uniform mat2 model;
	uniform vec2 offset;
};

void main() {
	gl_Position = proj * view * vec4(model * inPos + offset , 0.0 , 1.0); 
	outTextCoords = inTexCoords;
}
