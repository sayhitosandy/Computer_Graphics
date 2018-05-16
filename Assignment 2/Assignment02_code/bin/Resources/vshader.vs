#version 330
layout (location = 0)in vec3 position;
layout (location = 1)in vec3 color;

uniform mat4 vModel;
uniform mat4 vView;
uniform mat4 vProjection;

out vec3 fColor;
void main(void) {
	gl_Position = vProjection * vView * vModel * vec4(position, 1.0);
	fColor = color; //Interpolate color
}
