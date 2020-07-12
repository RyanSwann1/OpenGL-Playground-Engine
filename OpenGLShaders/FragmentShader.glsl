#version 330 core

out vec4 color;

uniform vec3 uAmbientColor;
uniform float uAmbientStrength;
uniform vec3 uLightPosition;
uniform vec3 uLightColor;

in vec3 vNormal;
in vec3 vFragPosition;

void main()
{
	//Ambient
	vec3 ambient = uAmbientColor * uAmbientStrength;

	//Diffuse
	vec3 normal = normalize(vNormal);
	vec3 lightDirection = normalize(uLightPosition - vFragPosition);

	vec3 diffuse = max(dot(normal, lightDirection), 0.0) * uLightColor;

	vec3 result = (ambient + diffuse) * vec3(1.0, 0.5, 0.31);
	color = vec4(result, 1.0); 
};