#version 330 core

#define POINT_LIGHT_COUNT 3

struct Light
{
	vec3 position;
	vec3 color;
};

out vec4 color;

in vec3 vFragPosition;
in vec3 vNormal;
in vec3 vModelNormal;
in vec2 vTextCoords;
in vec3 vLightPosition;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform bool uDiffuseTexture;
uniform bool uSpecularTexture;
uniform vec3 uMaterialColor;
uniform vec3 uLightColor;
uniform Light uPointLights[POINT_LIGHT_COUNT];

const float ambientStrength = 0.1;
const float specularStrength = 0.5;
const float constantAttentuationParamater = 1.0;
const float linearAttenuationParamter = 0.007;
const float quadraticAttenuationParameter = 0.0002;
const vec3 directionalLightRotation = vec3(-0.14, 0.54, 0.0);
const vec3 directionalLightColor = vec3(1.0, 1.0, 1.0);
const float directionalLightItensity = 0.2;

vec3 calculateDirectionalLight(vec3 n)
{
	vec3 diffuse = vec3(0.0);
	vec3 nDirectionalLight = normalize(directionalLightRotation);
	if(uDiffuseTexture)
	{
		diffuse = texture(texture_diffuse, vTextCoords).rgb * 
			max(dot(nDirectionalLight, n), 0.0) * directionalLightColor * directionalLightItensity;
	}

	vec3 specular = vec3(0.0);
	if(uSpecularTexture)
	{
		specular = texture(texture_specular, vTextCoords).rgb * directionalLightColor * directionalLightItensity * specularStrength * 
			pow(max(dot(normalize(-vFragPosition), normalize(-reflect(nDirectionalLight, n))), 0.0), 64);
	}

	if(!uDiffuseTexture && !uSpecularTexture)
	{
		vec3 materialColor = vec3(0.7);
		diffuse = max(dot(nDirectionalLight, n), 0.0) * directionalLightColor * directionalLightItensity * materialColor;
		specular = directionalLightColor * directionalLightItensity * specularStrength * 
			pow(max(dot(normalize(-vFragPosition), normalize(-reflect(nDirectionalLight, n))), 0.0), 64) * materialColor;
	}

	return diffuse + specular;
}

vec3 calculatePointLight(vec3 n, Light light)
{
	vec3 lightDirection = normalize(light.position - vFragPosition);

	float distance = length(light.position - vFragPosition);
	float attenuation = 1.0 / (constantAttentuationParamater + linearAttenuationParamter * distance + 
		quadraticAttenuationParameter * (distance * distance));

	vec3 diffuse = vec3(0.0);
	if(uDiffuseTexture)
	{
		diffuse = texture(texture_diffuse, vTextCoords).rgb * max(dot(lightDirection, n), 0.0) * light.color;	
	}
	
	vec3 specular = vec3(0.0);
	if(uSpecularTexture)
	{
		specular = texture(texture_specular, vTextCoords).rgb * light.color * specularStrength * 
			pow(max(dot(normalize(-vFragPosition), normalize(reflect(-lightDirection, n))), 0.0), 64);
	}

	if(!uDiffuseTexture && !uSpecularTexture)
	{
		vec3 materialColor = vec3(0.7);
		
		diffuse = max(dot(lightDirection, n), 0.0) * light.color * materialColor;
		specular = light.color * specularStrength * 
			pow(max(dot(normalize(-vFragPosition), normalize(reflect(-lightDirection, n))), 0.0), 64) * materialColor;
	}

	return diffuse * attenuation + specular * attenuation;
}

void main()
{
	vec3 n = normalize(vNormal);
	vec3 lightDirection = normalize(vLightPosition - vFragPosition);

	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);

	if(!uDiffuseTexture && !uSpecularTexture)
	{
		vec3 materialColor = vec3(0.7);
		diffuse = max(dot(lightDirection, n), 0.0) * uLightColor * materialColor;
		specular = uLightColor * specularStrength * 
			pow(max(dot(normalize(-vFragPosition), normalize(reflect(-lightDirection, n))), 0.0), 64) * materialColor;

		vec3 result = vec3(0.0);
		for(int i = 0; i < POINT_LIGHT_COUNT; ++i)
		{
			result += calculatePointLight(n, uPointLights[i]);
		}

		color = vec4(result + calculateDirectionalLight(normalize(vModelNormal) + ambientStrength * materialColor), 1.0);
		return;
	}

	float diffuseAlpha = 1.0;
	vec3 ambient = vec3(0.0);
	if(uDiffuseTexture)
	{
		diffuseAlpha = texture(texture_diffuse, vTextCoords).a;
		ambient = texture(texture_diffuse, vTextCoords).rgb * ambientStrength;
	}

	vec3 result = vec3(0.0);
	for(int i = 0; i < POINT_LIGHT_COUNT; ++i)
	{
		result += calculatePointLight(n, uPointLights[i]);
	}

	color = vec4(vec3(result + calculateDirectionalLight(normalize(vModelNormal)) + ambient), diffuseAlpha);	
	if(color.a < 0.1)
	{
		discard;
	}
};