#version 150 core

in vec3 position;
in vec3 color;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec3 fragColor;

void main(void)
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	fragColor = color;
}