#version 450

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inTexCoords;

layout (location = 0) out vec2 outTexCoords;

layout (binding = 0) uniform UniformBufferObject {
	mat4 view;
	mat4 proj;
} perFrameData;

layout (push_constant) uniform Push {
	mat2 transform;
} perObjectData;

void main() {
	gl_Position = perFrameData.proj * perFrameData.view * vec4(perObjectData.transform * inPosition, 0.0, 1.0);

	outTexCoords = inTexCoords;
}