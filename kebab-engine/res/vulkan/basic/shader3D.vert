#version 450

layout (location = 0) in vec3 position;

layout (location = 0) out vec3 outColor;

layout (push_constant) uniform Push {
	mat4 model;
	mat4 view;
	mat4 proj;
} push;

layout (binding = 0) uniform UniformBufferObject {
	mat4 model;
	mat4 view;
	mat4 proj;
} ubo;

void main() {
    outColor = vec3(0.2, 0.6, 0.9);
    gl_Position = push.proj * push.view * push.model * vec4(position, 1.0);
	//gl_Position = ubo.proj * ubo.view * ubo.model * vec4(position, 1.0);
}