#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D image;
uniform vec4 color;

void main() {
    FragColor = texture(image, TexCoord) * color;
};