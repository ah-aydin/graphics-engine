#version 460 core

layout (location = 0) out vec4 fragColor;

layout (location = 0) in vec2 inTextCoords;

void main() {
	fragColor = vec4(0.9, 0.4, 0.78, 1.0);
}