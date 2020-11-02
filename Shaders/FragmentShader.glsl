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

const vec3 lightColor = vec3(1, 1, 0);
const float ambientStrength = 0.7;

void main()
{
	//Diffuse Lighting
	vec3 n = normalize(vNormal);
	vec3 lightDirection = normalize(uLightPosition - vFragPosition);
	vec3 diffuse = max(dot(n, lightDirection), 0.0) * lightColor;

	if(uDiffuseTexture)
	{
		color = texture(texture_diffuse, vTextCoords) * ambientStrength;
	}

	if(uSpecularTexture)
	{
		color = texture(texture_specular, vTextCoords) * ambientStrength;
	}

	if(!uDiffuseTexture && !uSpecularTexture)
	{
		float dotFactor = dot(vNormal, vec3(0.0, 1.0, 0.0)) * 0.5 + 0.5;
		float darkenFactor = 0.5 + dotFactor * (1.0 - 0.5);
		vec3 outputColour = uMaterialColor * darkenFactor;
		color = vec4(outputColour * ambientStrength, 1.0);
	}

	color = vec4(vec3((ambientStrength + diffuse) * color.xyz), 1.0);
};