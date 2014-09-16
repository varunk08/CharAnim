//vertex shader

#version 150 core

in vec2 position;
in vec2 texCoord;
uniform mat4 MVP;
out vec2 TexCoord;

void main()
{
	gl_Position = MVP * vec4(position, 0.0, 1.0);
	TexCoord = texCoord;
}