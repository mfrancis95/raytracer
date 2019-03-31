#version 440

layout(origin_upper_left) in vec4 gl_FragCoord;

layout(binding = 3, std430) buffer PixelStorage {
    vec3 colours[];
};

out vec3 colour;

void main() {
    colour = colours[int(gl_FragCoord.x) + int(gl_FragCoord.y) * 1280];
}