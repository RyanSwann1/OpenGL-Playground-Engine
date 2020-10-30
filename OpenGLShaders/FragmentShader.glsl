#version 330 core

out vec4 color;

uniform vec3 uAmbientColor;
uniform float uAmbientStrength;
uniform vec3 uLightColor;
uniform float uSpecularStrength;

in vec3 vNormal;
in vec3 vFragPosition;
in vec3 vLightPosition;

void main()
{
	vec3 normal = normalize(vNormal);
	vec3 lightDirection = normalize(vLightPosition - vFragPosition);

	//Ambient
	vec3 ambient = uAmbientColor * uAmbientStrength;

	//Diffuse
	vec3 diffuse = max(dot(normal, lightDirection), 0.0) * uLightColor;

	//Specular
	vec3 viewDirection = normalize(-vFragPosition);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	vec3 specular = uSpecularStrength * pow(max(dot(viewDirection, reflectDirection), 0.0), 256) * uLightColor;  

	vec3 result = (ambient + diffuse + specular) * vec3(1.0, 0.5, 0.31);
	color = vec4(result, 1.0); 
};