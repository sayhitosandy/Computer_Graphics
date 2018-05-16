#version 330

layout (location = 0)in vec2 position;
layout (location = 1)in vec3 color;

uniform mat4 projectionMatrix;
uniform vec2 delta;


out vec3 fragColor;
void main() {

	gl_Position = vec4(position+delta,0.0, 1.0);
    fragColor = color;

}
