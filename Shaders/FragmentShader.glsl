#version 330 core

out vec4 color;

in vec3 vFragPosition;
in vec3 vNormal;
in vec2 vTextCoords;
in vec3 vLightPosition;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;

uniform bool uDiffuseTexture;
uniform bool uSpecularTexture;
uniform vec3 uMaterialColor;
uniform vec3 uLightColor;

const float ambientStrength = 0.2;
const float specularStrength = 0.5;
const float constantAttentuationParamater = 1.0;
const float linearAttenuationParamter = 0.0014;
const float quadraticAttenuationParameter = 0.000007;

void main()
{
	vec3 n = normalize(vNormal);
	vec3 lightDirection = normalize(vLightPosition - vFragPosition);
			
	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);

	float distance = length(vLightPosition - vFragPosition);
	float attenuation = 1.0 / (constantAttentuationParamater + linearAttenuationParamter * distance + 
		quadraticAttenuationParameter * (distance * distance));

	if(!uDiffuseTexture && !uSpecularTexture)
	{
		vec3 materialColor = vec3(0.7);
		diffuse = max(dot(lightDirection, n), 0.0) * uLightColor;
		specular = uLightColor * specularStrength * 
			(pow(max(dot(normalize(-vFragPosition), normalize(reflect(-lightDirection, n))), 0.0), 64));

		color = vec4(vec3((diffuse + specular) * materialColor) * attenuation + vec3(ambientStrength) * materialColor, 1.0);	
		return;
	}

	float diffuseAlpha = 1.0;
	if(uDiffuseTexture)
	{
		diffuse = texture(texture_diffuse, vTextCoords).rgb * max(dot(lightDirection, n), 0.0) * uLightColor;
		diffuseAlpha = texture(texture_diffuse, vTextCoords).a;
		ambient = texture(texture_diffuse, vTextCoords).rgb * ambientStrength;
	}

	if(uSpecularTexture)
	{
		specular = texture(texture_specular, vTextCoords).rgb *
			uLightColor * specularStrength * 
			(pow(max(dot(normalize(-vFragPosition), normalize(reflect(-lightDirection, n))), 0.0), 64));
	}

	color = vec4(vec3((diffuse + specular)) * attenuation + ambient, diffuseAlpha);	
	if(color.a < 0.1)
	{
		discard;
	}
};