#version 150 core

in vec3 position;
uniform vec3 color;
void main(void)
{
	gl_Position = vec4(position, 1.0f);
}