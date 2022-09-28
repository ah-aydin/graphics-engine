#version 450

layout (location = 0) in vec3 inposition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

layout (location = 0) out vec2 outTexCoords;

layout (binding = 0) uniform UniformBufferObject {
	mat4 view;
	mat4 proj;
} perFrameData;

layout (push_constant) uniform Push {
	mat4 model;
} perObjectData;

void main() {
    gl_Position = perFrameData.proj * perFrameData.view * perObjectData.model * vec4(inPosition, 1.0);

	outTexCoords = inTexCoords;
}