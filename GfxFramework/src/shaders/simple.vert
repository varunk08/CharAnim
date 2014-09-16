#version 150 core

in vec3 position;
in vec3 color;

uniform mat4 projection;
uniform mat4 model;

out vec3 fragColor;

void main(void)
{
	gl_Position = projection * model * vec4(position, 1.0f);
	fragColor = color;
}