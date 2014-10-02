#version 150 core

//attributes
in vec3 position;
in vec3 color;
in vec3 normal;

//lighting
uniform vec3 viewpos; //world space cam pos
uniform vec3 lightDiffuse;
uniform vec3 lightpos; //world space light pos

//material properties
uniform float ka;
uniform float kd;
uniform float ks;

//matrices
uniform mat3 normalmatrix; //normal matrix
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;


out vec3 fragColor;

void main(void)
{
	//vertex position
	gl_Position = projection * view * model * vec4(position, 1.0f);

	//lighting
	vec3 pos = -(view * model * vec4(position, 1.0f)).xyz;
	vec3 lpos = (view * model * vec4(lightpos, 1.0f)).xyz;
	vec3 N = normalize((view * model * vec4(normal, 1.0f)).xyz);
	vec3 L = normalize(lpos - pos);
	vec3 E = normalize( pos);

	float geom = max(dot(L, N), 0.0);
	vec3 diffColor = kd * color * geom * lightDiffuse;
	vec3 ambient = 0.3*color;
	
	vec3 H = normalize(L + E);
	float specGeom = pow( max(dot(N, H), 0.0), 100.0 );
    vec3  specular = specGeom * vec3(1.0f, 1.0f, 1.0f);
    
    if( dot(L, N) < 0.0 ) {
	specular = vec3(0.0, 0.0, 0.0);
    } 


	fragColor = ambient + diffColor + specular;
	//fragColor = color;
}