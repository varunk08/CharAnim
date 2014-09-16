#version 150 core

in vec3 fragColor;
void main(void)
{
	gl_FragColor = vec4(fragColor, 1.0f);
}