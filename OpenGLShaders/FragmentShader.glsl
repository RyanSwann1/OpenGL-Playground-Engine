#version 330 core

out vec4 color;

uniform vec3 uAmbientColor;
uniform float uAmbientStrength;

void main()
{
	//Ambient
	vec3 ambient = uAmbientColor * uAmbientStrength;
	vec3 result = ambient * vec3(1.0, 0.5, 0.31);

	color = vec4(result, 1.0); 
};