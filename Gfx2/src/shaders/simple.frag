#version 150 core
uniform vec3 lightDiffuse;

in vec3 camSpaceLightDir; //L
in vec3 camSpaceNormal; // N
in vec3 camSpaceEye; // E
in vec3 fragColor;

//out vec4 fragmentColor;
void main(void)
{

		vec3 ambient = 0.0* fragColor;

	float geom = max(dot(camSpaceLightDir, camSpaceNormal), 0.0);
	vec3 diffColor = fragColor * geom * lightDiffuse;

	
	vec3 H = normalize(camSpaceLightDir + camSpaceEye);
	float specGeom = pow( max(dot(camSpaceNormal, H), 0.0), 500.0 );
    vec3  specular = specGeom * vec3(1.0f, 1.0f, 1.0f);
    
    if( dot(camSpaceLightDir, camSpaceNormal) < 0.0 ) {
		specular = vec3(0.0, 0.0, 0.0);
    } 

	gl_FragColor = vec4(ambient + diffColor + specular, 1.0f);
	//gl_FragColor = vec4(fragColor, 1.0f);
}