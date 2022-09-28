#version 450

layout (location = 0) out vec4 outColor;

layout (location = 0) in vec2 inTexCoords;

void main() {
    outColor = vec4(1.0, 0.5, 0.5, 1.0);
}