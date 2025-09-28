#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec4 color;

void main() {
    float radius = length(TexCoords);
    if (radius > 1.0) {
        discard;
    }

    FragColor = color;
};