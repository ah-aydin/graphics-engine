#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 inColor;

layout (location = 0) out vec3 outColor;

layout (push_constant) uniform Push {
	mat4 transform;
	vec3 color;
} push;

layout (binding = 0) uniform UniformBufferObject {
	mat4 model;
	mat4 view;
	mat4 proj;
} ubo;

void main() {
    outColor = inColor;
    //gl_Position = push.transform * vec4(position, 1.0);
	gl_Position = ubo.proj * ubo.view * ubo.model * vec4(position, 1.0);
}