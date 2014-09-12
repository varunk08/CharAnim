#version 150 core


in vec2 TexCoord;
uniform sampler2D tex;

void main()
{
	//gl_FragColor = vec4(vColor, 1.0);
	gl_FragColor =  texture(tex, TexCoord);
}