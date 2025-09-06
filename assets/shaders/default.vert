#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform vec2 uv0;
uniform vec2 uv1;
uniform vec2 uvSize;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec2 pixelPos = aPos.xy * uvSize;
    gl_Position = projection * view * model * vec4(pixelPos, aPos.z, 1.0);
    TexCoord = mix(uv0, uv1, aTexCoord);
}