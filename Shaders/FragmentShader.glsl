#version 330 core

out vec4 color;

in vec3 vFragPosition;
in vec3 vNormal;
in vec2 vTextCoords;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;

uniform bool uDiffuseTexture;
uniform bool uSpecularTexture;
uniform vec3 uMaterialColor;

uniform vec3 uLightPosition;
uniform vec3 uLightColor;

uniform vec3 uViewPosition;

const float ambientStrength = 0.7;
const float specularStrength = 0.5;

void main()
{
	//Diffuse Lighting
	vec3 n = normalize(vNormal);
	vec3 lightDirection = normalize(uLightPosition - vFragPosition);
	vec3 diffuse = max(dot(lightDirection, n), 0.0) * uLightColor;

	//Specular Lighting
	vec3 viewDirection = normalize(uViewPosition - vFragPosition);
	vec3 reflectDirection = reflect(-lightDirection, n);
	vec3 specular = uLightColor * specularStrength * (pow(max(dot(viewDirection, reflectDirection), 0.0), 128));

	if(uDiffuseTexture)
	{
		color = texture(texture_diffuse, vTextCoords);
	}

	if(uSpecularTexture)
	{
		color = texture(texture_specular, vTextCoords);
	}

	if(!uDiffuseTexture && !uSpecularTexture)
	{
		float dotFactor = dot(vNormal, vec3(0.0, 1.0, 0.0)) * 0.5 + 0.5;
		float darkenFactor = 0.5 + dotFactor * (1.0 - 0.5);
		vec3 outputColour = uMaterialColor * darkenFactor;
		color = vec4(outputColour * ambientStrength, 1.0);
	}

	color = vec4(vec3((ambientStrength + diffuse + specular) * color.xyz), 1.0);	
};