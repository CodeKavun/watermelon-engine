#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform vec2 origin;
uniform vec2 sourceMinUV;
uniform vec2 sourceMaxUV;
uniform vec2 sourceSize;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec2 pixelPos = aPos.xy * sourceSize - origin;
    gl_Position = projection * view * model * vec4(pixelPos, aPos.z, 1.0);
    TexCoord = mix(sourceMinUV, sourceMaxUV, aTexCoord);
}