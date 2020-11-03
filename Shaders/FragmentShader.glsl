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

const float ambientStrength = 0.4;
const float specularStrength = 0.5;
const float constantAttentuationParamater = 1.0;
const float linearAttenuationParamter = 0.0014;
const float quadraticAttenuationParameter = 0.000007;

void main()
{
	vec3 n = normalize(vNormal);
	vec3 lightDirection = normalize(vLightPosition - vFragPosition);
	
	//Diffuse Lighting
	vec3 diffuse = max(dot(lightDirection, n), 0.0) * uLightColor;

	//Specular Lighting
	vec3 specular = 
		uLightColor * specularStrength * (pow(max(dot(normalize(-vFragPosition), normalize(reflect(-lightDirection, n))), 0.0), 64));

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
		color = vec4(vec3(0.7), 1.0);
	}

	float distance = length(vLightPosition - vFragPosition);
	float attenuation = 1.0 / (constantAttentuationParamater + linearAttenuationParamter * distance + 
		quadraticAttenuationParameter * (distance * distance));

	color = vec4(vec3((diffuse + specular) * color.rgb) * attenuation + vec3(ambientStrength) * color.rgb, 1.0 * color.w);	
	if(color.a < 0.1)
	{
		discard;
	}
};