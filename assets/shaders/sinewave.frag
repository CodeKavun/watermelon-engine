#version 460 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D image;

uniform float time;
uniform float speed;
uniform float amplitude;
uniform float frecuency;

void main() {
    vec2 uv = TexCoord;
    uv.x += sin(uv.y * frecuency + time * speed) * amplitude;
    FragColor = texture(image, uv);
}