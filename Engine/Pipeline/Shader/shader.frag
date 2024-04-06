#version 460

layout(location = 0) in vec3 FragmentColour;
layout(location = 0) out vec4 OutColour;

void main() {
    OutColour = vec4(FragmentColour, 1.0);
};